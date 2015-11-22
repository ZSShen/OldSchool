#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;


template <typename T>
class Profile
{
  private:
    int pivot_;
    vector<pair<T, T> > vec_;

    enum {
        DIRECTION_LEFT,
        DIRECTION_RGHT
    };

  public:
    Profile()
      : pivot_(0),
        vec_()
    {
        vec_.push_back(pair<T, T>(1, 10000000));
    }

    ~Profile()
    {
        vec_.clear();
    }

    T MatchRival(const pair<T, T>& rival);
    void InsertRival(const pair<T, T>& rival);
};

template <typename T>
void Profile<T>::InsertRival(const pair<T, T>& rival)
{
    vec_.insert(vec_.begin() + pivot_, rival);
}

template <typename T>
T Profile<T>::MatchRival(const pair<T, T>& rival)
{
    // Binary search to quickly locate the proper position of the given rival.
    char direct;
    T left = 0, rght = vec_.size() - 1, power = rival.second;
    T mid, pivot;
    while (left < rght) {
        mid = (left + rght) >> 1;
        if (power > vec_.at(mid).second) {
            left = mid + 1;
            pivot = rght;
            direct = DIRECTION_RGHT;
        }
        else {
            rght = mid - 1;
            pivot = left;
            direct = DIRECTION_LEFT;
        }
    }

    // The rival should select the candidate with minimal power difference as
    // the best match.
    T candidate;
    if (direct == DIRECTION_LEFT) {
        if (power <= vec_.at(pivot).second) {
            if (pivot == 0)
                candidate = pivot;
            else {
                if ((power - vec_.at(pivot - 1).second) <=
                    (vec_.at(pivot).second - power))
                    candidate = pivot - 1;
                else
                    candidate = pivot;
            }
            pivot_ = pivot;
        } else {
            if ((power - vec_.at(pivot).second) <=
                (vec_.at(pivot + 1).second - power))
                candidate = pivot;
            else
                candidate = pivot + 1;
            pivot_ = pivot + 1;
        }
    } else {
        if (power >= vec_.at(pivot).second) {
            if (pivot == (vec_.size() - 1))
                candidate = pivot;
            else {
                if ((power - vec_.at(pivot).second) <=
                    (vec_.at(pivot + 1).second - power))
                    candidate = pivot;
                else
                    candidate = pivot + 1;
            }
            pivot_ = pivot + 1;
        } else {
            if ((power - vec_.at(pivot - 1).second) <=
                (vec_.at(pivot).second - power))
                candidate = pivot - 1;
            else
                candidate = pivot;
            pivot_ = pivot;
        }
    }

    return vec_.at(candidate).first;
}

int main()
{
    Profile<int> profile;

    int loop;
    cin >> loop;

    int id, power;
    cin >> id >> power;
    profile.InsertRival(pair<int, int>(id, power));
    cout << id << " 1" << endl;

    for (int i = 1 ; i < loop ; ++i) {
        cin >> id >> power;
        pair<int, int> rival(id, power);

        int candidate = profile.MatchRival(rival);
        cout << id << ' ' << candidate << endl;

        profile.InsertRival(rival);
    }
}