#include <algorithm>
#include <future>
#include <iostream>
#include <list>

template <typename T>
void print_list(const std::string &prefix, const std::list<T> &input)
{
    std::cout << prefix;
    for (auto &item : input)
    {
        std::cout << " " << item;
    }
    std::cout << std::endl;
}

template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    print_list("Input:", input);

    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());

    const T& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    auto sorted_lower = sequential_quick_sort(std::move(lower_part));
    auto sorted_higher = sequential_quick_sort(std::move(input));

    result.splice(result.begin(), sorted_lower);
    result.splice(result.end(), sorted_higher);

    print_list("Output:", result);
    return result;
}

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());

    const T& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    std::future<std::list<T> > sorted_lower = std::async(&parallel_quick_sort<T>, std::move(lower_part));
    auto sorted_higher = parallel_quick_sort(std::move(input));

    result.splice(result.begin(), sorted_lower.get());
    result.splice(result.end(), sorted_higher);

    return result;
}

int main()
{
    std::list<int> list_to_sort = {2, 4, 6, 7, 8, 3, 5, 1, 0};
    std::list<int> list_sorted = parallel_quick_sort(list_to_sort);
    print_list("Sorted:", list_sorted);
}
