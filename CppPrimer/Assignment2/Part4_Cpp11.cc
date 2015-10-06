#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

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

string LABEL_WARRIOR[COUNT_TYPE_WARRIOR] = {
    "dragon",
    "ninja",
    "iceman",
    "lion",
    "wolf"
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
    vector<int> m_vec_life;

  public:
    GeneratorMap(int dragon, int ninja, int iceman, int lion, int wolf)
    {
        m_vec_life.push_back(dragon);
        m_vec_life.push_back(ninja);
        m_vec_life.push_back(iceman);
        m_vec_life.push_back(lion);
        m_vec_life.push_back(wolf);
    }

    int getWarriorLife(char type_warr) const { return m_vec_life[type_warr]; }
};


class HeadQuarter
{
  private:
    int m_warr_count, m_gen_idx, m_life, m_tick;

    string m_label;

    vector<char> m_gen_seq;
    vector<Dragon> m_vec_dragon;
    vector<Ninja> m_vec_ninja;
    vector<Iceman> m_vec_iceman;
    vector<Lion> m_vec_lion;
    vector<Wolf> m_vec_wolf;

  public:
    HeadQuarter(const string& label, int life, const vector<char>& gen_seq)
      : m_warr_count(0),
        m_gen_idx(-1),
        m_life(life),
        m_tick(0),
        m_label(label),
        m_gen_seq(gen_seq)
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

        /* Store the newly generated warrior. */
        if (found) {
            const string& label_warr = LABEL_WARRIOR[type_warr];
            int count_warr;
            switch (type_warr) {
                case DRAGON:
                    m_vec_dragon.push_back(Dragon(m_warr_count, life_warr));
                    count_warr = m_vec_dragon.size();
                    break;
                case NINJA:
                    m_vec_ninja.push_back(Ninja(m_warr_count, life_warr));
                    count_warr = m_vec_ninja.size();
                    break;
                case ICEMAN:
                    m_vec_iceman.push_back(Iceman(m_warr_count, life_warr));
                    count_warr = m_vec_iceman.size();
                    break;
                case LION:
                    m_vec_lion.push_back(Lion(m_warr_count, life_warr));
                    count_warr = m_vec_lion.size();
                    break;
                case WOLF:
                    m_vec_wolf.push_back(Wolf(m_warr_count, life_warr));
                    count_warr = m_vec_wolf.size();
            }

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
    vector<char> gen_seq_red {ICEMAN, LION, WOLF, NINJA, DRAGON};
    vector<char> gen_seq_blue {LION, DRAGON, NINJA, ICEMAN, WOLF};

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

