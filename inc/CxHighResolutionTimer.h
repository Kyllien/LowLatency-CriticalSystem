//
// Created by Kiki on 30/03/2025.
//

#ifndef CXHIGHRESOLUTIONTIMER_H
#define CXHIGHRESOLUTIONTIMER_H

#include <chrono>
#include <vector>

/////// Types ///////
using clock_type = std::chrono::high_resolution_clock;
using time_point = clock_type::time_point;
using clock_ns   = std::chrono::nanoseconds;

/////// Const ///////

class HighResolutionTimer {
public:
    HighResolutionTimer();
    HighResolutionTimer(const int);

    ~HighResolutionTimer();

    void reset();

    // Enregistre le temps écoulé depuis le dernier lap ou reset
    void lap();
    void lap_static();

    // Affiche les statistiques des mesures
    void printStats() const;

    // Récupérer les résultats sans copie (pour traitement externe)
    std::vector<clock_ns> getTimes() && {
        return std::move(m_t_lap);
    }

    template<typename DurationType = std::chrono::microseconds>
    std::vector<double> getTimesAs() const;

private:
    time_point m_t_start;
    std::vector<clock_ns> m_t_lap;
    int m_i_index;
};

#endif //CXHIGHRESOLUTIONTIMER_H
