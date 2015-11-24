#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>


// Implement the class A and B.
class A
{
  private:
    char type_;
    int age_;

  public:
    A(int age, char type = 'A')
      : type_(type),
        age_(age)
    {}

    virtual ~A()
    {}

    // Remember that getters should always be qualified as "const".
    char GetType() const
    {
        return type_;
    }

    int GetAge() const
    {
        return age_;
    }
};

class B : public A
{
  public:
    B(int age, char type = 'B')
      : A(age, type)
    {}

    ~B()
    {}
};


// Implement the comparator object for std::set.
class Comp
{
  public:
    Comp()
    {}

    ~Comp()
    {}

    bool operator() (const A* const lhs, const A* const rhs) const
    {
        return lhs->GetAge() <= rhs->GetAge();
    }
};

// Implement the helper function object for for_each() function.
class PrintClass
{
  public:
    PrintClass()
    {}

    ~PrintClass()
    {}

    void operator() (A* obj)
    {
        std::cout << obj->GetType() << ' ' << obj->GetAge() << std::endl;
        delete obj;
    }
} Print;
// End


int main()
{
    int t;
    std::cin >> t;
    std::set<A*, Comp> ct;
    while (t--) {
        int n;
        std::cin >> n;
        ct.clear();
        for (int i = 0 ; i < n ; ++i) {
            char c;
            int k;
            std::cin >> c >> k;

            if (c == 'A')
                ct.insert(new A(k));
            else
                ct.insert(new B(k));
        }

        for_each(ct.begin(), ct.end(), Print);
        std::cout << "****" << std::endl;
    }

    return 0;
}