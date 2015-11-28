#include <iostream>
#include <cstring>

using namespace std;


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