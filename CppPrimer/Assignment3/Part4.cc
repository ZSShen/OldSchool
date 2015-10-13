#include <iostream>
#include <cstring>

using namespace std;


class BigInt {
  private:
    struct Number {
        int m_len;
        char* m_array;
    };

    bool m_sign;
    Number* m_num;
    char* m_rep;

    Number* addNumber(const Number*, const Number*) const;
    Number* subNumber(const Number*, const Number*) const;
    Number* mulNumber(const Number*, const Number*) const;
    bool compareLarger(const Number*, const Number*) const;

  public:
    BigInt(const char*);
    BigInt(bool, Number*);
    ~BigInt();

    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;

    char* getNumber() const { return m_rep; }
};

/*-----------------------------------------------------------------------------*
 *                  Implementation for the internal operators                  *
 *-----------------------------------------------------------------------------*/
BigInt::Number* BigInt::addNumber(const Number* num_src, const Number* num_dst) const
{
    int idx_src = num_src->m_len;
    int idx_dst = num_dst->m_len;
    char* array_src = num_src->m_array;
    char* array_dst = num_dst->m_array;

    int len_res = (idx_src > idx_dst)? (idx_src + 1) : (idx_dst + 1);
    char array_res[len_res];
    memset(array_res, 0, sizeof(char) * (len_res));

    // Add the source and destination arrays until reaching their boundaries.
    int idx_res = len_res - 1;
    --idx_src;
    --idx_dst;
    while ((idx_src >= 0) && (idx_dst >= 0)) {
        array_res[idx_res] += array_src[idx_src] + array_dst[idx_dst];
        if (array_res[idx_res] > 9) {
            array_res[idx_res] -= 10;
            array_res[idx_res - 1] += 1;
        }
        --idx_src;
        --idx_dst;
        --idx_res;
    }

    // Add the remained array if necessary.
    if ((idx_src == -1) && (idx_dst >= 0)) {
        while (idx_dst >= 0) {
            array_res[idx_res] += array_dst[idx_dst];
            if (array_res[idx_res] > 9) {
                array_res[idx_res] -= 10;
                array_res[idx_res - 1] += 1;
            }
            --idx_dst;
            --idx_res;
        }
    } else if ((idx_src >= 0) && (idx_dst == -1)) {
        while (idx_src >= 0) {
            array_res[idx_res] += array_src[idx_src];
            if (array_res[idx_res] > 9) {
                array_res[idx_res] -= 10;
                array_res[idx_res - 1] += 1;
            }
            --idx_src;
            --idx_res;
        }
    }

    Number* num_res = new Number;
    idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);

    return num_res;
}

BigInt::Number* BigInt::subNumber(const Number* num_src, const Number* num_dst) const
{
    int idx_src = num_src->m_len - 1;
    int idx_dst = num_dst->m_len - 1;
    char* array_src = num_src->m_array;
    char* array_dst = num_dst->m_array;

    int len_res = num_src->m_len;
    int idx_res = len_res - 1;
    char array_res[len_res];
    memset(array_res, 0, sizeof(char) * len_res);

    while (idx_dst >= 0)
    {
        array_res[idx_res] += array_src[idx_src] - array_dst[idx_dst];
        if (array_res[idx_res] < 0) {
            array_res[idx_res] += 10;
            array_res[idx_res - 1] -= 1;
        }
        --idx_src;
        --idx_dst;
        --idx_res;
    }
    while (idx_src >= 0) {
        array_res[idx_res] += array_src[idx_src];
        if (array_res[idx_res] < 0) {
            array_res[idx_res] += 10;
            array_res[idx_res - 1] -= 1;
        }
        --idx_src;
        --idx_res;
    }

    Number* num_res = new Number;
    idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);

    return num_res;
}



bool BigInt::compareLarger(const Number* num_src, const Number* num_dst) const
{
    if (num_src->m_len > num_dst->m_len)
        return true;

    if (num_src->m_len < num_dst->m_len)
        return false;

    for (int i = 0 ; i < num_src->m_len ; ++i) {
        if (num_src->m_array[i] < num_dst->m_array[i])
            return false;
    }
    return true;
}


/*-----------------------------------------------------------------------------*
 *           Implementation for the constructors and the destructor            *
 *-----------------------------------------------------------------------------*/
BigInt::BigInt(const char* str_num)
{
    m_num = new Number;
    int len = strlen(str_num);
    m_num->m_len = len;

    if (str_num[0] == '-') {
        // Prepare the internal numeric format.
        --(m_num->m_len);
        int i = 1;
        while ((i < len - 1) && (str_num[i] == '0')) {
            --(m_num->m_len);
            ++i;
        }
        m_num->m_array = new char[m_num->m_len];
        for (int j = 0 ; i < len ; ++i, ++j)
            m_num->m_array[j] = str_num[i] - '0';
        m_sign = true;

        // Prepare the public representation.
        m_rep = new char[m_num->m_len + 2];
        m_rep[0] = '-';
        for (i = 1 ; i <= m_num->m_len ; ++i)
            m_rep[i] = m_num->m_array[i - 1] + '0';
        m_rep[i] = 0;
    } else {
        int i = 0;
        while ((i < len - 1) && (str_num[i] == '0')) {
            --(m_num->m_len);
            ++i;
        }
        m_num->m_array = new char[m_num->m_len];
        for (int j = 0 ; i < len ; ++i, ++j)
            m_num->m_array[j] = str_num[i] - '0';
        m_sign = false;

        m_rep = new char[m_num->m_len + 1];
        for (i = 0 ; i < m_num->m_len ; ++i)
            m_rep[i] = m_num->m_array[i] + '0';
        m_rep[i] = 0;
    }
}

BigInt::BigInt(bool sign, Number* num)
  : m_sign(sign),
    m_num(num)
{
    int len = m_num->m_len;
    if (m_sign) {
        m_rep = new char[len + 2];
        m_rep[0] = '-';
        for (int i = 0 ; i < len ; ++i)
            m_rep[i + 1] = m_num->m_array[i] + '0';
        m_rep[len + 1] = 0;
    } else {
        m_rep = new char[len + 1];
        for (int i = 0 ; i < len ; ++i)
            m_rep[i] = m_num->m_array[i] + '0';
        m_rep[len] = 0;
    }
}

BigInt::~BigInt()
{
    delete[] m_num->m_array;
    delete m_num;
    delete[] m_rep;
}


/*-----------------------------------------------------------------------------*
 *        Implementation for the overloaded binary arithmetic operators        *
 *-----------------------------------------------------------------------------*/
BigInt BigInt::operator+(const BigInt& rhs) const
{
    /* lhs is positive and rhs is positive. */
    if (!m_sign && !rhs.m_sign) {
        Number* num_res = addNumber(m_num, rhs.m_num);
        return BigInt(false, num_res);
    }

    /* lhs is positive and rhs is negative. */
    if (!m_sign && rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            Number* num_res = subNumber(m_num, rhs.m_num);
            return BigInt(false, num_res);
        }
        Number* num_res = subNumber(rhs.m_num, m_num);
        return BigInt(true, num_res);
    }

    /* lhs is negative and rhs is positive. */
    if (m_sign && !rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            Number* num_res = subNumber(m_num, rhs.m_num);
            return BigInt(true, num_res);
        }
        Number* num_res = subNumber(rhs.m_num, m_num);
        return BigInt(false, num_res);
    }

    /* lhs is negative and rhs is positive. */
    Number* num_res = addNumber(m_num, rhs.m_num);
    return BigInt(true, num_res);
}

BigInt BigInt::operator-(const BigInt& rhs) const
{
    /* lhs is positive and rhs is positive. */
    if (!m_sign && !rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            Number* num_res = subNumber(m_num, rhs.m_num);
            return BigInt(false, num_res);
        }
        Number* num_res = subNumber(rhs.m_num, m_num);
        return BigInt(true, num_res);
    }

    /* lhs is positive and rhs is negative. */
    if (!m_sign && rhs.m_sign) {
        Number* num_res = addNumber(m_num, rhs.m_num);
        return BigInt(false, num_res);
    }

    /* lhs is negative and rhs is positive. */
    if (m_sign && !rhs.m_sign) {
        Number* num_res = addNumber(m_num, rhs.m_num);
        return BigInt(true, num_res);
    }

    /* lhs is negative and rhs is negative. */
    if (compareLarger(m_num, rhs.m_num)) {
        Number* num_res = subNumber(m_num, rhs.m_num);
        return BigInt(true, num_res);
    }
    Number* num_res = subNumber(rhs.m_num, m_num);
    return BigInt(false, num_res);
}

BigInt BigInt::operator*(const BigInt& rhs) const
{}

BigInt BigInt::operator/(const BigInt& rhs) const
{}


/* Overload ostream to print BigInt. */
ostream& operator<<(ostream& out, const BigInt& rhs)
{
    out << rhs.getNumber();
}


int main()
{
    string str_op1, str_op2;
    char oper;


#ifdef IN_HOUSE
    while (cin >> str_op1) {
        cin >> oper >> str_op2;
        BigInt bigint_op1(str_op1.c_str());
        BigInt bigint_op2(str_op2.c_str());

        cout << "Opnd_1: " << bigint_op1 << endl;
        cout << "Oper  : " << oper << endl;
        cout << "Opnd_2: " << bigint_op2 << endl;
        switch (oper) {
            case '+':
                cout << "Result: " << bigint_op1 + bigint_op2 << endl << endl;
                break;
            case '-':
                cout << "Result: " << bigint_op1 - bigint_op2 << endl << endl;
                break;
        }
    }

#else
    cin >> str_op1 >> oper >> str_op2;
    BigInt bigint_op1(str_op1.c_str());
    BigInt bigint_op2(str_op2.c_str());

    cout << bigint_op1 << endl;
    cout << bigint_op2 << endl;
    switch (oper) {
        case '+':
            cout << bigint_op1 + bigint_op2 << endl;
            break;
        case '-':
            cout << bigint_op1 - bigint_op2 << endl;
            break;
    }
#endif

    return 0;
}