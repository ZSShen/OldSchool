#include <iostream>


// Implement the helper function object.
template <typename T>
class Square
{
  public:
    Square()
    {}

    ~Square()
    {}

    T operator() (T num)
    {
        return num * num;
    }
};

// Implement the template function.
template <typename T, typename Functor>
T sum(T* array, std::size_t size, Functor func)
{
    T result = 0;
    for (std::size_t i = 0 ; i < size ; ++i)
        result += func(array[i]);
    return result;
}


int sqr(int n)
{
    return n * n;
}


int main()
{
    int t, n, a[0x100];
    std::cin >> t;
    for (int c = 0 ; c < t ; ++c) {
        std::cin >> n;
        for (int i = 0 ; i < n ; ++i) std::cin >> a[i];
        std::cout << sum(a, n, sqr) << std::endl;
    }

    return 0;
}