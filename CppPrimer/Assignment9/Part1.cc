#include <iostream>


class Number
{
  private:
    int num;

  public:
    Number(int n = 0)
      : num(n)
    {}

    // Overload the operators here.
    const Number operator*(const Number& rhs)
    {
        return Number(this->num * rhs.num);
    }

    Number& operator=(const Number& rhs)
    {
        num = rhs.num;
    }

    operator int()
    {
        return num;
    }
    // End

};


int main()
{
    Number n1(10), n2(20);
    Number n3;
    n3 = n1 * n2;
    std::cout << int(n3) << std::endl;
    return 0;
}