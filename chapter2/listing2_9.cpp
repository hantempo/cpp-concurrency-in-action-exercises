#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template<typename Iterator, typename T>
class accumulate_block
{
public:
    void operator()(Iterator first, Iterator last, T& result)
    {
        std::cout << "block_first=" << *first << " block_last=" << *(last-1) << std::endl;
        result = std::accumulate(first, last, result);
        std::cout << "block_result=" << result << std::endl;
    }
};

template<typename Iterator, typename T>
void parallel_accumulate(Iterator first, Iterator last, T &init)
{
    const unsigned long length = std::distance(first, last);
    std::cout << "count=" << length << std::endl;

    if (!length)
    {
        return;
    }

    std::cout << "*first=" << *first
              << " *(--last)=" << *(last-1)
              << std::endl;

    const unsigned long min_per_thread = 25;
    const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    std::cout << "max_threads=" << max_threads << std::endl;

    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    std::cout << "hardward_threads=" << hardware_threads << std::endl;
    const unsigned long num_threads = std::min(max_threads,
        hardware_threads != 0 ? hardware_threads : 2);
    std::cout << "num_threads=" << num_threads << std::endl;
    const unsigned long block_size = length / num_threads;
    std::cout << "block_size=" << block_size << std::endl;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    // Accumulate the left
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
    
    for (auto &entry: threads)
        entry.join();

    init = std::accumulate(results.begin(), results.end(), init);
}

int main()
{
    std::vector<unsigned> v;
    for (unsigned i = 0; i < 100000; ++i)
    {
        v.push_back(i);
    }

    unsigned result = 0;
    typedef unsigned T;
    typedef std::vector<T>::const_iterator Iterator;
    Iterator block_start = v.begin();
    Iterator block_end = v.end();
    std::thread t(parallel_accumulate<Iterator, T>, block_start, block_end, std::ref(result));
    t.join();
    std::cout << "Result: " << result << std::endl;
}
