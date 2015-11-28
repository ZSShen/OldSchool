#include <iostream>
#include <cstring>
#include <cstdlib>

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

    void addNumber(const Number*, const Number*, Number*) const;
    void subNumber(const Number*, const Number*, Number*) const;
    void mulNumber(const Number*, const Number*, Number*) const;
    void divNumber(const Number*, const Number*, Number*) const;
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
void BigInt::addNumber(const Number* num_src, const Number* num_dst,
                       Number* num_res) const
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

    idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);
}

void BigInt::subNumber(const Number* num_src, const Number* num_dst,
                       Number* num_res) const
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

    idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);
}

void BigInt::mulNumber(const Number* num_src, const Number* num_dst,
                       Number* num_res) const
{
    int len_src = num_src->m_len;
    int len_dst = num_dst->m_len;
    char* array_src = num_src->m_array;
    char* array_dst = num_dst->m_array;

    int len_res = (len_src > len_dst)? (len_src << 1) : (len_dst << 1);
    char array_res[len_res];
    memset(array_res, 0, sizeof(char) * len_res);

    for (int i = len_src - 1, k = 1 ; i >= 0 ; --i, ++k) {
        int idx_res = len_res - k;
        for (int j = len_dst - 1 ; j >= 0 ; --j) {
            array_res[idx_res] += array_src[i] * array_dst[j];
            div_t result = div(array_res[idx_res], 10);
            array_res[idx_res] = result.rem;
            array_res[idx_res - 1] += result.quot;
            --idx_res;
        }
    }

    int idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);
}

void BigInt::divNumber(const Number* num_src, const Number* num_dst,
                       Number* num_res) const
{
    // Early return if the dividend is smaller than divisor.
    if (compareLarger(num_dst, num_src)) {
        num_res->m_len = 1;
        num_res->m_array = new char[1];
        num_res->m_array[0] = 0;
        return;
    }

    int len_divr = num_dst->m_len + 1;
    int len_res = num_src->m_len + 1;
    char* array_src = num_src->m_array;

    char array_divr[len_divr];
    char array_divd[len_divr];
    char array_res[len_res];
    array_divr[0] = 0;
    memcpy(array_divr + 1, num_dst->m_array, len_divr - 1);
    array_divd[0] = 0;
    memcpy(array_divd + 1, array_src, len_divr - 1);
    memset(array_res, 0, sizeof(char) * len_res);

    int idx_res = len_divr - 1;
    int idx_divd = num_dst->m_len - 1;
    int iter = len_res - len_divr + 1;
    for (int i = 0 ; i < iter ; ++i) {
        for (int j = 0 ; j < 10 ; ++j) {
            // Subtraction trial
            for (int k = len_divr - 1 ; k > 0 ; --k) {
                array_divd[k] -= array_divr[k];
                if (array_divd[k] < 0) {
                    array_divd[k - 1] -= 1;
                    array_divd[k] += 10;
                }
            }
            // Negative sign scanning
            bool is_neg = false;
            for (int k = 0 ; k < len_divr ; ++k) {
                if (array_divd[k] < 0) {
                    is_neg = true;
                    break;
                }
            }

            if (!is_neg)
                continue;
            // Determine the per-radix quotient and compensate the dividend.
            array_res[idx_res++] = j;
            for (int k = len_divr - 1 ; k > 0 ; --k) {
                array_divd[k] += array_divr[k];
                if (array_divd[k] > 9) {
                    array_divd[k - 1] += 1;
                    array_divd[k] -= 10;
                }
            }
            if (idx_divd < (num_src->m_len - 1)) {
                memmove(array_divd, array_divd + 1, len_divr);
                array_divd[len_divr - 1] = array_src[++idx_divd];
            }
            break;
        }
    }

    idx_res = 0;
    while ((idx_res < len_res - 1) && (array_res[idx_res] == 0))
        ++idx_res;
    num_res->m_len = len_res - idx_res;
    num_res->m_array = new char[num_res->m_len];
    memmove(num_res->m_array, array_res + idx_res, sizeof(char) * num_res->m_len);
}

bool BigInt::compareLarger(const Number* num_src, const Number* num_dst) const
{
    if (num_src->m_len > num_dst->m_len)
        return true;

    if (num_src->m_len < num_dst->m_len)
        return false;

    for (int i = 0 ; i < num_src->m_len ; ++i) {
        if (num_src->m_array[i] > num_dst->m_array[i])
            return true;
    }
    return false;
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
        if ((m_num->m_len == 1) && (m_num->m_array[0] == 0)) {
            m_sign = false;
            m_rep = new char[2];
            m_rep[0] = '0';
            m_rep[1] = 0;
        } else {
            m_rep = new char[m_num->m_len + 2];
            m_rep[0] = '-';
            for (i = 1 ; i <= m_num->m_len ; ++i)
                m_rep[i] = m_num->m_array[i - 1] + '0';
            m_rep[i] = 0;
        }
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
        if ((len == 1) && (num->m_array[0] == 0)) {
            m_sign = false;
            m_rep = new char[2];
            m_rep[0] = '0';
            m_rep[1] = 0;
        } else {
            m_rep = new char[len + 2];
            m_rep[0] = '-';
            for (int i = 0 ; i < len ; ++i)
                m_rep[i + 1] = m_num->m_array[i] + '0';
            m_rep[len + 1] = 0;
        }
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
    Number* num_res = new Number;

    /* lhs is positive and rhs is positive. */
    if (!m_sign && !rhs.m_sign) {
        addNumber(m_num, rhs.m_num, num_res);
        return BigInt(false, num_res);
    }

    /* lhs is positive and rhs is negative. */
    if (!m_sign && rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            subNumber(m_num, rhs.m_num, num_res);
            return BigInt(false, num_res);
        }
        subNumber(rhs.m_num, m_num, num_res);
        return BigInt(true, num_res);
    }

    /* lhs is negative and rhs is positive. */
    if (m_sign && !rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            subNumber(m_num, rhs.m_num, num_res);
            return BigInt(true, num_res);
        }
        subNumber(rhs.m_num, m_num, num_res);
        return BigInt(false, num_res);
    }

    /* lhs is negative and rhs is positive. */
    addNumber(m_num, rhs.m_num, num_res);
    return BigInt(true, num_res);
}

BigInt BigInt::operator-(const BigInt& rhs) const
{
    Number* num_res = new Number;

    /* lhs is positive and rhs is positive. */
    if (!m_sign && !rhs.m_sign) {
        if (compareLarger(m_num, rhs.m_num)) {
            subNumber(m_num, rhs.m_num, num_res);
            return BigInt(false, num_res);
        }
        subNumber(rhs.m_num, m_num, num_res);
        return BigInt(true, num_res);
    }

    /* lhs is positive and rhs is negative. */
    if (!m_sign && rhs.m_sign) {
        addNumber(m_num, rhs.m_num, num_res);
        return BigInt(false, num_res);
    }

    /* lhs is negative and rhs is positive. */
    if (m_sign && !rhs.m_sign) {
        addNumber(m_num, rhs.m_num, num_res);
        return BigInt(true, num_res);
    }

    /* lhs is negative and rhs is negative. */
    if (compareLarger(m_num, rhs.m_num)) {
        subNumber(m_num, rhs.m_num, num_res);
        return BigInt(true, num_res);
    }
    subNumber(rhs.m_num, m_num, num_res);
    return BigInt(false, num_res);
}

BigInt BigInt::operator*(const BigInt& rhs) const
{
    Number* num_res = new Number;

    /* lhs and rhs have the same sign. */
    if (m_sign == rhs.m_sign) {
        mulNumber(m_num, rhs.m_num, num_res);
        return BigInt(false, num_res);
    }

    mulNumber(m_num, rhs.m_num, num_res);
    return BigInt(true, num_res);
}

BigInt BigInt::operator/(const BigInt& rhs) const
{
    Number* num_res = new Number;

    /* lhs and rhs have the sname sign. */
    if (m_sign == rhs.m_sign) {
        divNumber(m_num, rhs.m_num, num_res);
        return BigInt(false, num_res);
    }

    divNumber(m_num, rhs.m_num, num_res);
    return BigInt(true, num_res);
}


/* Overload ostream to print BigInt. */
ostream& operator<<(ostream& out, const BigInt& rhs)
{
    out << rhs.getNumber();
}


int main()
{
    string str_op1, str_op2;
    char oper;

    cin >> str_op1 >> oper >> str_op2;
    BigInt bigint_op1(str_op1.c_str());
    BigInt bigint_op2(str_op2.c_str());

    switch (oper) {
        case '+':
            cout << bigint_op1 + bigint_op2;
            break;
        case '-':
            cout << bigint_op1 - bigint_op2;
            break;
        case '*':
            cout << bigint_op1 * bigint_op2;
            break;
        case '/':
            cout << bigint_op1 / bigint_op2;
            break;
    }

    return 0;
}