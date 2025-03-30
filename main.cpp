#include "inc/CxHighResolutionTimer.h"

#include <chrono>
#include <thread>

int main() {
    int n = 100;
    HighResolutionTimer timer(n);

    // Test simple : mesurons quelques opérations
    for (int i = 0; i < n; ++i) {
        // Opération à mesurer (simulation avec sleep)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timer.lap_static();
    }

    timer.printStats();

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.