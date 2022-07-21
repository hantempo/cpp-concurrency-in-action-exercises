#include <climits>
#include <iostream>
#include <mutex>

class hierarchical_mutex
{
public:
    explicit hierarchical_mutex(unsigned long value) :
        hierarchy_value(value),
        previous_hierarchy_value(0)
    {
    }

    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        if (this_thread_hierarchy_value != hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();

        std::cout << __func__
                  << " this_thread_hierarchy_value=" << this_thread_hierarchy_value
                  << " hierarchy_value=" << hierarchy_value
                  << " previous_hierarchy_value=" << previous_hierarchy_value
                  << std::endl;
    }

private:
    void check_for_hierarchy_violation()
    {
        std::cout << __func__
                  << " this_thread_hierarchy_value=" << this_thread_hierarchy_value
                  << " hierarchy_value=" << hierarchy_value
                  << " previous_hierarchy_value=" << previous_hierarchy_value
                  << std::endl;
        if (this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;

        std::cout << __func__
                  << " this_thread_hierarchy_value=" << this_thread_hierarchy_value
                  << " hierarchy_value=" << hierarchy_value
                  << " previous_hierarchy_value=" << previous_hierarchy_value
                  << std::endl;
    }

    std::mutex internal_mutex;
    const unsigned long hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

int main()
{
    hierarchical_mutex high_level_mutex(10000);
    hierarchical_mutex middle_level_mutex(6000);
    hierarchical_mutex low_level_mutex(5000);

    std::lock_guard<hierarchical_mutex> high_level_lock(high_level_mutex);
    std::lock_guard<hierarchical_mutex> low_level_lock(low_level_mutex);
    std::lock_guard<hierarchical_mutex> middle_level_lock(middle_level_mutex);
}
