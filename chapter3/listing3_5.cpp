#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>

class empty_stack : public std::exception
{
public:
    const char *what() const throw()
    {
        return "Empty stack";
    }
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack()
    {
    }

    threadsafe_stack(const threadsafe_stack &other)
    {
        std::lock_guard<std::mutex> lock(other.mutex);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack &) = delete;

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        data.push(std::move(value));
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return data.empty();
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (data.empty()) throw empty_stack();
        const std::shared_ptr<T> res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

private:
    std::stack<T> data;
    mutable std::mutex mutex;
};

int main()
{
    threadsafe_stack<int> stack;
    std::cout << "empty=" << stack.empty() << std::endl;

    stack.push(2);
    std::cout << "pushed one item" << std::endl;
    std::cout << "empty=" << stack.empty() << std::endl;

    threadsafe_stack<int> other_stack = stack;
    std::cout << "copied" << std::endl;
    std::cout << "empty=" << stack.empty() << " other_empty=" << other_stack.empty() << std::endl;

    int value = 0;
    stack.pop(value);
    std::cout << "popped_value=" << value << " empty=" << stack.empty() << " other_empty=" << other_stack.empty() << std::endl;

    std::shared_ptr<int> popped_value = other_stack.pop();
    std::cout << "popped_value=" << *popped_value << " empty=" << stack.empty() << " other_empty=" << other_stack.empty() << std::endl;
}
