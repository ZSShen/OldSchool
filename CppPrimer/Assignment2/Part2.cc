#include <iostream>

using namespace std;


class Sample {
  public:
    int v;
    Sample(int n) : v(n) {}

    // Pad the copy constructor.
    Sample(const Sample& that) { this->v = that.v + 5; }
};


int main()
{
    Sample a(5);
    Sample b = a;
    cout << b.v;
    return 0;
}


