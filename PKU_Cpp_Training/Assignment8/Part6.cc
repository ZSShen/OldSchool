#include <iostream>
#include <list>
#include <cmath>

using namespace std;


template <typename T>
class PriorityQueue
{
  private:
    list<pair<T, T> > elements_;

    bool is_prime(T);

  public:
    PriorityQueue()
      : elements_()
    {}

    ~PriorityQueue()
    {
        elements_.clear();
    }

    void push(T);
    T get_min();
    T get_max();
    void pop_min();
    void pop_max();
    void show_elements();
};


template <typename T>
bool PriorityQueue<T>::is_prime(T num)
{
    T sqrt_root = static_cast<T>(sqrt(static_cast<double>(num)));
    for (T fact = 2 ; fact <= sqrt_root ; ++fact) {
        if (num % fact == 0)
            return false;
    }
    return true;
}

template <typename T>
void PriorityQueue<T>::push(T element)
{
    // Count the primes of a given number.
    T count = 0;
    for (T fact = 2 ; fact <= (element >> 1) ; ++fact) {
        if ((element % fact == 0) && is_prime(fact))
            ++count;
    }

    // Put this new element in the correct priority position.
    typename list<pair<T, T> >::iterator iter = elements_.begin();
    while (iter != elements_.end()) {
        if (count < iter->second)
            break;
        if ((count == iter->second) && (element < iter->first))
            break;
        ++iter;
    }
    elements_.insert(iter, pair<T, T>(element, count));
}

template <typename T>
T PriorityQueue<T>::get_min()
{
    return elements_.front().first;
}

template <typename T>
T PriorityQueue<T>::get_max()
{
    return elements_.back().first;
}

template <typename T>
void PriorityQueue<T>::pop_min()
{
    elements_.pop_front();
}

template <typename T>
void PriorityQueue<T>::pop_max()
{
    elements_.pop_back();
}

template <typename T>
void PriorityQueue<T>::show_elements()
{
    typename list<pair<T, T> >::iterator iter = elements_.begin();
    while (iter != elements_.end()) {
        cout << iter->first << ' ' << iter->second << endl;
        ++iter;
    }
}


int main()
{
    int loop;
    cin >> loop;

    PriorityQueue<int> queue;

    for (int i = 0 ; i < loop ; ++i) {
        for (int j = 0 ; j < 10 ; ++j) {
            int element;
            cin >> element;
            queue.push(element);
        }

        //queue.show_elements();

        // Print and pop the elements with maximum and minimum priority.
        int max = queue.get_max();
        int min = queue.get_min();
        queue.pop_max();
        queue.pop_min();
        cout << max << ' ' << min << endl;
    }
}