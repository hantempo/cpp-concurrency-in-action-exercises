#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>

std::condition_variable cv;
// This mutex is used for three purposes:
// 1) to synchronize accesses to i
// 2) to synchronize accesses to std::cerr
// 3) for the condition variable cv
std::mutex cv_m;

int i = 0;

void waits()
{
    std::unique_lock<std::mutex> lock(cv_m);
    std::cerr << "Waiting..." << std::endl;
    cv.wait(lock, []{return i == 1;});
    std::cerr << "Finished waiting." << std::endl;
}

void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(cv_m);
        std::cerr << "Notifying..." << std::endl;
    }
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(cv_m);
        i = 1;
        std::cerr << "Notifying again..." << std::endl;
    }
    cv.notify_all();
}

int main()
{
    std::thread t1(waits), t2(waits), t3(waits), t4(signals); 
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
