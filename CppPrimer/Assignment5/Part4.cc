#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdio>

using namespace std;


const int SIZE_BLAH_BUF = 512;
const char* TROOP_RED = "red";
const char* TROOP_BLUE = "blue";

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

    virtual ~Weapon()
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
    int id_, life_, power_, step_;
    const char* troop_;
    Weapon *p_sword_, *p_bomb_, *p_arrow_;
    string race_;

  public:
    Warrior(const char* troop, const string& race, int id, int life, int power)
      : id_(id),
        life_(life),
        power_(power),
        step_(0),
        troop_(troop),
        p_sword_(NULL),
        p_bomb_(NULL),
        p_arrow_(NULL),
        race_(race)
    {}

    virtual ~Warrior()
    {
        if (p_sword_)
            delete p_sword_;
        if (p_bomb_)
            delete p_bomb_;
        if (p_arrow_)
            delete p_arrow_;
    }

    virtual void Attack(Warrior*) = 0;
    virtual void FightBack(Warrior*) = 0;
    virtual void ChargeAfterFight(Warrior*) = 0;

    virtual int GetAttackPower();
    virtual int GetFightBackPower(int);
    virtual bool WillBeKilled(Warrior*);

    virtual void WalkAhead()
    {
        ++step_;
    }

    int GetId()
    {
        return id_;
    }

    int GetLifePoint()
    {
        return life_;
    }

    int GetAttackPowerNoWeapon()
    {
        return power_;
    }

    int GetWalkStep()
    {
        return step_;
    }

    const char* GetTroop()
    {
        return troop_;
    }

    const string& GetRace()
    {
        return race_;
    }

    bool IsAlive()
    {
        return (life_ > 0)? true : false;
    }

    bool HasSword()
    {
        return (p_sword_)? true : false;
    }

    bool HasBomb()
    {
        return (p_bomb_)? true : false;
    }

    bool HasArrow()
    {
        if (!p_arrow_)
            return false;
        return (p_arrow_->IsUseable())? true : false;
    }

    Weapon* OfferSword()
    {
        Weapon* p_weapon = p_sword_;
        p_sword_ = NULL;
        return p_weapon;
    }

    Weapon* OfferBomb()
    {
        Weapon* p_weapon = p_bomb_;
        p_bomb_ = NULL;
        return p_weapon;
    }

    Weapon* OfferArrow()
    {
        Weapon* p_weapon = p_arrow_;
        p_arrow_ = NULL;
        return p_weapon;
    }

    void AddLifePoint(int life)
    {
        life_ += life;
    }

    int Shot()
    {
        p_arrow_->Age();
        return static_cast<int>(p_arrow_->GetPower());
    }

    void BeWound(int atk_point)
    {
        life_ = (life_ > atk_point)? (life_ - atk_point) : 0;
    }
};

int Warrior::GetAttackPower()
{
    int atk_point = power_;
    if (p_sword_ && p_sword_->IsUseable())
        atk_point += static_cast<int>(p_sword_->GetPower());
    return atk_point;
}

int Warrior::GetFightBackPower(int atk_point)
{
    if (atk_point >= life_)
        return 0;
    atk_point = power_ >> 1;
    if (p_sword_ && p_sword_->IsUseable())
        atk_point += static_cast<int>(p_sword_->GetPower());
    return atk_point;
}

bool Warrior::WillBeKilled(Warrior* p_enemy)
{
    if (p_enemy->GetAttackPower() >= life_)
        return true;
    int atk_point = GetAttackPower();
    if (p_enemy->GetFightBackPower(atk_point) >= life_)
        return true;
    return false;
}


class Dragon : public Warrior
{
  private:
    double morale_;

  public:
    Dragon(const char* troop, int id, int life, int power_me, double morale,
           int power_arrow)
      : Warrior(troop, "dragon", id, life, power_me),
        morale_(morale)
    {
        // Take the weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_sword_ = new Sword(static_cast<double>(power_me) * 0.2);
            break;
          case BOMB:
            p_bomb_ = new Bomb();
            break;
          case ARROW:
            p_arrow_ = new Arrow(static_cast<double>(power_arrow));
        }
    }

    void Attack(Warrior* p_enemy)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    void ChargeAfterFight(Warrior* p_enemy)
    {
        if (p_enemy->IsAlive())
            morale_ -= 0.2;
        else
            morale_ += 0.2;
    }

    void FightBack(Warrior* p_enemy)
    {
        int atk_point = power_ >> 1;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    double GetMorale()
    {
        return morale_;
    }
};

class Ninja : public Warrior
{
  public:
    Ninja(const char* troop, int id, int life, int power_me, int power_arrow)
      : Warrior(troop, "ninja", id, life, power_me)
    {
        // Take the first weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_sword_ = new Sword(static_cast<double>(power_me) * 0.2);
            break;
          case BOMB:
            p_bomb_ = new Bomb();
            break;
          case ARROW:
            p_arrow_ = new Arrow(static_cast<double>(power_arrow));
        }
        // Take the second weapon.
        switch ((id + 1) % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_sword_ = new Sword(static_cast<double>(power_me) * 0.2);
            break;
          case BOMB:
            p_bomb_ = new Bomb();
            break;
          case ARROW:
            p_arrow_ = new Arrow(static_cast<double>(power_arrow));
        }
    }

    void Attack(Warrior* p_enemy)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    void ChargeAfterFight(Warrior* p_enemy)
    {}

    void FightBack(Warrior* p_enemy)
    {}

    int GetFightBackPower(int atk_point)
    {
        return 0;
    }
};

class Iceman : public Warrior
{
  public:
    Iceman(const char* troop, int id, int life, int power_me, int power_arrow)
      : Warrior(troop, "iceman", id, life, power_me)
    {
        // Take the weapon.
        switch (id % TOTAL_TYPE_WEAPON) {
          case SWORD:
            p_sword_ = new Sword(static_cast<double>(power_me) * 0.2);
            break;
          case BOMB:
            p_bomb_ = new Bomb();
            break;
          case ARROW:
            p_arrow_ = new Arrow(static_cast<double>(power_arrow));
        }
    }

    void Attack(Warrior* p_enemy)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    void ChargeAfterFight(Warrior* p_enemy)
    {}

    void FightBack(Warrior* p_enemy)
    {
        int atk_point = power_ >> 1;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    void WalkAhead()
    {
        ++step_;
        if (step_ == 2) {
            step_ = 0;
            life_ -= 9;
            if (life_ <= 0)
                life_ = 1;
            power_ += 20;
        }
    }
};

class Lion : public Warrior
{
  private:
    int loyalty_, degrade_, legacy_;

  public:
    Lion(const char* troop, int id, int life, int power, int loyalty, int degrade)
      : Warrior(troop, "lion", id, life, power),
        loyalty_(loyalty),
        degrade_(degrade),
        legacy_(life)
    {}

    void BeWound(int atk_point)
    {
        legacy_ = life_;
        life_ = (life_ > atk_point)? (life_ - atk_point) : 0;
    }

    void Attack(Warrior* p_enemy)
    {
        int life = life_;
        p_enemy->BeWound(power_);
    }

    void ChargeAfterFight(Warrior* p_enemy)
    {
        if (p_enemy->IsAlive())
            loyalty_ -= degrade_;

        if (!IsAlive())
            p_enemy->AddLifePoint(legacy_);
    }

    void FightBack(Warrior* p_enemy)
    {
        p_enemy->BeWound(power_ >> 1);
    }

    int GetAttackPower()
    {
        return power_;
    }

    int GetFightBackPower(int atk_point)
    {
        return (atk_point >= life_)? 0 : (power_ >> 1);
    }

    bool IsLoyal()
    {
        return (loyalty_ > 0)? true : false;
    }

    int GetLoyalty()
    {
        return loyalty_;
    }

    int GetLegacy()
    {
        return legacy_;
    }
};

class Wolf : public Warrior
{
  public:
    Wolf(const char* troop, int id, int life, int power)
      : Warrior(troop, "wolf", id, life, power)
    {}

    void Attack(Warrior* p_enemy)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }

    void ChargeAfterFight(Warrior* p_enemy)
    {
        if (p_enemy->IsAlive())
            return;

        if (!p_sword_ && p_enemy->HasSword())
            p_sword_ = p_enemy->OfferSword();
        if (!p_bomb_ && p_enemy->HasBomb())
            p_bomb_ = p_enemy->OfferBomb();
        if (!p_arrow_ && p_enemy->HasArrow())
            p_arrow_ = p_enemy->OfferArrow();
    }

    void FightBack(Warrior* p_enemy)
    {
        int atk_point = power_ >> 1;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_enemy->BeWound(atk_point);
    }
};


/*-----------------------------------------------------------------------------*
 *                               City Definition                               *
 *-----------------------------------------------------------------------------*/
class Place
{
  private:
    char flag_, streak_;
    int life_;
    queue<Warrior*> q_red_;
    queue<Warrior*> q_blue_;

  public:
    enum {FLAG_NONE, FLAG_RED, FLAG_BLUE};

    Place(char flag = FLAG_NONE, int life = 0)
      : flag_(flag),
        streak_(0),
        life_(life),
        q_red_(),
        q_blue_()
    {}

    ~Place()
    {
        while (!q_red_.empty()) {
            Warrior* p_warr = q_red_.front();
            delete p_warr;
            q_red_.pop();
        }
        while (!q_blue_.empty()) {
            Warrior* p_warr = q_blue_.front();
            delete p_warr;
            q_blue_.pop();
        }
    }

    void MarkRedWin()
    {
        switch (streak_) {
            case -2:
            case -1:
            case 0:
                streak_ = 1;
                break;
            case 1:
                streak_ = 2;
                flag_ = FLAG_RED;
        }
    }

    void MarkBlueWin()
    {
        switch (streak_) {
            case 2:
            case 1:
            case 0:
                streak_ = -1;
                break;
            case -1:
                streak_ = -2;
                flag_ = FLAG_BLUE;
        }
    }

    char GetFlag()
    {
        return flag_;
    }

    void ProduceLifePoint()
    {
        life_ += 10;
    }

    int BeHunted()
    {
        int life = life_;
        life_ = 0;
        return life;
    }

    void SetRedWarrior(Warrior* p_warr)
    {
        q_red_.push(p_warr);
    }

    void SetBlueWarrior(Warrior* p_warr)
    {
        q_blue_.push(p_warr);
    }

    Warrior* GetRedWarrior()
    {
        return (q_red_.empty())? NULL : q_red_.front();
    }

    Warrior* GetBlueWarrior()
    {
        return (q_blue_.empty())? NULL : q_blue_.front();
    }

    void PopRedWarrior()
    {
        if (!q_red_.empty())
            q_red_.pop();
    }

    void PopBlueWarrior()
    {
        if (!q_blue_.empty())
            q_blue_.pop();
    }

    int CountRedWarrior()
    {
        return q_red_.size();
    }

    int CountBlueWarrior()
    {
        return q_blue_.size();
    }
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


class HeadQuarter
{
  protected:
    int life_, turn_;
    const Config& config_;

  public:
    HeadQuarter(int life, const Config& config)
      : life_(life),
        turn_(1),
        config_(config)
    {}

    virtual ~HeadQuarter()
    {}

    virtual Warrior* ProduceWarrior(const char*) = 0;

    void IncreaseLifePoint(int life)
    {
        life_ += life;
    }
};

class HeadQuarterRed : public HeadQuarter
{
  public:
    HeadQuarterRed(int life, const Config& config)
      : HeadQuarter(life, config)
    {}

    Warrior* ProduceWarrior(const char*);
};

class HeadQuarterBlue : public HeadQuarter
{
  public:
    HeadQuarterBlue(int life, const Config& config)
      : HeadQuarter(life, config)
    {}

    Warrior* ProduceWarrior(const char*);
};

Warrior* HeadQuarterRed::ProduceWarrior(const char* troop)
{
    Warrior *p_warr = NULL;
    switch (turn_ % TOTAL_TYPE_WARRIOR) {
      case 1:
        if (life_ < config_.life_iceman)
            break;
        life_ -= config_.life_iceman;
        p_warr = new Iceman(troop, turn_, config_.life_iceman, config_.power_iceman,
                            config_.power_arrow);
        break;
      case 2:
        if (life_ < config_.life_lion)
            break;
        life_ -= config_.life_lion;
        p_warr = new Lion(troop, turn_, config_.life_lion, config_.power_lion,
                          life_, config_.loyalty);
        break;
      case 3:
        if (life_ < config_.life_wolf)
            break;
        life_ -= config_.life_wolf;
        p_warr = new Wolf(troop, turn_, config_.life_wolf, config_.power_wolf);
        break;
      case 4:
        if (life_ < config_.life_ninja)
            break;
        life_ -= config_.life_ninja;
        p_warr = new Ninja(troop, turn_, config_.life_ninja, config_.power_ninja,
                           config_.power_arrow);
        break;
      case 5:
        if (life_ < config_.life_dragon)
            break;
        life_ -= config_.life_dragon;
        p_warr = new Dragon(troop, turn_, config_.life_dragon, config_.power_dragon,
                            static_cast<double>(life_) / config_.life_dragon,
                            config_.power_arrow);
    }

    ++turn_;
    return p_warr;
}

Warrior* HeadQuarterBlue::ProduceWarrior(const char* troop)
{
    Warrior *p_warr = NULL;
    switch (turn_ % TOTAL_TYPE_WARRIOR) {
      case 1:
        if (life_ < config_.life_lion)
            break;
        life_ -= config_.life_lion;
        p_warr = new Lion(troop, turn_, config_.life_lion, config_.power_lion,
                          life_, config_.loyalty);
        break;
      case 2:
        if (life_ < config_.life_dragon)
            break;
        life_ -= config_.life_dragon;
        p_warr = new Dragon(troop, turn_, config_.life_dragon, config_.power_dragon,
                            static_cast<double>(life_) / config_.life_dragon,
                            config_.power_arrow);
        break;
      case 3:
        if (life_ < config_.life_ninja)
            break;
        life_ -= config_.life_ninja;
        p_warr = new Ninja(troop, turn_, config_.life_ninja, config_.power_ninja,
                           config_.power_arrow);
        break;
      case 4:
        if (life_ < config_.life_iceman)
            break;
        life_ -= config_.life_iceman;
        p_warr = new Iceman(troop, turn_, config_.life_iceman, config_.power_iceman,
                            config_.power_arrow);
        break;
      case 5:
        if (life_ < config_.life_wolf)
            break;
        life_ -= config_.life_wolf;
        p_warr = new Wolf(troop, turn_, config_.life_wolf, config_.power_wolf);
    }

    ++turn_;
    return p_warr;
}


/*-----------------------------------------------------------------------------*
 *                            GameMaster Definition                            *
 *-----------------------------------------------------------------------------*/
class GameMaster
{
  private:
    int POS_REDHQ, POS_BLUEHQ;
    const Config& config_;
    HeadQuarter *p_redhq_, *p_bluehq_;
    vector<Place> vec_place_;

    void ProduceWarrior(int);
    void ProduceWarriorLog(int, Warrior*);

    void FleeLion(int);
    void FleeLionLog(int, Lion*);

    void MarchWarrior(int);
    void MarchWarriorLogToCity(int, Warrior*, int);
    void MarchWarriorLogToHQ(int, Warrior*, const char*);
    void MarchWarriorLogTakeHQ(int, const char*);

    void ProduceLifePoint();
    void GatherLifePoint(int);
    void GatherLifePointLog(int, Warrior*, int);

    void ShotEnemy(int);
    void ShotEnemyLog(int, Warrior*, Warrior*);

    void ExplodeEnemy(int);
    void ExplodeEnemyLog(int, Warrior*, Warrior*);

    void Fight(int);
    void ActiveFightLog(int, int, Warrior*, Warrior*);
    void PassiveFightLog(int, int, Warrior*, Warrior*);
    void DieLog(int, int, Warrior*);
    void YellLog(int, int, Warrior*);

    bool CheckTermination();

  public:
    GameMaster(const Config& config)
      : POS_REDHQ(0),
        POS_BLUEHQ(config.num_city + 1),
        config_(config),
        p_redhq_(new HeadQuarterRed(config.life_hq, config)),
        p_bluehq_(new HeadQuarterBlue(config.life_hq, config)),
        vec_place_(config.num_city + 2)
    {}

    ~GameMaster()
    {
        if (p_redhq_)
            delete p_redhq_;
        if (p_bluehq_)
            delete p_bluehq_;
    }

    void Run();
};

void GameMaster::Run()
{
    int hour = 0;
    int total = config_.minute / 60;
    while (hour < total) {
        if (CheckTermination())
            break;

        // At h:00, HQs produce warriors.
        ProduceWarrior(hour);

        // At h:05, the lions with loyalty less than or equal to 0 should flee.
        FleeLion(hour);

        // At h:10, each warrior marches one step further to the enemy HQ.
        MarchWarrior(hour);

        // At h:20, each city produces 10 life points.
        ProduceLifePoint();

        // At h:30, warriors try to gather life points for their HQ.
        GatherLifePoint(hour);

        // At h:35, warriors having arrows should their rivals.
        ShotEnemy(hour);

        // At h:38, warriors having bombs try to explode rivals.
        ExplodeEnemy(hour);

        ++hour;
    }
}

void GameMaster::ProduceWarrior(int hour)
{
    Warrior* p_warr = p_redhq_->ProduceWarrior(TROOP_RED);
    if (p_warr) {
        vec_place_[POS_REDHQ].SetRedWarrior(p_warr);
        ProduceWarriorLog(hour, p_warr);
    }

    p_warr = p_bluehq_->ProduceWarrior(TROOP_BLUE);
    if (p_warr) {
        vec_place_[POS_BLUEHQ].SetBlueWarrior(p_warr);
        ProduceWarriorLog(hour, p_warr);
    }
}

void GameMaster::ProduceWarriorLog(int hour, Warrior* p_warr)
{
    char msg[SIZE_BLAH_BUF];
    int ofst = sprintf(msg, "%03d:00 %s %s %d born\n", hour, p_warr->GetTroop(),
                       p_warr->GetRace().c_str(), p_warr->GetId());

    if (typeid(*p_warr) == typeid(Lion))
        sprintf(msg + ofst, "Its loyalty is %d\n",
                static_cast<Lion*>(p_warr)->GetLoyalty());
    else if (typeid(*p_warr) == typeid(Dragon))
        sprintf(msg + ofst, "Its morale is %.2lf\n",
                static_cast<Dragon*>(p_warr)->GetMorale());

    cout << msg;
}

void GameMaster::FleeLion(int hour)
{
    // Handle the lion standing at red HQ.
    Warrior* p_warr = vec_place_[POS_REDHQ].GetRedWarrior();
    if (p_warr && typeid(*p_warr) == typeid(Lion)) {
        Lion* p_lion = static_cast<Lion*>(p_warr);
        if (!p_lion->IsLoyal()) {
            FleeLionLog(hour, p_lion);
            delete p_warr;
            vec_place_[POS_REDHQ].PopRedWarrior();
        }
    }

    // Handle the lions standing at each city.
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        Place& city = vec_place_[i];

        p_warr = city.GetRedWarrior();
        if (p_warr && typeid(*p_warr) == typeid(Lion)) {
            Lion* p_lion = static_cast<Lion*>(p_warr);
            if (!p_lion->IsLoyal()) {
                FleeLionLog(hour, p_lion);
                delete p_warr;
                vec_place_[i].PopRedWarrior();
            }
        }

        p_warr = city.GetBlueWarrior();
        if (p_warr && typeid(*p_warr) == typeid(Lion)) {
            Lion* p_lion = static_cast<Lion*>(p_warr);
            if (!p_lion->IsLoyal()) {
                FleeLionLog(hour, p_lion);
                delete p_warr;
                vec_place_[i].PopBlueWarrior();
            }
        }
    }

    // Handle the lion standing at blue HQ.
    p_warr = vec_place_[POS_BLUEHQ].GetBlueWarrior();
    if (p_warr && typeid(*p_warr) == typeid(Lion)) {
        Lion* p_lion = static_cast<Lion*>(p_warr);
        if (!p_lion->IsLoyal()) {
            FleeLionLog(hour, p_lion);
            delete p_warr;
            vec_place_[POS_BLUEHQ].PopBlueWarrior();
        }
    }
}

void GameMaster::FleeLionLog(int hour, Lion* p_lion)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:05 %s lion %d ran away\n", hour, p_lion->GetTroop(),
            p_lion->GetId());
    cout << msg;
}

void GameMaster::MarchWarrior(int hour)
{
    // Handle the warrior in blue troop which is ready to march for red HQ.
    Warrior* p_warr = vec_place_[POS_REDHQ + 1].GetBlueWarrior();
    if (p_warr && (p_warr->GetWalkStep() == hour)) {
        p_warr->WalkAhead();
        vec_place_[POS_REDHQ].SetBlueWarrior(p_warr);
        vec_place_[POS_REDHQ + 1].PopBlueWarrior();
        MarchWarriorLogToHQ(hour, p_warr, TROOP_RED);
        if (vec_place_[POS_REDHQ].CountBlueWarrior() == 2)
            MarchWarriorLogTakeHQ(hour, TROOP_RED);
    }

    // Handle the warriors ready to march for cities from #1 to #N.
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        p_warr = vec_place_[i - 1].GetRedWarrior();
        if (p_warr && (p_warr->GetWalkStep() == hour)) {
            p_warr->WalkAhead();
            vec_place_[i].SetRedWarrior(p_warr);
            vec_place_[i - 1].PopRedWarrior();
            MarchWarriorLogToCity(hour, p_warr, i);
        }

        p_warr = vec_place_[i + 1].GetBlueWarrior();
        if (p_warr && (p_warr->GetWalkStep() == hour)) {
            p_warr->WalkAhead();
            vec_place_[i].SetBlueWarrior(p_warr);
            vec_place_[i + 1].PopBlueWarrior();
            MarchWarriorLogToCity(hour, p_warr, i);
        }
    }

    // Handle the warrior in red troop which is ready to march for blue HQ.
    p_warr = vec_place_[POS_BLUEHQ - 1].GetRedWarrior();
    if (p_warr && (p_warr->GetWalkStep() == hour)) {
        p_warr->WalkAhead();
        vec_place_[POS_BLUEHQ].SetRedWarrior(p_warr);
        vec_place_[POS_BLUEHQ - 1].PopRedWarrior();
        MarchWarriorLogToHQ(hour, p_warr, TROOP_BLUE);
        if (vec_place_[POS_BLUEHQ].CountRedWarrior() == 2)
            MarchWarriorLogTakeHQ(hour, TROOP_BLUE);
    }
}

void GameMaster::MarchWarriorLogToCity(int hour, Warrior* p_warr, int idx)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:10 %s %s %d marched to city %d with %d elements and force %d\n",
            hour, p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(),
            idx, p_warr->GetLifePoint(), p_warr->GetAttackPowerNoWeapon());
    cout << msg;
}

void GameMaster::MarchWarriorLogToHQ(int hour, Warrior* p_warr, const char* troop_enemy)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:10 %s %s %d reached %s headquarter with %d elements and force %d\n",
            hour, p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(),
            troop_enemy, p_warr->GetLifePoint(), p_warr->GetAttackPower());
    cout << msg;
}

void GameMaster::MarchWarriorLogTakeHQ(int hour, const char* troop)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:10 %s headquarter was taken\n", hour, troop);
    cout << msg;
}

void GameMaster::ProduceLifePoint()
{
    for (int i = 1 ; i < POS_BLUEHQ ; ++i)
        vec_place_[i].ProduceLifePoint();
}

void GameMaster::GatherLifePoint(int hour)
{
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        Place& city = vec_place_[i];
        if ((city.CountRedWarrior() == 1) && (city.CountBlueWarrior() == 0)) {
            Warrior* p_warr = city.GetRedWarrior();
            int life = city.BeHunted();
            p_redhq_->IncreaseLifePoint(life);
            GatherLifePointLog(hour, p_warr, life);
            continue;
        }
        if ((city.CountRedWarrior() == 0) && (city.CountBlueWarrior() == 1)) {
            Warrior* p_warr = city.GetBlueWarrior();
            int life = city.BeHunted();
            p_bluehq_->IncreaseLifePoint(life);
            GatherLifePointLog(hour, p_warr, life);
        }
    }
}

void GameMaster::GatherLifePointLog(int hour, Warrior* p_warr, int life)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:30 %s %s %d earned %d elements for his headquarter\n",
        hour, p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(), life);
    cout << msg;
}

void GameMaster::ShotEnemy(int hour)
{
    if (config_.num_city == 1)
        return;

    // Handle the warrior standing at city #1.
    Warrior* p_warr = vec_place_[POS_REDHQ + 1].GetRedWarrior();
    Warrior* p_enemy = vec_place_[POS_REDHQ + 2].GetBlueWarrior();
    if (p_warr && p_enemy && p_warr->HasArrow()) {
        int atk_point = p_warr->Shot();
        p_enemy->BeWound(atk_point);
        ShotEnemyLog(hour, p_warr, p_enemy);
    }

    // Handle the warriors standing at cities from #2 to #(N-1).
    for (int i = 2 ; i < POS_BLUEHQ - 1 ; ++i) {
        p_warr = vec_place_[i].GetRedWarrior();
        p_enemy = vec_place_[i + 1].GetBlueWarrior();
        if (p_warr && p_enemy && p_warr->HasArrow()) {
            int atk_point = p_warr->Shot();
            p_enemy->BeWound(atk_point);
            ShotEnemyLog(hour, p_warr, p_enemy);
        }

        p_warr = vec_place_[i].GetBlueWarrior();
        p_enemy = vec_place_[i - 1].GetRedWarrior();
        if (p_warr && p_enemy && p_warr->HasArrow()) {
            int atk_point = p_warr->Shot();
            p_enemy->BeWound(atk_point);
            ShotEnemyLog(hour, p_warr, p_enemy);
        }
    }

    // Handle the warrior standing at city #N.
    p_warr = vec_place_[POS_BLUEHQ - 1].GetBlueWarrior();
    p_enemy = vec_place_[POS_BLUEHQ - 2].GetRedWarrior();
    if (p_warr && p_enemy && p_warr->HasArrow()) {
        int atk_point = p_warr->Shot();
        p_enemy->BeWound(atk_point);
        ShotEnemyLog(hour, p_warr, p_enemy);
    }
}

void GameMaster::ShotEnemyLog(int hour, Warrior* p_warr, Warrior* p_enemy)
{
    char msg[SIZE_BLAH_BUF];
    int ofst = sprintf(msg, "%03d:35 %s %s %d shot\n", hour, p_warr->GetTroop(),
                       p_warr->GetRace().c_str(), p_warr->GetId());
    if (p_enemy->GetLifePoint() == 0)
    sprintf(msg + ofst - 1, " and killed %s %s %d\n", p_enemy->GetTroop(),
            p_enemy->GetRace().c_str(), p_enemy->GetId());
    cout << msg;
}

void GameMaster::ExplodeEnemy(int hour)
{
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        Warrior* p_warr = vec_place_[i].GetRedWarrior();
        Warrior* p_enemy = vec_place_[i].GetBlueWarrior();
        if (p_warr && p_enemy && p_warr->HasBomb() &&
            p_warr->WillBeKilled(p_enemy)) {
            ExplodeEnemyLog(hour, p_warr, p_enemy);
            vec_place_[i].PopRedWarrior();
            vec_place_[i].PopBlueWarrior();
            delete p_warr;
            delete p_enemy;
            continue;
        }

        p_warr = vec_place_[i].GetBlueWarrior();
        p_enemy = vec_place_[i].GetRedWarrior();
        if (p_warr && p_enemy && p_warr->HasBomb() &&
            p_warr->WillBeKilled(p_enemy)) {
            ExplodeEnemyLog(hour, p_warr, p_enemy);
            vec_place_[i].PopBlueWarrior();
            vec_place_[i].PopRedWarrior();
            delete p_warr;
            delete p_enemy;
        }
    }
}

void GameMaster::ExplodeEnemyLog(int hour, Warrior* p_warr, Warrior* p_enemy)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:38 %s %s %d used a bomb and killed %s %s %d\n", hour,
            p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(),
            p_enemy->GetTroop(), p_enemy->GetRace().c_str(), p_enemy->GetId());
    cout << msg;
}

void GameMaster::Fight(int hour)
{
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        bool red_first = true;
        char flag = vec_place_[i].GetFlag();
        switch (flag) {
            case Place::FLAG_RED:
                break;
            case Place::FLAG_BLUE:
                red_first = false;
                break;
            default:
                if ((i >> 1 << 1) == i)
                    red_first = false;
        }

        Warrior *p_active, *p_passive;
        if (red_first) {
            p_active = vec_place_[i].GetRedWarrior();
            p_passive = vec_place_[i].GetBlueWarrior();
        } else {
            p_active = vec_place_[i].GetBlueWarrior();
            p_passive = vec_place_[i].GetRedWarrior();
        }

        if (!p_active || !p_passive)
            return;

        if (!p_passive->IsAlive()) {
            ActiveFightLog(hour, i, p_active, p_passive);
            p_active->Attack(p_passive);
            if (p_passive->IsAlive()) {
                PassiveFightLog(hour, i, p_passive, p_active);
                p_passive->FightBack(p_active);
                if (!p_active->IsAlive())
                    DieLog(hour, i, p_active);
            } else
                DieLog(hour, i, p_passive);
        }

        p_active->ChargeAfterFight(p_passive);
        p_passive->ChargeAfterFight(p_active);

        // Dragon yells if necessary.
        if ((typeid(*p_active) == typeid(Dragon)) && p_active->IsAlive() &&
            (static_cast<Dragon*>(p_active)->GetMorale() > 0.8))
            YellLog(hour, i, p_active);

    }
}

void GameMaster::ActiveFightLog(int hour, int city, Warrior* p_warr, Warrior* p_enemy)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements"
            "and force %d\n", hour, p_warr->GetTroop(), p_warr->GetRace().c_str(),
            p_warr->GetId(), p_enemy->GetTroop(), p_enemy->GetRace().c_str(),
            p_enemy->GetId(), city, p_warr->GetLifePoint(),
            p_warr->GetAttackPowerNoWeapon());
    cout << msg;
}

void GameMaster::PassiveFightLog(int hour, int city, Warrior* p_warr, Warrior* p_enemy)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:40 %s %s %d fought back against %s %s %d in city %d\n",
            hour, p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(),
            p_enemy->GetTroop(), p_enemy->GetRace().c_str(), p_enemy->GetId(), city);
    cout << msg;
}

void GameMaster::DieLog(int hour, int city, Warrior* p_warr)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:40 %s %s %d was killed in city %d\n", hour,
            p_warr->GetTroop(), p_warr->GetRace().c_str(), p_warr->GetId(), city);
    cout << msg;
}

void GameMaster::YellLog(int hour, int city, Warrior* p_warr)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:40 %s %s %d yelled in city %d\n", hour, p_warr->GetTroop(),
            p_warr->GetRace().c_str(), p_warr->GetId(), city);
    cout << msg;
}

bool GameMaster::CheckTermination()
{
    return (vec_place_[POS_REDHQ].CountBlueWarrior() >= 2) ||
           (vec_place_[POS_BLUEHQ].CountRedWarrior() >= 2);
}


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

        GameMaster gm(config);
        gm.Run();
    }

    return 0;
}