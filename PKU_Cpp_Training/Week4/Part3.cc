#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

/*
// Implement the smart pointer for class "Array2". The underlying resource should
// be duplicated if the assignment operator is called.
template <typename T>
class ArrayPtr
{
  private:
    std::size_t size_;
    T* array_;

  public:
    ArrayPtr(T* array, std::size_t size)
      : size_(size),
        array_(array)
    {}

    ~ArrayPtr()
    {
        delete[] array_;
    }

    T& operator[] (T idx)
    {
        return array_[idx];
    }

    ArrayPtr& operator= (const ArrayPtr& rhs)
    {
        T* new_array = new T[rhs.size_];
        for (T i = 0 ; i < rhs.size_ ; ++i)
            new_array[i] = rhs.array_[i];
        delete[] this->array_;
        this->size_ = rhs.size_;
        this->array_ = new_array;
    }
};

// Implement the designated "Array2" class.
class Array2
{
  private:
    int row_, col_;
    ArrayPtr<int> array_;

  public:
    Array2(int row = 0, int col = 0)
      : row_(row),
        col_(col),
        array_(new int[row * col], row * col)
    {}

    ~Array2()
    {}

    // The array indexing utility.
    class SecondIndexer
    {
      private:
        int base_, bound_;
        ArrayPtr<int>& array_;

      public:
        SecondIndexer(ArrayPtr<int>& array, int base, int bound)
          : base_(base),
            bound_(bound),
            array_(array)
        {}

        int& operator[](int ofst)
        {
            if (!((0 <= ofst) && (ofst < bound_)))
                throw "Invalid column index\n";
            return array_[base_ + ofst];
        }
    };

    SecondIndexer operator[](int row)
    {
        if (!((0 <= row) && (row < row_)))
            throw "Invalid row index\n";
        return SecondIndexer(array_, row * col_, col_);
    }

    int& operator()(int row, int col)
    {
        if (!((0 <= row) && (row < row_)))
            throw "Invalid row index\n";
        if (!((0 <= col) && (col < col_)))
            throw "Invalid column index\n";
        return array_[row * col_ + col];
    }

    Array2& operator=(const Array2& rhs)
    {
        this->array_ = rhs.array_;
        this->row_ = rhs.row_;
        this->col_ = rhs.col_;
    }
};
*/

// An alternative way to implement Array2.
class ColumnIndexer
{
  public:
    ColumnIndexer(vector<int>& line)
      : bound_(line.size()),
        line_(line)
    {}

    int& operator[] (int col)
    {
        if (col >= bound_)
            throw "Column index out of bound.";
        return line_[col];
    }

  private:
    int bound_;
    vector<int>& line_;
};

class Array2
{
  public:
    // Default constructor
    Array2(int row = 0, int col = 0)
      : row_(row),
        col_(col),
        matrix_(row)
    {
        for (int r = 0 ; r < row ; ++r)
            matrix_[r].resize(col);
    }

    ~Array2()
    {}

    // Overloaded copy assignment "=" operator
    Array2& operator= (const Array2& that)
    {
        row_ = that.row_;
        col_ = that.col_;

        matrix_.resize(row_);
        for (int r = 0 ; r < row_ ; ++r) {
            matrix_[r].resize(col_);
            for (int c = 0 ; c < col_ ; ++c)
                matrix_[r][c] = that.matrix_[r][c];
        }
    }

    // Overloaded subscripting "[]" operator
    ColumnIndexer operator[] (int row)
    {
        if (row >= row_)
            throw "Row index out of bound.";
        return ColumnIndexer(matrix_[row]);
    }

    // Overloaded function call "()" operator
    int& operator() (int row, int col)
    {
        if (row >= row_)
            throw "Row index out of bound.";
        if (col >= col_)
            throw "Column index out of bound.";
        return matrix_[row][col];
    }

  private:
    int row_;
    int col_;
    vector<vector<int>> matrix_;
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