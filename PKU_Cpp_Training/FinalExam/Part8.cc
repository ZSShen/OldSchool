#include <iostream>


template <typename T>
class MagicNum
{
  private:
    T num_;

  public:
    MagicNum(T n, T i, T j)
      : num_(0)
    {
        T mask = 0x1 << i;
        T pivot = (n & mask) >> i;
        T rev_pivot = pivot ^ 0x1;

        num_ |= pivot << i;
        for (T idx = i + 1 ; idx < j ; ++idx)
            num_ |= 0x1 << idx;
        num_ |= rev_pivot << j;
    }

    ~MagicNum()
    {}

    friend std::ostream& operator<< (std::ostream& os, const MagicNum& obj)
    {
        os << obj.num_;
        return os;
    }
};



int main()
{
    int t;
    std::cin >> t;

    for (int idx = 0 ; idx < t ; ++idx) {
        int n, i, j;
        std::cin >> n >> i >> j;
        MagicNum<int> magic_num(n, i, j);
        std::cout << std::hex << magic_num << std::endl;
    }
}