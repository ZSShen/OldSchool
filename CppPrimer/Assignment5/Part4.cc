#include <iostream>
#include <sstream>
#include <vector>

using namespace std;


enum TYPE_WEAPON
{
    SWORD,
    BOMB,
    ARROW,
    TOTAL_TYPE_WEAPON
};

enum TYPE_WARRIOR {
    DRAGON,
    NINJA,
    ICEMAN,
    LION,
    WOLF,
    TOTAL_TYPE_WARRIOR
};

enum FLAG {
    NONE,
    BLUE,
    RED
};

/*-----------------------------------------------------------------------------*
 *                              Weapon Definition                              *
 *-----------------------------------------------------------------------------*/
class Weapon
{
  protected:
    bool useable_;
    double power_;

  public:
    Weapon(bool useable, double power)
      : useable_(useable),
        power_(power)
    {}

    virtual void Age() = 0;

    bool IsUseable()
    {
        return useable_;
    }
    double GetPower()
    {
        return power_;
    }
};

class Sword : public Weapon
{
  public:
    Sword(double power)
      : Weapon((power != 0), power)
    {}

    void Age()
    {
        power_ *= 0.8;
        if (power_ == 0)
            useable_ = false;
    }
};

class Bomb : public Weapon
{
  public:
    Bomb()
      : Weapon(true, 0)
    {}

    void Age()
    {}
};

class Arrow : public Weapon
{
  private:
    int times_;

  public:
    Arrow(double power)
      : Weapon((power != 0), power),
        times_(3)
    {}

    void Age()
    {
        if (times_ > 0)
            --times_;
        if (times_ == 0)
            useable_ = false;
    }
};


/*-----------------------------------------------------------------------------*
 *                              Warrior Definition                             *
 *-----------------------------------------------------------------------------*/
class Warrior
{
  protected:
    int id_, life_, power_;

  public:
    Warrior(int id, int life, int power)
      : id_(id),
        life_(life),
        power_(power)
    {}
    virtual ~Warrior()
    {}
    virtual void Attack(Warrior *p_warr) = 0;
    virtual void Wound(Warrior *p_warr, int atk_point) = 0;
    virtual void FightBack(int atk_point) = 0;

    bool IsAlive()
    {
        return (life_ > 0)? true : false;
    }
};

class Dragon : public Warrior
{
  private:
    bool has_sword_;
    double morale_;
    Weapon *p_weapon_;

  public:
    Dragon(int id, int life, int power_me, double morale, int power_arrow)
      : Warrior(id, life, power_me),
        has_sword_(false),
        morale_(morale),
        p_weapon_(NULL)
    {
        // Take the weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_weapon_ = new Sword(static_cast<double>(power_me) * 0.2);
            has_sword_ = true;
            break;
          case BOMB:
            p_weapon_ = new Bomb();
            break;
          case ARROW:
            p_weapon_ = new Arrow(static_cast<double>(power_arrow));
            break;
        }
    }

    ~Dragon()
    {
        delete p_weapon_;
    }

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->Wound(this, atk_point);

        // Prepare dragon yelling.
        if (p_warr->IsAlive())
            morale_ -= 0.2;
        else
            morale_ += 0.2;
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;

        atk_point = power_ >> 1;
        if (has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->FightBack(atk_point);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }
};

class Ninja : public Warrior
{
  private:
    bool has_sword_;
    Weapon *p_weapon_fst_, *p_weapon_snd_;

  public:
    Ninja(int id, int life, int power_me, int power_arrow)
      : Warrior(id, life, power_me),
        has_sword_(false),
        p_weapon_fst_(NULL),
        p_weapon_snd_(NULL)
    {
        // Take the first weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_weapon_fst_ = new Sword(static_cast<double>(power_me) * 0.2);
            has_sword_ = true;
            break;
          case BOMB:
            p_weapon_fst_ = new Bomb();
            break;
          case ARROW:
            p_weapon_fst_ = new Arrow(static_cast<double>(power_arrow));
            break;
        }

        // Take the second weapon.
        switch ((id + 1) % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_weapon_snd_ = new Sword(static_cast<double>(power_me) * 0.2);
            has_sword_ = true;
            break;
          case BOMB:
            p_weapon_snd_ = new Bomb();
            break;
          case ARROW:
            p_weapon_snd_ = new Arrow(static_cast<double>(power_arrow));
            break;
        }
    }

    ~Ninja()
    {
        delete p_weapon_fst_;
        delete p_weapon_snd_;
    }

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (has_sword_) {
            if (p_weapon_fst_->IsUseable()) {
                atk_point += static_cast<int>(p_weapon_fst_->GetPower());
                p_weapon_fst_->Age();
            }
            if (p_weapon_snd_->IsUseable()) {
                atk_point += static_cast<int>(p_weapon_snd_->GetPower());
                p_weapon_snd_->Age();
            }
        }

        p_warr->Wound(this, atk_point);
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }
};

class Iceman : public Warrior
{
  private:
    bool has_sword_;
    Weapon *p_weapon_;

  public:
    Iceman(int id, int life, int power_me, int power_arrow)
      : Warrior(id, life, power_me),
        has_sword_(false)
    {
        // Take the weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_weapon_ = new Sword(static_cast<double>(power_me) * 0.2);
            break;
          case BOMB:
            p_weapon_ = new Bomb();
            break;
          case ARROW:
            p_weapon_ = new Arrow(static_cast<double>(power_arrow));
            break;
        }
    }

    ~Iceman()
    {
        delete p_weapon_;
    }

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->Wound(this, atk_point);
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;

        atk_point = power_ >> 1;
        if (has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->FightBack(atk_point);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }
};

class Lion : public Warrior
{
  private:
    int loyalty_, degrade_;

  public:
    Lion(int id, int life, int power, int loyalty, int degrade)
      : Warrior(id, life, power),
        loyalty_(loyalty),
        degrade_(degrade)
    {}

    void Attack(Warrior *p_warr)
    {
        p_warr->Wound(this, power_);
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;
        p_warr->FightBack(power_ >> 1);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }
};

class Wolf : public Warrior
{
  private:
    bool has_sword_;
    Weapon *p_weapon_;

  public:
    Wolf(int id, int life, int power)
      : Warrior(id, life, power),
        has_sword_(false),
        p_weapon_(NULL)
    {}

    ~Wolf()
    {
        if (p_weapon_)
            delete p_weapon_;
    }

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (p_weapon_ && has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->Wound(this, atk_point);
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;

        atk_point = power_ >> 1;
        if (p_weapon_ && has_sword_ && p_weapon_->IsUseable()) {
            atk_point += static_cast<int>(p_weapon_->GetPower());
            p_weapon_->Age();
        }

        p_warr->FightBack(atk_point);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }
};


/*-----------------------------------------------------------------------------*
 *                               City Definition                               *
 *-----------------------------------------------------------------------------*/
class City
{
  private:
    char flag_;
    int life_;
    vector<Warrior*> vec_red_;
    vector<Warrior*> vec_blue_;

  public:
    City()
      : flag_(NONE),
        life_(0),
        vec_red_(),
        vec_blue_()
    {}
};


/*-----------------------------------------------------------------------------*
 *                            HeadQuarter Definition                           *
 *-----------------------------------------------------------------------------*/
typedef struct Config_
{
    int life_hq, num_city, power_arrow, loyalty, minute;
    int life_dragon, life_ninja, life_iceman, life_lion, life_wolf;
    int power_dragon, power_ninja, power_iceman, power_lion, power_wolf;
} Config;


class HeadQuarterRed
{
  private:
    int life_, turn_;
    const Config& config_;

  public:
    HeadQuarterRed(int life, const Config& config)
      : life_(life),
        turn_(1),
        config_(config)
    {}

    Warrior* ProduceWarrior()
    {
        // Early return if there is no enough life to produce new warrior.
        if ((life_ < config_.life_dragon) || (life_ < config_.life_ninja) ||
            (life_ < config_.life_iceman) || (life_ < config_.life_lion) ||
            (life_ < config_.life_wolf))
            return NULL;

        Warrior *p_warr;
        switch (turn_ % TOTAL_TYPE_WARRIOR) {
          case 1:
            life_ -= config_.life_iceman;
            p_warr = new Iceman(turn_, config_.life_iceman, config_.power_iceman,
                                config_.power_arrow);
            break;
          case 2:
            life_ -= config_.life_lion;
            p_warr = new Lion(turn_, config_.life_lion, config_.power_lion,
                              life_, config_.loyalty);
            break;
          case 3:
            life_ -= config_.life_wolf;
            p_warr = new Wolf(turn_, config_.life_wolf, config_.power_wolf);
            break;
          case 4:
            life_ -= config_.life_ninja;
            p_warr = new Ninja(turn_, config_.life_ninja, config_.power_ninja,
                               config_.power_arrow);
            break;
          case 5:
            life_ -= config_.life_dragon;
            p_warr = new Dragon(turn_, config_.life_dragon, config_.power_dragon,
                                static_cast<double>(life_) / config_.life_dragon,
                                config_.power_arrow);
        }

        ++turn_;
        return p_warr;
    }
};

class HeadQuarterBlue
{
  private:
    int life_, turn_;
    const Config& config_;

  public:
    HeadQuarterBlue(int life, const Config& config)
      : life_(life),
        turn_(1),
        config_(config)
    {}

    Warrior* ProduceWarrior()
    {
        // Early return if there is no enough life to produce new warrior.
        if ((life_ < config_.life_dragon) || (life_ < config_.life_ninja) ||
            (life_ < config_.life_iceman) || (life_ < config_.life_lion) ||
            (life_ < config_.life_wolf))
            return NULL;

        Warrior *p_warr;
        switch (turn_ % TOTAL_TYPE_WARRIOR) {
          case 1:
            life_ -= config_.life_lion;
            p_warr = new Lion(turn_, config_.life_lion, config_.power_lion,
                              life_, config_.loyalty);
            break;
          case 2:
            life_ -= config_.life_dragon;
            p_warr = new Dragon(turn_, config_.life_dragon, config_.power_dragon,
                                static_cast<double>(life_) / config_.life_dragon,
                                config_.power_arrow);
            break;
          case 3:
            life_ -= config_.life_ninja;
            p_warr = new Ninja(turn_, config_.life_ninja, config_.power_ninja,
                               config_.power_arrow);
            break;
          case 4:
            life_ -= config_.life_iceman;
            p_warr = new Iceman(turn_, config_.life_iceman, config_.power_iceman,
                                config_.power_arrow);
            break;
          case 5:
            life_ -= config_.life_wolf;
            p_warr = new Wolf(turn_, config_.life_wolf, config_.power_wolf);
        }

        ++turn_;
        return p_warr;
    }
};


/*-----------------------------------------------------------------------------*
 *                            GameMaster Definition                            *
 *-----------------------------------------------------------------------------*/
class GameMaster
{
  private:
    const Config& config_;
    HeadQuarterRed hq_red_;
    HeadQuarterBlue hq_blue_;
    vector<City> vec_city_;

  public:
    GameMaster(const Config& config)
      : config_(config),
        hq_red_(config.life_hq, config),
        hq_blue_(config.life_hq, config),
        vec_city_(config.num_city + 2)
    {}
};


int main()
{
    int test_round;
    cin >> test_round;

    Config config;
    for (int i = 0 ; i < test_round ; ++i) {
        cin >> config.life_hq >> config.num_city >> config.power_arrow \
            >> config.loyalty >> config.minute;

        cin >> config.life_dragon >> config.life_ninja >> config.life_iceman \
            >> config.life_lion >> config.life_wolf;

        cin >> config.power_dragon >> config.power_ninja >> config.power_iceman \
            >> config.power_lion >> config.power_wolf;
    }

    return 0;
}