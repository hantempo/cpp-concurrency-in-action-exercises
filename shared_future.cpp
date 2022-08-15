#include <chrono>
#include <future>
#include <iostream>

std::chrono::time_point<std::chrono::high_resolution_clock> start;

std::chrono::duration<double, std::milli> fun(std::shared_future<void> ready_future, std::promise<void> ready_promise)
{
    ready_promise.set_value(); 
    ready_future.wait();
    return std::chrono::high_resolution_clock::now() - start;
}

int main()
{
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future = ready_promise.get_future();

    auto future1 = t1_ready_promise.get_future();
    auto future2 = t2_ready_promise.get_future();

    auto result1 = std::async(std::launch::async, fun, ready_future, std::move(t1_ready_promise));
    auto result2 = std::async(std::launch::async, fun, ready_future, std::move(t2_ready_promise));

    future1.wait();
    future2.wait();

    start = std::chrono::high_resolution_clock::now();

    ready_promise.set_value();

    std::cout << "thread 1 received the signal " << result1.get().count() << " ms after start" << std::endl;
    std::cout << "thread 2 received the signal " << result2.get().count() << " ms after start" << std::endl;
}
