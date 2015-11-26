#include <iostream>

using namespace std;


class Sample
{
  public:
    int v;
    Sample(int n) : v(n) {}

    // Code the copy constructor.
    Sample(const Sample& rhs)
    {
    	this->v = rhs.v + 5;
    }
};


int main()
{
    Sample a(5);
    Sample b = a;
    cout << b.v;
    return 0;
}


