//
// Created by Kiki on 30/03/2025.
//

#include "../inc/CxHighResolutionTimer.h"
#include <iostream>
#include <numeric>
#include <cmath>

HighResolutionTimer::HighResolutionTimer()
{
    reset();
}
HighResolutionTimer::HighResolutionTimer(const int l_ct_nbLap)
{
    reset();
    m_t_lap.resize(l_ct_nbLap);
}

HighResolutionTimer::~HighResolutionTimer() = default;

void HighResolutionTimer::reset() {
    m_t_start = clock_type::now();
    m_t_lap.clear();
    m_i_index = 0;
}

// Enregistre le temps écoulé depuis le dernier lap ou reset
void HighResolutionTimer::lap() {
    const time_point l_ct_now = clock_type::now();
    m_t_lap.push_back(std::chrono::duration_cast<clock_ns>(l_ct_now - m_t_start));
    m_t_start = l_ct_now;
}

void HighResolutionTimer::lap_static() {
    if (m_i_index >= m_t_lap.size()) {
        throw std::out_of_range("Timer capacity exceeded");
    }

    const time_point l_ct_now = clock_type::now();
    m_t_lap.at(m_i_index) = (std::chrono::duration_cast<clock_ns>(l_ct_now - m_t_start));
    m_t_start = l_ct_now;
    m_i_index++;
}

template<typename DurationType>
std::vector<double> HighResolutionTimer::getTimesAs() const {
    std::vector<double> result;
    result.reserve(m_t_lap.size());

    for (const auto& time : m_t_lap) {
        result.push_back(std::chrono::duration_cast<DurationType>(time).count());
    }

    return result;
}

// Affiche les statistiques des mesures
void HighResolutionTimer::printStats() const {
    if (m_t_lap.empty()) {
        std::cout << "No measurements recorded\n";
        return;
    }

    // Utiliser une référence pour éviter la copie
    const auto& times = m_t_lap;

    // Trouver min et max sans tri complet
    auto [min_it, max_it] = std::minmax_element(times.begin(), times.end());
    clock_ns min_time = *min_it;
    clock_ns max_time = *max_it;

    // Calculer la moyenne efficacement
    clock_ns sum = std::accumulate(times.begin(), times.end(), clock_ns(0));
    clock_ns avg = sum / times.size();

    // Médiane (nécessite une copie partielle pour le tri)
    std::vector<clock_ns> sorted_times;
    sorted_times.reserve(times.size()); // Réserver la mémoire à l'avance
    sorted_times = times; // Copie nécessaire pour le tri
    std::sort(sorted_times.begin(), sorted_times.end());

    clock_ns median;
    size_t size = sorted_times.size();
    if (size % 2 == 0) {
        median = (sorted_times[size/2 - 1] + sorted_times[size/2]) / 2;
    } else {
        median = sorted_times[size/2];
    }

    // Calcul de l'écart-type (bonus)
    double variance = 0.0;
    for (const auto& time : times) {
        double diff = (time - avg).count();
        variance += diff * diff;
    }
    variance /= times.size();
    double stddev = std::sqrt(variance);

    // Affichage formaté pour une meilleure lisibilité
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Statistics (in nanoseconds):\n";
    std::cout << "Min  : " << min_time.count() << " ns\n";
    std::cout << "Max  : " << max_time.count() << " ns\n";
    std::cout << "Avg  : " << avg.count() << " ns\n";
    std::cout << "Med  : " << median.count() << " ns\n";
    std::cout << "StdDev: " << stddev << " ns\n";
}
