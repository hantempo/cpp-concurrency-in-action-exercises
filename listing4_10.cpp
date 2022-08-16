#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void accumulate(std::vector<int>::const_iterator begin,
    std::vector<int>::const_iterator end,
    std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(begin, end, 0);
    accumulate_promise.set_value(sum);
}

void do_work(std::promise<void> barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
        std::move(accumulate_promise));

    std::cout << "Result : " << accumulate_future.get() << std::endl;
    work_thread.join();

    std::promise<void> barrier_promise;
    std::future<void> barrier_future = barrier_promise.get_future();
    std::thread network_thread(do_work, std::move(barrier_promise));
    barrier_future.wait();
    network_thread.join();
}
