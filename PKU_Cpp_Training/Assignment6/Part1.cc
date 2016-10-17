#include <iostream>
#include <vector>

using namespace std;


/*
// Pad the designated template class.
template <class T>
class CArray3D {
  private:
    int m_dim_fst, m_dim_snd, m_dim_trd;
    T*** m_cube;

  public:
    // The array indexing utility.
    class ThirdIndexer {
      private:
        T* m_line;

      public:
        ThirdIndexer(T* line)
          : m_line(line)
        {}

        T& operator[](int dim_trd);
    };

    class SecondIndexer {
      private:
        T** m_rec;

      public:
        SecondIndexer(T** rec)
          : m_rec(rec)
        {}

        ThirdIndexer operator[](int dim_snd);
    };

    CArray3D(int dim_fst, int dim_snd, int dim_trd);
    ~CArray3D();
    SecondIndexer operator[](int dim_fst);
};

// The constructor
template <class T>
CArray3D<T>::CArray3D(int dim_fst, int dim_snd, int dim_trd)
  : m_dim_fst(dim_fst),
    m_dim_snd(dim_snd),
    m_dim_trd(dim_trd)
{
    m_cube = new T** [dim_fst];
    for (int i = 0 ; i < dim_fst ; ++i) {
        m_cube[i] = new T* [dim_snd];
        for (int j = 0 ; j < dim_snd ; ++j)
            m_cube[i][j] = new T [dim_trd];
    }
}

// The destructor
template <class T>
CArray3D<T>::~CArray3D()
{
    for (int i = 0 ; i < m_dim_fst ; ++i) {
        for (int j = 0 ; j < m_dim_snd ; ++j)
            delete[] m_cube[i][j];
        delete[] m_cube[i];
    }
    delete[] m_cube;
}

// Implementation for array indexer.
template <class T>
class CArray3D<T>::SecondIndexer CArray3D<T>::operator[] (int dim_fst)
{ return SecondIndexer(m_cube[dim_fst]); }

template <class T>
class CArray3D<T>::ThirdIndexer CArray3D<T>::SecondIndexer::operator[] (int dim_snd)
{ return CArray3D<T>::ThirdIndexer(m_rec[dim_snd]); }

template <class T>
T& CArray3D<T>::ThirdIndexer::operator[] (int dim_trd)
{ return m_line[dim_trd]; }
*/


// An alternative way to implement CArray3D.
template <class T>
class CArray3D
{
    // Forward declaration of indexer template classes.
  private:
    template <class T1> class RectangleIndex;
    template <class T2> class LineIndex;

  public:
    // The default constructor.
    CArray3D(int length, int width, int height)
      : length_(length),
        width_(width),
        height_(height),
        cube_(length)
    {
        for (int i = 0 ; i < length ; ++i) {
            cube_[i].resize(width);
            for (int j = 0 ; j < width ; ++j)
                cube_[i][j].resize(height);
        }
    }

    // The copy constructor.
    CArray3D(const CArray3D& rhs)
    {
        int new_length = rhs.length;
        int new_width = rhs.width;
        int new_height = rhs.height;
        auto& new_cube = rhs.cube_;

        cube_.resize(new_length);
        for (int i = 0 ; i < new_length ; ++i) {
            cube_[i].resize(new_width);
            for (int j = 0 ; j < new_width ; ++j) {
                cube_[i][j].resize(new_height);
                for (int k = 0 ; k < new_height ; ++k)
                    cube_[i][j][k] = new_cube[i][j][k];
            }
        }
    }

    // The copy assignment operator.
    CArray3D& operator= (const CArray3D& rhs)
    {
        int new_length = rhs.length;
        int new_width = rhs.width;
        int new_height = rhs.height;
        auto& new_cube = rhs.cube_;

        cube_.resize(new_length);
        for (int i = 0 ; i < new_length ; ++i) {
            cube_[i].resize(new_width);
            for (int j = 0 ; j < new_width ; ++j) {
                cube_[i][j].resize(new_height);
                for (int k = 0 ; k < new_height ; ++k)
                    cube_[i][j][k] = new_cube[i][j][k];
            }
        }

        return *this;
    }

    // The default destructor.
    ~CArray3D()
    {}

    // Overloaded subscripting "[]" operator.
    RectangleIndex<T> operator[] (int length)
    {
        if (length >= length_)
            throw "Illegal first dimension.\n";
        return RectangleIndex<T>(cube_[length].size(), cube_[length]);
    }

  private:

    // The second layer indexer.
    template <class T1>
    class RectangleIndex
    {
      public:
        RectangleIndex(int width, vector<vector<T1>>& rectangle)
          : width_(width),
            rectangle_(rectangle)
        {}

        ~RectangleIndex()
        {}

        // Overloaded subscripting "[]" operator.
        LineIndex<T1> operator[] (int width)
        {
            if (width >= width_)
                throw "Illegal second dimension.\n";
            return LineIndex<T1>(rectangle_[width].size(), rectangle_[width]);
        }

      private:
        int width_;
        vector<vector<T1>>& rectangle_;
    };

    // The third layer indexer.
    template <class T2>
    class LineIndex
    {
      public:
        LineIndex(int height, vector<T2>& line)
          : height_(height),
            line_(line)
        {}

        ~LineIndex()
        {}

        // Overloaded subscripting "[]" operator.
        T2& operator[] (int height)
        {
            if (height >= height_)
                throw "Illegal third dimension.\n";
            return line_[height];
        }

      private:
        int height_;
        vector<T2>& line_;
    };

    int length_;
    int width_;
    int height_;
    vector<vector<vector<T>>> cube_;
};


int main()
{
    CArray3D<int> a(3, 4, 5);

    int No = 0;
    for (int i = 0 ; i < 3 ; ++i)
        for (int j = 0 ; j < 4 ; ++j)
            for (int k = 0 ; k < 5 ; ++k)
                a[i][j][k] = No++;

    for (int i = 0 ; i < 3 ; ++i)
        for (int j = 0 ; j < 4 ; ++j)
            for (int k = 0 ; k < 5 ; ++k)
                cout << a[i][j][k] << ",";

    return 0;
}