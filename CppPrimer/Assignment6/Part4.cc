#include <iostream>
#include <string>
#include <vector>

using namespace std;


class StringFactory
{
  private:
    int m_size;
    vector<string*> m_vec;

  public:
    StringFactory(int size)
      : m_size(size)
    {
        m_vec.reserve(m_size);
    }

    ~StringFactory()
    {
        for (vector<string*>::iterator iter = m_vec.begin() ;
             iter != m_vec.end() ; ++iter)
            delete *iter;
        m_vec.clear();
    }

    string copy(int idx_str, int ofst, int len);
    string add(const string& str_fst, const string& str_snd);
    int find(const string& sub_str, int idx_str);
    int rfind(const string& sub_str, int idx_str);
    int insert(const string& str, int idx_str, int ofst);
    void reset(const string& str, int idx_str);
    void print(int idx_str);
    void printall();

    /*
    copy N X L：取出第N个字符串第X个字符开始的长度为L的字符串。

    add S1 S2：判断S1，S2是否为0-99999之间的整数，若是则将其转化为整数做加法，若不是，则作字符串加法，返回的值为一字符串。

    find S N：在第N个字符串中从左开始找寻S字符串，返回其第一次出现的位置，若没有找到，返回字符串的长度。

    rfind S N：在第N个字符串中从右开始找寻S字符串，返回其第一次出现的位置，若没有找到，返回字符串的长度。

    insert S N X：在第N个字符串的第X个字符位置中插入S字符串。

    reset S N：将第N个字符串变为S。

    print N：打印输出第N个字符串。

    printall：打印输出所有字符串。

    over：结束操作。
    */
};


int main()
{

    return 0;
}