#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>


// Implement the smart pointer for resource management.
template <typename T>
class auto_array
{
  private:
    T* array_;

  public:
    auto_array(T* array)
      : array_(array)
    {}

    ~auto_array()
    {
        delete[] array_;
    }

    T* operator->() const
    {
        return array_;
    }

    T* get() const
    {
        return array_;
    }

    T& operator[] (std::size_t idx)
    {
        return array_[idx];
    }

    auto_array& operator= (T* array)
    {
        delete[] array_;
        array_ = array;
        return *this;
    }
};
// End

// Implement the class here.
class MyString
{
  private:
    auto_array<char> str_;

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
        strcpy(str_.get(), str);
    }

    MyString(const char* str, std::size_t len)
      : str_(new char[len + 1])
    {
        strncpy(str_.get(), str, len);
        str_[len] = 0;
    }

    MyString(const char* str_src, const char* str_dst)
      : str_(new char[strlen(str_src) + strlen(str_dst) + 1])
    {
        strcpy(str_.get(), str_src);
        std::size_t len = strlen(str_.get());
        strcpy(str_.get() + len, str_dst);
    }

    MyString(const MyString& obj)
      : str_(new char[strlen(obj.str_.get()) + 1])
    {
        strcpy(str_.get(), obj.str_.get());
    }

    ~MyString()
    {}

    // Assignment Related Operators.
    MyString& operator= (const MyString& rhs)
    {
        char* new_str = new char[strlen(rhs.str_.get()) + 1];
        strcpy(new_str, rhs.str_.get());
        str_ = new_str;
        return *this;
    }

    MyString& operator+= (const MyString& rhs)
    {
        std::size_t len = strlen(str_.get());
        char* new_str = new char[len + strlen(rhs.str_.get()) + 1];
        strcpy(new_str, str_.get());
        strcpy(new_str + len, rhs.str_.get());
        str_ = new_str;
        return *this;
    }

    // Binary Arithmetic Related Operators.
    const MyString operator+ (const MyString& rhs)
    {
        return MyString(str_.get(), rhs.str_.get());
    }

    friend const MyString operator+ (const MyString& obj, std::string str)
    {
        return MyString(obj.str_.get(), str.c_str());
    }

    friend const MyString operator+ (std::string str, const MyString& obj)
    {
        return MyString(str.c_str(), obj.str_.get());
    }

    // Comparison Related Operators.
    bool operator< (const MyString& rhs)
    {
        return (strcmp(str_.get(), rhs.str_.get()) < 0)? true : false;
    }

    bool operator== (const MyString& rhs)
    {
        return (strcmp(str_.get(), rhs.str_.get()) == 0)? true : false;
    }

    bool operator> (const MyString& rhs)
    {
        return (strcmp(str_.get(), rhs.str_.get()) > 0)? true : false;
    }

    // I/O stream Related Operators.
    friend std::ostream& operator<< (std::ostream& os, const MyString& obj)
    {
        os << obj.str_.get();
        return os;
    }

    // Indexing Related Operators.
    char& operator[] (int idx)
    {
        return str_[idx];
    }

    const MyString operator() (int idx, std::size_t len)
    {
        return MyString(str_.get() + idx, len);
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
