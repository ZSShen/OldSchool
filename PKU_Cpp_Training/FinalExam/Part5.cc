#include <iostream>
#include <vector>
#include <algorithm>


// Implement the function object here.
class CMy_add
{
  private:
    int& sum_;

  public:
    CMy_add(int& sum)
      : sum_(sum)
    {}

    void operator() (int n)
    {
        sum_ += n & 0x7;
    }
};
// End

int main(int argc, char* argv[])
{
    int v, my_sum = 0;
    std::vector<int> vec;
    std::cin >> v;
    while (v) {
        vec.push_back(v);
        std::cin >> v;
    }
    std::for_each(vec.begin(), vec.end(), CMy_add(my_sum));
    std::cout << my_sum << std::endl;
    return 0;
}