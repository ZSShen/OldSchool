#include <iostream>
#include <map>


// Implement the class A and B here.
class A
{
  private:
    int num_;

  public:
    static int count; // Declaration for the member static variable.

    A()
      : num_(0)
    {
        ++count;
    }

    A(int num)
      : num_(num)
    {
        ++count;
    }

    A(const A& obj)
      : num_(obj.num_)
    {
        ++count;
    }

    virtual ~A() // Please be aware of virtual destructor.
    {
        --count;
        std::cout << "A::destructor" << std::endl;
    }
};

class B : public A
{
  public:
    B(int num)
      : A(num)
    {}

    B(const B& obj)
      : A()
    {}

    ~B()
    {
        std::cout << "B::destructor" << std::endl;
    }
};
// End


int A::count = 0;
void func(B b) {}


int main()
{
    A a1(5), a2;
    std::cout << A::count << std::endl;

    B b1(4);
    std::cout << A::count << std::endl;

    func(b1); // Function call will trigger B's copy constructor.
    std::cout << A::count << std::endl;

    A* pa = new B(4);
    std::cout << A::count << std::endl;

    delete pa;
    std::cout << A::count << std::endl;
    return 0;
}