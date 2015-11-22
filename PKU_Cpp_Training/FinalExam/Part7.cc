#include <iostream>
#include <string>
#include <list>


class A
{
  private:
    std::string name;

  public:
    A(std::string n)
      :name(n)
    {}

    friend bool operator < (const class A& a1, const class A &a2);

    friend bool operator == (const class A &a1, const class A &a2)
    {
        if (a1.name.size() == a2.name.size())
            return true;
        else
            return false;
    }

    friend std::ostream & operator << (std::ostream &o, const A &a)
    {
        o << a.name;
        return o;
    }

    std::string get_name() const
    {
        return name;
    }

    int get_size() const
    {
        return name.size();
    }
};


// Overload the less-than operator for class A.
bool operator< (const A& lhs, const A& rhs)
{
    return lhs.get_size() < rhs.get_size();
}

// Implement the Print() function object.
/*
template <typename T>
class Print
{
  public:
    Print()
    {}

    ~Print()
    {}

    void operator() (const T& obj)
    {
        std::cout << obj;
    }
};
*/
class Print
{
  public:
    Print()
    {}

    ~Print()
    {}

    void operator() (const A& obj)
    {
        std::cout << obj;
    }
};

// Implement the for_each() function.
template <typename Iterator, typename Functor>
Functor Show(Iterator bgn, Iterator end, Functor func)
{
    --end;
    while (bgn != end) {
        func(*bgn);
        std::cout << ' ';
        ++bgn;
    }
    func(*bgn);
    return func;
}

// Implement the MyLarge template function object.
template <typename T>
class MyLarge
{
  public:
    MyLarge()
    {}

    ~MyLarge()
    {}

    bool operator() (const T& obj_src, const T& obj_tge)
    {
        return obj_src.get_name().at(0) < obj_tge.get_name().at(0);
    }
};

// End


int main(int argc, char* argv[])
{
    std::list<A> lst;
    int ncase, n, loop = 1;
    std::string s;
    std::cin >> ncase;
    while (ncase--) {
        std::cout << "Case: "<< loop++ << std::endl;
        std::cin >> n;
        for (int i = 0; i < n; i++){
            std::cin >> s;
            lst.push_back(A(s));
        }
        lst.sort();
        Show(lst.begin(), lst.end(), Print());
        std::cout << std::endl;

        lst.sort(MyLarge<A>());
        Show(lst.begin(), lst.end(), Print());
        std::cout << std::endl;

        lst.clear();
    }

    return 0;
}