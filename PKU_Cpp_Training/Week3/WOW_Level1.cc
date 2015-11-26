#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;


#define DEFAULT_VECTOR_SIZE   (0x100)

enum TYPE_WARRIOR {
    DRAGON,
    NINJA,
    ICEMAN,
    LION,
    WOLF,
    COUNT_TYPE_WARRIOR
};

const char* LABEL_WARRIOR[COUNT_TYPE_WARRIOR] = {
    "dragon",
    "ninja",
    "iceman",
    "lion",
    "wolf"
};


// Implement the smart pointer.
template <typename T>
class ArrayPtr
{
  private:
    T* arr_;

  public:
    ArrayPtr(T* arr)
      : arr_(arr)
    {}

    ~ArrayPtr()
    {
        delete[] arr_;
    }

    T* Get()
    {
        return arr_;
    }

    ArrayPtr& operator= (T* arr)
    {
        delete[] arr_;
        arr_ = arr;
    }

    friend ostream& operator<< (ostream& os, const ArrayPtr& rhs)
    {
        os << rhs.arr_;
        return os;
    }
};


class Warrior
{
  private:
    int m_id, m_life, m_power;

  public:
    Warrior(int id, int life)
      : m_id(id),
        m_life(life),
        m_power(0)
    {}
};

class Dragon : public Warrior
{
  public:
    Dragon(int id, int life)
      : Warrior(id, life)
    {}
};

class Ninja : public Warrior
{
  public:
    Ninja(int id, int life)
      : Warrior(id, life)
    {}
};

class Iceman : public Warrior
{
  public:
    Iceman(int id, int life)
      : Warrior(id, life)
    {}
};

class Lion : public Warrior
{
  public:
    Lion(int id, int life)
      : Warrior(id, life)
    {}
};

class Wolf : public Warrior
{
  public:
    Wolf(int id, int life)
      : Warrior(id, life)
    {}
};


class GeneratorMap
{
  private:
    int m_arr_life[COUNT_TYPE_WARRIOR];

  public:
    GeneratorMap(int dragon, int ninja, int iceman, int lion, int wolf)
    {
        m_arr_life[DRAGON] = dragon;
        m_arr_life[NINJA] = ninja;
        m_arr_life[ICEMAN] = iceman;
        m_arr_life[LION] = lion;
        m_arr_life[WOLF] =wolf;
    }

    int getWarriorLife(char type_warr) const
    {
        return m_arr_life[type_warr];
    }
};


class HeadQuarter
{
  private:
    int m_warr_count, m_gen_idx, m_life, m_tick;
    int m_num_warrior[COUNT_TYPE_WARRIOR];

    ArrayPtr<char> m_label;
    char m_gen_seq[COUNT_TYPE_WARRIOR];

  public:
    HeadQuarter(const char* label, int life, const char* gen_seq)
      : m_warr_count(0),
        m_gen_idx(-1),
        m_life(life),
        m_tick(0),
        m_label(new char[strlen(label) + 1])
    {
        strcpy(m_label.Get(), label);
        memcpy(m_gen_seq, gen_seq, COUNT_TYPE_WARRIOR * sizeof(char));
        memset(m_num_warrior, 0, COUNT_TYPE_WARRIOR * sizeof(int));
    }

    ~HeadQuarter()
    {}

    bool generateWarrior(const GeneratorMap& gen_map)
    {
        /* Determine the warrior type. */
        bool found = false;
        char type_warr;
        int life_warr;
        int idx_next = m_gen_idx;
        for (int iter = 0 ; iter < COUNT_TYPE_WARRIOR ; ++iter) {
            ++idx_next;
            if (idx_next == COUNT_TYPE_WARRIOR)
                idx_next = 0;

            type_warr = m_gen_seq[idx_next];
            life_warr = gen_map.getWarriorLife(type_warr);
            if (life_warr <= m_life) {
                m_life -= life_warr;
                m_gen_idx = idx_next;
                ++m_warr_count;
                found = true;
                break;
            }
        }

        /* Update the generation message. */
        if (found) {
            const char* label_warr = LABEL_WARRIOR[type_warr];
            int count_warr = ++m_num_warrior[type_warr];

            cout << setfill('0') << setw(3) << m_tick << ' ' << m_label << ' ' \
            << label_warr << ' ' << m_warr_count << " born with strength " \
            << life_warr << ',' << count_warr << ' ' << label_warr << " in " \
            << m_label << " headquarter" << endl;
        } else
            cout << setfill('0') << setw(3) << m_tick << ' ' << m_label \
            << " headquarter stops making warriors" << endl;

        ++m_tick;
        return found;
    }
};


int main()
{
    char gen_seq_red[COUNT_TYPE_WARRIOR] = {ICEMAN, LION, WOLF, NINJA, DRAGON};
    char gen_seq_blue[COUNT_TYPE_WARRIOR] = {LION, DRAGON, NINJA, ICEMAN, WOLF};

    int case_count;
    cin >> case_count;

    for (int case_idx = 1 ; case_idx <= case_count ; ++case_idx) {
        cout << "Case:" << case_idx << endl;

        int life_hq;
        cin >> life_hq;

        int dragon, ninja, iceman, lion, wolf;
        cin >> dragon >> ninja >> iceman >> lion >> wolf;
        //cout << life_hq << ' ' << dragon << ' ' << wolf << ' ' << endl;

        GeneratorMap gen_map(dragon, ninja, iceman, lion, wolf);
        HeadQuarter hq_red("red", life_hq, gen_seq_red);
        HeadQuarter hq_blue("blue", life_hq, gen_seq_blue);

        bool cont_red = true, cont_blue = true;
        while (cont_red || cont_blue) {
            if (cont_red)
                cont_red = hq_red.generateWarrior(gen_map);
            if (cont_blue)
                cont_blue = hq_blue.generateWarrior(gen_map);
        }
    }

    return 0;
}

