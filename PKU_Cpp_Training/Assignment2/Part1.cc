#include <iostream>

using namespace std;


class A {
  public:
    int val;

    // Pad the designated constructor.
    A(int n = 0) { val = n; }

    // Pad the designated function.
    int& GetObj() { return val; }
};


int main()
{
    A a;
    cout << a.val << endl;
    a.GetObj() = 5;
    cout << a.val << endl;
}
