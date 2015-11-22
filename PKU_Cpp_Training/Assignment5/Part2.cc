#include <iostream>

using namespace std;


// Pad the designated "A" class, but we cannot implement the constructor for it.
class A {
  public:
    virtual ~A()
    { cout << "destructor A" << endl; }
};

class B : public A {
  public:
    ~B()
    { cout << "destructor B" << endl; }
};


int main()
{
    A *pa;
    pa = new B;
    delete pa;
    return 0;
}