#include <iostream>
#include <iomanip>

using namespace std;


int main()
{
    int num;

    while (cin >> num) {
        cout << hex << num << endl;
        cout << dec << setfill('0') << setw(10) << num << endl;
    }
}