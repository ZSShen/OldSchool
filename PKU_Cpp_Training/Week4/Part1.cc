#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;


class Complex {
  private:
    double r,i;

  public:
    void Print()
    { cout << r << "+" << i << "i" << endl; }

    // Overload the designated assignment operator.
    #define SIZE_BLAH_BUF  (0x100)

    Complex& operator=(const string& str)
    {
        const char *cstr = str.c_str();

        char buf[SIZE_BLAH_BUF];
        int bgn = 0, end = 0;
        while (cstr[end] != '+')
            buf[bgn++] = cstr[end++];
        buf[bgn] = 0;
        this->r = atoi(buf);

        bgn = 0;
        ++end;
        while (cstr[end] != 'i')
            buf[bgn++] = cstr[end++];
        buf[bgn] = 0;
        this->i = atoi(buf);
    }
};


int main()
{
    Complex a;
    a = "3+4i";
    a.Print();

    a = "5+6i";
    a.Print();

    return 0;
}