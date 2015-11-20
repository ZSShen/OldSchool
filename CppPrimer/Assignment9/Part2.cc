#include <iostream>


class Number
{
  private:
    int num;

  public:
    Number(int n)
      : num(n)
    {}

    // Overload the operators here.
    Number(const Number& rhs)
      : num(rhs.num)
    {}

    Number& operator=(const Number& rhs)
    {
        this->num = rhs.num;
        return *this;
    }

    Number& value()
    {
        return *this;
    }

    void operator+(const Number& rhs)
    {
        this->num += rhs.num;
    }

    friend std::ostream& operator<<(std::ostream& os, const Number& obj)
    {
        os << obj.num;
        return os;
    }
    // End
};


int main()
{
    Number a(2);
    Number b = a;
    std::cout << a.value() << std::endl;
    std::cout << b.value() << std::endl;
    a.value() = 8;
    std::cout << a.value() << std::endl;
    a + b;
    std::cout << a.value() << std::endl;
    return 0;
}