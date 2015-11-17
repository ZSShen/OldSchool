#include <iostream>
#include <string>
using namespace std;


// Implement our own istream_iterator.
template <typename T>
class CMyistream_iterator
{
  private:
    istream& cin_;
    T cache_;

  public:
    CMyistream_iterator(istream& cin)
      : cin_(cin)
    {
        cin_ >> cache_;
    }

    CMyistream_iterator(CMyistream_iterator& lhs)
      : cin_(lhs.cin_),
        cache_(lhs.cache_)
    {}

    T& operator*()
    {
        return cache_;
    }

    CMyistream_iterator operator++(int)
    {
        CMyistream_iterator copy(*this);
        cin_ >> cache_;
        return copy;
    }
};


int main()
{
    CMyistream_iterator<int> inputInt(cin);

    int n1 = * inputInt;
    int tmp = * inputInt;
    cout << tmp << endl;

    inputInt ++;
    int n2 = * inputInt;

    inputInt ++;
    int n3 = * inputInt;
    cout << n1 << "," << n2<< "," << n3 << endl;

    CMyistream_iterator<string> inputStr(cin);

    string s1 = * inputStr;
    inputStr ++;

    string s2 = * inputStr;
    cout << s1 << "," << s2 << endl;

    return 0;
}