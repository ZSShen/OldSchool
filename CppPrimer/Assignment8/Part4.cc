#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;


const int SIZE_BLAH_BUF = 512;


template <typename T>
class Handle
{
  private:
    size_t* count_;
    T* ptr_;

    void Clear()
    {
        if (*count_ == 0)
            return;
        --*count_;
        if (*count_ > 0)
            return;
        delete count_;
        delete ptr_;
    }

  public:
    explicit Handle(T* ptr)
      : count_(new size_t(1)),
        ptr_(ptr)
    {}

    Handle(const Handle& rhs)
      : count_(rhs.count_),
        ptr_(rhs.ptr_)
    {
        ++*count_;
    }

    Handle& operator=(const Handle& rhs)
    {
        if (this == &rhs)
            return *this;
        Clear();
        count_ = rhs.count_;
        ptr_ = rhs.ptr_;
        ++*count_;
    }

    ~Handle()
    {
        Clear();
    }

    T& operator*()
    {
        return *ptr_;
    }

    T* operator->()
    {
        return ptr_;
    }
};


class StringFactory
{
  private:
    int size_;

    string oper_copy;
    string oper_add;
    string oper_find;
    string oper_rfind;
    string oper_insert;
    string oper_reset;
    string oper_print;
    string oper_printall;
    string oper_over;

    vector<Handle<string> > vec_;

    string GetString();
    int GetInteger();

    string OperatorCopy();
    string OperatorAdd();
    int OperatorFind();
    int OperatorRFind();
    void OperatorInsert();
    void OperatorReset();
    void OperatorPrint();
    void OperatorPrintAll();

  public:
    StringFactory(int size)
      : size_(size),
        oper_copy("copy"),
        oper_add("add"),
        oper_find("find"),
        oper_rfind("rfind"),
        oper_insert("insert"),
        oper_reset("reset"),
        oper_print("print"),
        oper_printall("printall"),
        oper_over("over"),
        vec_()
    {}

    ~StringFactory()
    {}

    void Init();
    void Run();
};

string StringFactory::GetString()
{
    string str;
    cin >> str;

    if (str == oper_copy)
        return OperatorCopy();
    if (str == oper_add)
        return OperatorAdd();
    return str;
}

int StringFactory::GetInteger()
{
    string str;
    cin >> str;

    if (str == oper_find)
        return OperatorFind();
    if (str == oper_rfind)
        return OperatorRFind();
    return atoi(str.c_str());
}

string StringFactory::OperatorCopy()
{
    int n = GetInteger();
    int x = GetInteger();
    int l = GetInteger();
    return vec_[n - 1]->substr(x, l);
}

string StringFactory::OperatorAdd()
{
    string s1 = GetString();
    string s2 = GetString();

    if (s1.length() > 5)
        return s1 + s2;
    if (s2.length() > 5)
        return s1 + s2;

    const char* cs1 = s1.c_str();
    for (int i = 0 ; i < s1.length() ; ++i) {
        if (!(cs1[i] >= '0' && cs1[i] <= '9'))
            return s1 + s2;
    }
    const char* cs2 = s2.c_str();
    for (int i = 0 ; i < s2.length() ; ++i) {
        if (!(cs2[i] >= '0' && cs2[i] <= '9'))
            return s1 + s2;
    }

    int int1 = atoi(cs1);
    int int2 = atoi(cs2);
    char sum[SIZE_BLAH_BUF];
    sprintf(sum, "%d", int1 + int2);
    return string(sum);
}

int StringFactory::OperatorFind()
{
    string s = GetString();
    int n = GetInteger();

    int res = vec_[n - 1]->find(s);
    return (res == -1)? vec_[n - 1]->length() : res;
}

int StringFactory::OperatorRFind()
{
    string s = GetString();
    int n = GetInteger();

    int res = vec_[n - 1]->rfind(s);
    return (res == -1)? vec_[n - 1]->length() : res;
}

void StringFactory::OperatorInsert()
{
    string s = GetString();
    int n = GetInteger();
    int x = GetInteger();

    vec_[n - 1]->insert(x, s);
}

void StringFactory::OperatorReset()
{
    string s = GetString();
    int n = GetInteger();

    *vec_[n - 1] = s;
}

void StringFactory::OperatorPrint()
{
    int n = GetInteger();
    cout << *vec_[n - 1] << endl;
}

void StringFactory::OperatorPrintAll()
{
    vector<Handle<string> >::iterator iter = vec_.begin();
    while (iter != vec_.end()) {
        cout << **iter << endl;
        ++iter;
    }
}

void StringFactory::Init()
{
    for (int i = 0 ; i < size_ ; ++i) {
        string in;
        cin >> in;
        vec_.push_back(Handle<string>(new string(in)));
    }
}

void StringFactory::Run()
{
    while (true) {
        string cmd;
        cin >> cmd;
        if (cmd == oper_over)
            break;
        if (cmd == oper_copy) {
            OperatorCopy();
            continue;
        }
        if (cmd == oper_add) {
            OperatorAdd();
            continue;
        }
        if (cmd == oper_find) {
            OperatorFind();
            continue;
        }
        if (cmd == oper_rfind) {
            OperatorRFind();
            continue;
        }
        if (cmd == oper_insert) {
            OperatorInsert();
            continue;
        }
        if (cmd == oper_reset) {
            OperatorReset();
            continue;
        }
        if (cmd == oper_print) {
            OperatorPrint();
            continue;
        }
        if (cmd == oper_printall)
            OperatorPrintAll();
    }
}

int main()
{
    int size;
    cin >> size;

    StringFactory factory(size);
    factory.Init();
    factory.Run();

    return 0;
}