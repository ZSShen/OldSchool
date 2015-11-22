#include <iostream>
#include <iomanip>

using namespace std;


int main()
{
    double num;

    while (cin >> num) {
        cout << fixed << setprecision(5) << num << endl;
        cout << fixed << setprecision(7) << scientific << num << endl;
    }

    return 0;
}