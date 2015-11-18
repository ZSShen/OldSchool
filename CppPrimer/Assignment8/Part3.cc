#include <iostream>
#include <map>

using namespace std;
#define BUF_SIZE (64)


template <typename T>
class MultiSet
{
  private:
    map<T, T> map_;

  public:
    MultiSet()
      : map_()
    {}

    ~MultiSet()
    {}

    T Insert(T);
    T Delete(T);
    pair<T, T> Ask(T);
};

template <typename T>
T MultiSet<T>::Insert(T num)
{
    typename map<T, T>::iterator iter = map_.find(num);
    if (iter != map_.end())
        return ++(iter->second);
    map_.insert(pair<T, T>(num, 1));
    return 1;
}

template <typename T>
T MultiSet<T>::Delete(T num)
{
    typename map<T, T>::iterator iter = map_.find(num);
    if (iter == map_.end())
        return 0;
    T times = iter->second;
    iter->second = 0;
    return times;
}

template <typename T>
pair<T, T> MultiSet<T>::Ask(T num)
{
    typename map<T, T>::iterator iter = map_.find(num);
    if (iter == map_.end())
        return pair<T, T>(0, 0);
    return pair<T, T>(1, iter->second);
}


int main()
{
    MultiSet<int> set;
    int loop;
    cin >> loop;

    for (int i = 0 ; i < loop ; ++i) {
        int num;
        string oper;
        cin >> oper >> num;
        if (oper == "add") {
            int times = set.Insert(num);
            cout << times << endl;
        } else if (oper == "del") {
            int times = set.Delete(num);
            cout << times << endl;
        } else if (oper == "ask") {
            const pair<int, int>& state = set.Ask(num);
            cout << state.first << ' ' << state.second << endl;
        }
    }

    return 0;
}