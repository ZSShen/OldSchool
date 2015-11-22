#include <iostream>
#include <map>
#include <cstdio>

using namespace std;


template <typename T>
class Profile
{
  private:
    map<T, T> map_;

  public:
    Profile()
      : map_()
    {
        map_.insert(pair<T, T>(10000000, 1));
    }

    ~Profile()
    {
        map_.clear();
    }

    T MatchRival(const pair<T, T>& rival);
};


template <typename T>
T Profile<T>::MatchRival(const pair<T, T>& rival)
{
    // Insert the new rival into proper position.
    typename map<T, T>::iterator iter = map_.insert(rival).first;

    // Select the best match with minimum difference power.
    if (iter == map_.begin())
        return (++iter)->second;
    else if (iter == (--map_.end()))
        return (--iter)->second;

    T prev_diff = rival.first - (--iter)->first;
    T prev_id = iter->second;
    ++iter;
    T succ_diff = (++iter)->first - rival.first;
    T succ_id = iter->second;

    return (prev_diff <= succ_diff)? prev_id : succ_id;
}

int main()
{
    Profile<int> profile;

    int loop;
    cin >> loop;
    for (int i = 0 ; i < loop ; ++i) {
        int id, power;
        cin >> id >> power;
        int match = profile.MatchRival(pair<int, int>(power, id));
        cout << id << ' ' << match << endl;
    }
}