#include <iostream>
#include <cstring>

using namespace std;


// Pad the designated "Array2" class.
class Array2 {
  private:
    int m_row, m_col;
    int **m_array;

  public:
    Array2(int row = 0, int col = 0)
      : m_row(row),
        m_col(col)
    {
        if ((row == 0) || (col == 0))
            m_array = NULL;

        m_array = new int*[m_row];
        for (int i = 0 ; i < m_row ; ++i)
            m_array[i] = new int[m_col];
    }

    ~Array2()
    {
        if (m_array) {
            for (int i = 0 ; i < m_row ; ++i)
                delete[] m_array[i];
            delete[] m_array;
        }
    }

    int*& operator[](int row)
    {
        if (!m_array)
            throw "Array is not initialized\n";

        if (!(((0 <= row) && (row < m_row))))
            throw "Invalid array index\n";

        return m_array[row];
    }

    int& operator()(int row, int col)
    {
        if (!m_array)
            throw "Array is not initialized\n";

        if (!(((0 <= row) && (row < m_row)) &&
              ((0 <= col) && (col < m_col))))
            throw "Invalid array index\n";

        return m_array[row][col];
    }

    Array2& operator=(const Array2& rhs)
    {
        this->~Array2();

        this->m_row = rhs.m_row;
        this->m_col = rhs.m_col;
        this->m_array = new int*[m_row];
        for (int i = 0 ; i < m_row ; ++i) {
            this->m_array[i] = new int[m_col];
            for (int j = 0 ; j < m_col ; ++j)
                this->m_array[i][j] = rhs.m_array[i][j];
        }

        return *this;
    }
};


int main()
{
    Array2 a(3,4);
    for (int i = 0 ; i < 3 ; ++i)
        for (int j = 0 ; j < 4 ; j++)
            a[i][j] = i * 4 + j;

    for (int i = 0 ; i < 3 ; ++i) {
        for (int j = 0 ; j < 4 ; ++j)
            cout << a(i,j) << ",";
        cout << endl;
    }
    cout << "next" << endl;

    Array2 b;
    b = a;
    for (int i = 0 ; i < 3 ; ++i) {
        for (int j = 0 ; j < 4 ; ++j)
            cout << b[i][j] << ",";
        cout << endl;
    }

    return 0;
}