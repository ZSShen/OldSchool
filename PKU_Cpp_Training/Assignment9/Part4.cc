#include <iostream>


// Implement the class here.
class CType
{
  private:
    int num;

  public:
    CType (int n = 0)
      : num(n)
    {}

    CType (const CType& obj)
      : num(obj.num)
    {}

    ~CType()
    {}

    void setvalue(int n)
    {
        num = n;
    }

    CType operator++(int)
    {
        CType obj(*this);
        this->num *= this->num;
        return obj;
    }

    friend std::ostream& operator<<(std::ostream& os, const CType& obj)
    {
        os << obj.num;
        return os;
    }
};
// End


int main(int argc, char* argv[])
{
    CType obj;
    int n;
    std::cin >> n;
    while (n) {
        obj.setvalue(n);
        std::cout << obj++ << " " << obj << std::endl;
        std::cin >> n;
    }
    return 0;
}