#include <iostream>
#include <cstring>
#include <cstdlib>
#include <memory>

using namespace std;


// Code the designated "MyString" class. Just simply inherit the functionality
// from STL string class.
/*
class MyString : public string
{
  public:
    MyString()
      : string()
    {}

    MyString(const char* str)
      : string(str)
    {}

    MyString(const string& str)
      : string(str)
    {}

    ~MyString()
    {}

    string operator()(int bgn, int length)
    {
        return substr(bgn, length);
    }
};
*/

// Code the specified MyString class without inheriting STL string class.
class MyString
{
  public:
    // Default constructor
    MyString(const char* str = nullptr)
      : len_((str != nullptr)? strlen(str) : 0),
        str_(new char[len_ + 1])
    {
        if (len_ > 0)
            strncpy(str_.get(), str, len_);
        str_[len_] = 0;
    }

    // Copy constructor
    MyString(const MyString& that)
      : len_(that.len_),
        str_(new char[len_ + 1])
    {
        if (len_ > 0)
            strncpy(str_.get(), that.str_.get(), len_);
        str_[len_] = 0;
    }

    // Copy assignment operator
    MyString& operator= (const MyString& that)
    {
        if (this == &that)
            return *this;

        char* old = str_.release();
        delete[] old;

        len_ = that.len_;
        str_.reset(new char[len_ + 1]);
        if (len_ > 0)
            strncpy(str_.get(), that.str_.get(), len_);
        str_[len_] = 0;

        return *this;
    }

    // Destructor
    ~MyString()
    {}

    // Overloaded arithmetic "+" operator
    MyString operator+ (const MyString& that)
    {
        size_t len = len_ + that.len_;
        char buf[len + 1];

        size_t ofst = 0;
        if (len_ > 0) {
            ofst = len_;
            strncpy(buf, str_.get(), len_);
        }
        if (that.len_ > 0)
            strncpy(buf + ofst, that.str_.get(), that.len_);
        buf[len] = 0;

        return MyString(buf);
    }

    // Overloaded comparison "<" operator
    bool operator< (const MyString& that)
    {
        if (len_ == 0 && that.len_ == 0)
            return false;
        if (len_ == 0)
            return true;
        if (that.len_ == 0)
            return false;

        int order = strcmp(str_.get(), that.str_.get());
        return (order < 0)? true : false;
    }

    // Overloaded comparison ">" operator
    bool operator> (const MyString& that)
    {
        if (len_ == 0 && that.len_ == 0)
            return false;
        if (len_ == 0)
            return false;
        if (that.len_ == 0)
            return true;

        int order = strcmp(str_.get(), that.str_.get());
        return (order > 0)? true : false;
    }

    // Overloaded comparison "==" operator
    bool operator== (const MyString& that)
    {
        if (len_ == 0 && that.len_ == 0)
            return true;
        if (len_ == 0 || that.len_ == 0)
            return false;

        int order = strcmp(str_.get(), that.str_.get());
        return (order == 0)? true : false;
    }

    // Overloaded addition assignment "+=" operator
    MyString& operator+= (const MyString& that)
    {
        size_t len = len_ + that.len_;
        char* new_buf = new char[len + 1];

        size_t ofst = 0;
        if (len_ > 0) {
            ofst = len_;
            strncpy(new_buf, str_.get(), len_);
        }
        if (that.len_ > 0)
            strncpy(new_buf + ofst, that.str_.get(), that.len_);
        new_buf[len] = 0;

        char* old = str_.release();
        delete[] old;
        str_.reset(new_buf);

        return *this;
    }

    // Overloaded subscripting "[]" operator
    char& operator[] (size_t idx)
    {
        return str_[idx];
    }

    // Overloaded function call "()" operator
    MyString operator() (size_t bgn, size_t len)
    {
        char buf[len + 1];
        strncpy(buf, str_.get() + bgn, len);
        buf[len] = 0;
        return MyString(buf);
    }

    // Other types of overloaded operators.
    friend ostream& operator<< (ostream&, const MyString&);
    friend MyString operator+ (const char*, const MyString&);

  private:
    int len_;
    unique_ptr<char[]> str_;
};


// Overloaded ostream operator
ostream& operator<< (ostream& os, const MyString& obj)
{
    os << obj.str_.get();
    return os;
}

MyString operator+ (const char* lhs, const MyString& rhs)
{
    size_t len_lhs = (lhs != nullptr)? strlen(lhs) : 0;
    size_t len = len_lhs + rhs.len_;
    char buf[len];

    size_t ofst = 0;
    if (len_lhs > 0) {
        ofst = len_lhs;
        strncpy(buf, lhs, len_lhs);
    }
    if (rhs.len_ > 0)
        strncpy(buf + ofst, rhs.str_.get(), rhs.len_);
    buf[len] = 0;

    return MyString(buf);
}


// The following driver code cannot be modified.
int CompareString(const void *e1, const void *e2)
{
    MyString *s1 = (MyString*)e1;
    MyString *s2 = (MyString*)e2;
    if (*s1 < *s2) return -1;
    else if (*s1 == *s2) return 0;
    else if (*s1 > *s2) return 1;
}

int main()
{
    MyString s1("abcd-"), s2, s3("efgh-"), s4(s1);
    MyString SArray[4] = {"big", "me", "about", "take"};
    cout << "1. " << s1 << s2 << s3 << s4 << endl;

    s4 = s3;
    s3 = s1 + s3;
    cout << "2. " << s1 << endl;
    cout << "3. " << s2 << endl;
    cout << "4. " << s3 << endl;
    cout << "5. " << s4 << endl;
    cout << "6. " << s1[2] << endl;

    s2 = s1;
    s1 = "ijkl-";
    s1[2] = 'A' ;
    cout << "7. " << s2 << endl;
    cout << "8. " << s1 << endl;

    s1 += "mnop";
    cout << "9. " << s1 << endl;

    s4 = "qrst-" + s2;
    cout << "10. " << s4 << endl;

    s1 = s2 + s4 + " uvw " + "xyz";
    cout << "11. " << s1 << endl;

    qsort(SArray, 4, sizeof(MyString), CompareString);
    for (int i = 0 ; i < 4 ; ++i)
        cout << SArray[i] << endl;

    cout << s1(0, 4) << endl;
    cout << s1(5, 10) << endl;
    return 0;
}