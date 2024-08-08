#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "MultiUniset.h"

namespace KDebugger
{
struct SolverThread
{
    void start()
    {
        solver_thread = std::thread(std::mem_fn(&SolverThread::run), this);
        solver_thread.detach();
    }

    std::thread solver_thread;
    bool well_ = true;
    bool done_ = false;
    int result_ = -1;
    /* data */
private:
    void run()
    {
        for (int i = 0; i < 20; ++i)
        {
            if (!well_)
            {
                done_ = true;
                std::cout << "thread break\n";
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "thread done\n";
        result_ = 3;
        done_ = true;
    }
};
} // namespace KDebugger