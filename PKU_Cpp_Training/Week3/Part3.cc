#include <iostream>

using namespace std;


class Base
{
  public:
    int k;
    Base(int n) : k(n) {}
};

class Big
{
  public:
    int v;
    Base b;

    // Pad the designated constructor.
    Big(int n)
      : v(n),
        b(n)
    {}

    // Pad the designated copy constructor.
    Big(const Big& rhs)
      : v(rhs.v),
        b(rhs.v)
    {}
};

int main()
{
    Big a1(5);
    Big a2 = a1;
    cout << a1.v << "," << a1.b.k << endl;
    cout << a2.v << "," << a2.b.k << endl;
    return 0;
}

