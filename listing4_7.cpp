#include <cmath>
#include <future>
#include <iostream>
#include <mutex>
#include <string>

std::mutex m;

struct X
{
    void foo(int value, const std::string &str)
    {
        std::lock_guard<std::mutex> lock(m); 
        std::cout << "X::foo " << value << " " << str << std::endl;
    }

    std::string bar(const std::string &str)
    {
        std::lock_guard<std::mutex> lock(m); 
        std::cout << "X::bar " << str << std::endl;
        return str;
    }
};

struct Y
{
    double operator()(double value)
    {
        std::lock_guard<std::mutex> lock(m); 
        std::cout << "Y::operator() " << value << std::endl;
        return value; 
    }
};

double square_root(double v)
{
    if (v < 0)
    {
        throw std::out_of_range("x<0");
    }
    return std::sqrt(v);
}

int main()
{
    X x;
    auto f1 = std::async(std::launch::deferred, &X::foo, &x, 42, "hello");
    auto f2 = std::async(std::launch::deferred, &X::bar, x, "goodbye");

    Y y;
    auto f3 = std::async(std::launch::async, Y(), 3.141);
    auto f4 = std::async(std::launch::async, std::ref(y), 2.145);

    auto f5 = std::async(square_root, -1);
    const double f5_result = f5.get();
}
