#include <iostream>

using namespace std;


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