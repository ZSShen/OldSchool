#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdio>

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

enum TYPE_WEAPON {
    SWORD,
    BOMB,
    ARROW
};

const char* LABEL_WARRIOR[COUNT_TYPE_WARRIOR] = {
    "dragon",
    "ninja",
    "iceman",
    "lion",
    "wolf"
};


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

    ArrayPtr& operator= (const ArrayPtr& rhs)
    {
        T* new_array = new T[rhs.size_];
        memcpy(new_array, rhs.array_);
        delete[] this->array_;
        this->array_ = new_array;
        this->size_ = rhs.size_;
    }

    T* get()
    {
        return array_;
    }

    ~ArrayPtr()
    {
        delete[] array_;
    }
};


class Warrior
{
  protected:
    int m_id, m_life, m_power;

  public:
    Warrior(int id, int life)
      : m_id(id),
        m_life(life),
        m_power(0)
    {}

    virtual ~Warrior()
    {}

    virtual void printInfo() = 0;
};

class Dragon : public Warrior
{
  private:
    int m_id_weap;
    double m_morale;

  public:
    Dragon(int id, int life_warr, int life_hq)
      : Warrior(id, life_warr)
    {
        m_id_weap = id % 3;
        m_morale = double(life_hq) / double(life_warr);
    }

    ~Dragon()
    {}

    void printInfo()
    {
        cout << "It has a ";
        switch (m_id_weap) {
            case SWORD:
                cout << "sword";
                break;
            case BOMB:
                cout << "bomb";
                break;
            case ARROW:
                cout << "arrow";
        }
        cout << ",and it's morale is " << fixed << setprecision(2) << m_morale << endl;
    }
};

class Ninja : public Warrior
{
  private:
    int m_id_weapfst, m_id_weapsnd;

  public:
    Ninja(int id, int life_warr, int life_hq)
      : Warrior(id, life_warr)
    {
        m_id_weapfst = id % 3;
        m_id_weapsnd = (id + 1) % 3;
    }

    ~Ninja()
    {}

    void printInfo()
    {
        cout << "It has a ";
        switch (m_id_weapfst) {
            case SWORD:
                cout << "sword";
                break;
            case BOMB:
                cout << "bomb";
                break;
            case ARROW:
                cout << "arrow";
        }
        cout << " and a ";
        switch (m_id_weapsnd) {
            case SWORD:
                cout << "sword" << endl;
                break;
            case BOMB:
                cout << "bomb" << endl;
                break;
            case ARROW:
                cout << "arrow" << endl;
        }
    }
};

class Iceman : public Warrior
{
  private:
    int m_id_weap;

  public:
    Iceman(int id, int life_warr, int life_hq)
      : Warrior(id, life_warr)
    {
        m_id_weap = id % 3;
    }

    ~Iceman()
    {}

    void printInfo()
    {
        cout << "It has a ";
        switch (m_id_weap) {
            case SWORD:
                cout << "sword" << endl;
                break;
            case BOMB:
                cout << "bomb" << endl;
                break;
            case ARROW:
                cout << "arrow" << endl;
        }
    }
};

class Lion : public Warrior
{
  private:
    int m_loyalty;

  public:
    Lion(int id, int life_warr, int life_hq)
      : Warrior(id, life_warr)
    {
        m_loyalty = life_hq;
    }

    ~Lion()
    {}

    void printInfo()
    {
        cout << "It's loyalty is " << m_loyalty << endl;
    }
};

class Wolf : public Warrior
{
  public:
    Wolf(int id, int life_warr, int life_hq)
      : Warrior(id, life_warr)
    {}

    ~Wolf()
    {}

    void printInfo()
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

    int getWarriorLife(char type_warr) const { return m_arr_life[type_warr]; }
};


class HeadQuarter
{
  private:
    int m_warr_count, m_gen_idx, m_life, m_tick;
    int m_num_warrior[COUNT_TYPE_WARRIOR];

    ArrayPtr<char> m_label;
    char m_gen_seq[COUNT_TYPE_WARRIOR];

    vector<Warrior*> m_vec_warr;

  public:
    HeadQuarter(const char* label, int life, const char* gen_seq)
      : m_warr_count(0),
        m_gen_idx(-1),
        m_life(life),
        m_tick(0),
        m_label(new char [strlen(label) + 1], strlen(label) + 1),
        m_vec_warr()
    {
        strcpy(m_label.get(), label);

        memcpy(m_gen_seq, gen_seq, COUNT_TYPE_WARRIOR * sizeof(char));

        memset(m_num_warrior, 0, COUNT_TYPE_WARRIOR * sizeof(int));
    }

    ~HeadQuarter()
    {
        for (vector<Warrior*>::iterator iter = m_vec_warr.begin() ;
             iter != m_vec_warr.end() ; ++iter) {
            Warrior *p_warr = *iter;
            delete p_warr;
        }
        m_vec_warr.clear();
    }

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

            cout << setfill('0') << setw(3) << m_tick << ' ' << m_label.get() << ' ' \
            << label_warr << ' ' << m_warr_count << " born with strength " \
            << life_warr << ',' << count_warr << ' ' << label_warr << " in " \
            << m_label.get() << " headquarter" << endl;

            Warrior *p_warr;
            switch (type_warr) {
                case DRAGON:
                    p_warr = new Dragon(m_warr_count, life_warr, m_life);
                    break;
                case NINJA:
                    p_warr = new Ninja(m_warr_count, life_warr, m_life);
                    break;
                case ICEMAN:
                    p_warr = new Iceman(m_warr_count, life_warr, m_life);
                    break;
                case LION:
                    p_warr = new Lion(m_warr_count, life_warr, m_life);
                    break;
                case WOLF:
                    p_warr = new Wolf(m_warr_count, life_warr, m_life);
            }
            p_warr->printInfo();
            m_vec_warr.push_back(p_warr);
        } else
            cout << setfill('0') << setw(3) << m_tick << ' ' << m_label.get() \
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

