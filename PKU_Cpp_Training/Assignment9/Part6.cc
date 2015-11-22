#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>


// Implement the class here.
class MyString
{
  private:
    char* str_;

  public:
    // Constructor, Copy Constructor, and Destructor.
    MyString()
      : str_(new char[1])
    {
        str_[0] = 0;
    }

    MyString(const char* str)
      : str_(new char[strlen(str) + 1])
    {
        strcpy(str_, str);
    }

    MyString(const char* str, std::size_t len)
      : str_(new char[len + 1])
    {
        strncpy(str_, str, len);
        str_[len] = 0;
    }

    MyString(const char* str_src, const char* str_dst)
      : str_(new char[strlen(str_src) + strlen(str_dst) + 1])
    {
        strcpy(str_, str_src);
        std::size_t len = strlen(str_);
        strcpy(str_ + len, str_dst);
    }

    MyString(const MyString& obj)
      : str_(new char[strlen(obj.str_) + 1])
    {
        strcpy(str_, obj.str_);
    }

    ~MyString()
    {
        delete[] str_;
    }

    // Assignment Related Operators.
    MyString& operator= (const MyString& rhs)
    {
        char* new_str = new char[strlen(rhs.str_) + 1];
        strcpy(new_str, rhs.str_);
        delete[] str_;
        str_ = new_str;
        return *this;
    }

    MyString& operator+= (const MyString& rhs)
    {
        std::size_t len = strlen(str_);
        char* new_str = new char[len + strlen(rhs.str_) + 1];
        strcpy(new_str, str_);
        strcpy(new_str + len, rhs.str_);
        delete[] str_;
        str_ = new_str;
        return *this;
    }

    // Binary Arithmetic Related Operators.
    const MyString operator+ (const MyString& rhs)
    {
        return MyString(str_, rhs.str_);
    }

    friend const MyString operator+ (const MyString& obj, std::string str)
    {
        return MyString(obj.str_, str.c_str());
    }

    friend const MyString operator+ (std::string str, const MyString& obj)
    {
        return MyString(str.c_str(), obj.str_);
    }

    // Comparison Related Operators.
    bool operator< (const MyString& rhs)
    {
        return (strcmp(str_, rhs.str_) < 0)? true : false;
    }

    bool operator== (const MyString& rhs)
    {
        return (strcmp(str_, rhs.str_) == 0)? true : false;
    }

    bool operator> (const MyString& rhs)
    {
        return (strcmp(str_, rhs.str_) > 0)? true : false;
    }

    // I/O stream Related Operators.
    friend std::ostream& operator<< (std::ostream& os, const MyString& obj)
    {
        os << obj.str_;
        return os;
    }

    // Indexing Related Operators.
    char& operator[] (int idx)
    {
        return str_[idx];
    }

    const MyString operator() (int idx, std::size_t len)
    {
        return MyString(str_ + idx, len);
    }
};

// End

int CompareString(const void* e1, const void* e2)
{
    MyString* s1 = (MyString*) e1;
    MyString* s2 = (MyString*) e2;
    if(*s1 < *s2)
        return -1;
    else if(*s1 == *s2)
        return 0;
    else if(*s1 > *s2 )
        return 1;
}


int main()
{
    MyString s1("abcd-"), s2, s3("efgh-"), s4(s1);
    MyString SArray[4] = {"big", "me", "about", "take"};
    std::cout << "1. " << s1 << s2 << s3 << s4 << std::endl;

    s4 = s3;
    s3 = s1 + s3;
    std::cout << "2. " << s1 << std::endl;
    std::cout << "3. " << s2 << std::endl;
    std::cout << "4. " << s3 << std::endl;
    std::cout << "5. " << s4 << std::endl;
    std::cout << "6. " << s1[2] << std::endl;

    s2 = s1;
    s1 = "ijkl-";
    s1[2] = 'A' ;
    std::cout << "7. " << s2 << std::endl;
    std::cout << "8. " << s1 << std::endl;

    s1 += "mnop";
    std::cout << "9. " << s1 << std::endl;
    s4 = "qrst-" + s2;
    std::cout << "10. " << s4 << std::endl;
    s1 = s2 + s4 + " uvw " + "xyz";
    std::cout << "11. " << s1 << std::endl;

    qsort(SArray, 4, sizeof(MyString), CompareString);
    for (int i = 0 ; i < 4 ; ++i)
        std::cout << SArray[i] << std::endl;
    std::cout << s1(0, 4) << std::endl;
    std::cout << s1(5, 10) << std::endl;

    return 0;
}
