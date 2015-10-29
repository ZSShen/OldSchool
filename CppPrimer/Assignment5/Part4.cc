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
    Weapon *p_sword_, *p_bomb_, *p_arrow_;
    string race_;

  public:
    Warrior(const string& race, int id, int life, int power)
      : id_(id),
        life_(life),
        power_(power),
        step_(0),
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

    virtual void Attack(Warrior *p_warr) = 0;
    virtual void Wound(Warrior *p_warr, int atk_point) = 0;
    virtual void FightBack(int atk_point) = 0;

    int GetId()
    {
        return id_;
    }

    int GetLifePoint()
    {
        return life_;
    }

    int GetAttackPower()
    {
        return power_;
    }

    void WalkAhead()
    {
        ++step_;
    }

    int GetWalkStep()
    {
        return step_;
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
        return (p_arrow_)? true : false;
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

    void BeShot(int atk_point)
    {
        life_ -= atk_point;
    }

    void BeBombed()
    {
        life_ = 0;
    }
};

class Dragon : public Warrior
{
  private:
    double morale_;

  public:
    Dragon(int id, int life, int power_me, double morale, int power_arrow)
      : Warrior("dragon", id, life, power_me),
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

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
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
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_warr->FightBack(atk_point);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }

    double GetMorale()
    {
        return morale_;
    }
};

class Ninja : public Warrior
{
  public:
    Ninja(int id, int life, int power_me, int power_arrow)
      : Warrior("ninja", id, life, power_me)
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

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
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
    int step_;

  public:
    Iceman(int id, int life, int power_me, int power_arrow)
      : Warrior("iceman", id, life, power_me),
        step_(0)
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

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_warr->Wound(this, atk_point);
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;

        atk_point = power_ >> 1;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_warr->FightBack(atk_point);
    }

    void FightBack(int atk_point)
    {
        life_ -= atk_point;
    }

    void BeFrantic()
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
    int loyalty_, degrade_;

  public:
    Lion(int id, int life, int power, int loyalty, int degrade)
      : Warrior("lion", id, life, power),
        loyalty_(loyalty),
        degrade_(degrade)
    {}

    void Attack(Warrior *p_warr)
    {
        int life = life_;
        p_warr->Wound(this, power_);

        // Degrade its loyalty if it cannot kill the rival.
        if (p_warr->IsAlive())
            loyalty_ -= degrade_;

        // Transfer the life point to its rival.
        if (life_ <= 0)
            p_warr->AddLifePoint(life);
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

    bool IsLoyal()
    {
        return (loyalty_ > 0)? true : false;
    }

    int GetLoyalty()
    {
        return loyalty_;
    }
};

class Wolf : public Warrior
{
  public:
    Wolf(int id, int life, int power)
      : Warrior("wolf", id, life, power)
    {}

    void Attack(Warrior *p_warr)
    {
        int atk_point = power_;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
        }
        p_warr->Wound(this, atk_point);

        if (p_warr->IsAlive())
            return;

        // If its rival is dead, grab the rival's weapon.
        if (!p_sword_ && p_warr->HasSword())
            p_sword_ = p_warr->OfferSword();
        if (!p_bomb_ && p_warr->HasBomb())
            p_bomb_ = p_warr->OfferBomb();
        if (!p_arrow_ && p_warr->HasArrow())
            p_arrow_ = p_warr->OfferArrow();
    }

    void Wound(Warrior *p_warr, int atk_point)
    {
        life_ -= atk_point;
        if (life_ <= 0)
            return;

        atk_point = power_ >> 1;
        if (p_sword_ && p_sword_->IsUseable()) {
            atk_point += static_cast<int>(p_sword_->GetPower());
            p_sword_->Age();
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
class Place
{
  private:
    char flag_;
    int life_;
    queue<Warrior*> q_red_;
    queue<Warrior*> q_blue_;

  public:
    enum {FLAG_NONE, FLAG_RED, FLAG_BLUE};

    Place(char flag = FLAG_NONE, int life = 0)
      : flag_(flag),
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

    virtual Warrior* ProduceWarrior() = 0;

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

    Warrior* ProduceWarrior();
};

class HeadQuarterBlue : public HeadQuarter
{
  public:
    HeadQuarterBlue(int life, const Config& config)
      : HeadQuarter(life, config)
    {}

    Warrior* ProduceWarrior();
};

Warrior* HeadQuarterRed::ProduceWarrior()
{
    Warrior *p_warr = NULL;
    switch (turn_ % TOTAL_TYPE_WARRIOR) {
      case 1:
        if (life_ < config_.life_iceman)
            break;
        life_ -= config_.life_iceman;
        p_warr = new Iceman(turn_, config_.life_iceman, config_.power_iceman,
                            config_.power_arrow);
        break;
      case 2:
        if (life_ < config_.life_lion)
            break;
        life_ -= config_.life_lion;
        p_warr = new Lion(turn_, config_.life_lion, config_.power_lion,
                          life_, config_.loyalty);
        break;
      case 3:
        if (life_ < config_.life_wolf)
            break;
        life_ -= config_.life_wolf;
        p_warr = new Wolf(turn_, config_.life_wolf, config_.power_wolf);
        break;
      case 4:
        if (life_ < config_.life_ninja)
            break;
        life_ -= config_.life_ninja;
        p_warr = new Ninja(turn_, config_.life_ninja, config_.power_ninja,
                           config_.power_arrow);
        break;
      case 5:
        if (life_ < config_.life_dragon)
            break;
        life_ -= config_.life_dragon;
        p_warr = new Dragon(turn_, config_.life_dragon, config_.power_dragon,
                            static_cast<double>(life_) / config_.life_dragon,
                            config_.power_arrow);
    }

    ++turn_;
    return p_warr;
}

Warrior* HeadQuarterBlue::ProduceWarrior()
{
    Warrior *p_warr = NULL;
    switch (turn_ % TOTAL_TYPE_WARRIOR) {
      case 1:
        if (life_ < config_.life_lion)
            break;
        life_ -= config_.life_lion;
        p_warr = new Lion(turn_, config_.life_lion, config_.power_lion,
                          life_, config_.loyalty);
        break;
      case 2:
        if (life_ < config_.life_dragon)
            break;
        life_ -= config_.life_dragon;
        p_warr = new Dragon(turn_, config_.life_dragon, config_.power_dragon,
                            static_cast<double>(life_) / config_.life_dragon,
                            config_.power_arrow);
        break;
      case 3:
        if (life_ < config_.life_ninja)
            break;
        life_ -= config_.life_ninja;
        p_warr = new Ninja(turn_, config_.life_ninja, config_.power_ninja,
                           config_.power_arrow);
        break;
      case 4:
        if (life_ < config_.life_iceman)
            break;
        life_ -= config_.life_iceman;
        p_warr = new Iceman(turn_, config_.life_iceman, config_.power_iceman,
                            config_.power_arrow);
        break;
      case 5:
        if (life_ < config_.life_wolf)
            break;
        life_ -= config_.life_wolf;
        p_warr = new Wolf(turn_, config_.life_wolf, config_.power_wolf);
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
    void ProduceWarriorPostLog(int, const char*, Warrior*);

    void FleeLion(int);
    void FleeLionPostLog(int, const char*, Lion*);

    void MarchWarrior(int);
    void MarchWarriorPostLogToCity(int, const char*, Warrior*, int);
    void MarchWarriorPostLogToHQ(int, const char*, Warrior*, const char*);
    void MarchWarriorPostLogTakeHQ(int, const char*);

    void ProduceLifePoint();
    void GatherLifePoint(int);
    void GatherLifePointPostLog(int, const char*, Warrior*, int);

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

        ++hour;
    }
}

void GameMaster::ProduceWarrior(int hour)
{
    Warrior* p_warr = p_redhq_->ProduceWarrior();
    if (p_warr) {
        vec_place_[POS_REDHQ].SetRedWarrior(p_warr);
        ProduceWarriorPostLog(hour, TROOP_RED, p_warr);
    }

    p_warr = p_bluehq_->ProduceWarrior();
    if (p_warr) {
        vec_place_[POS_BLUEHQ].SetBlueWarrior(p_warr);
        ProduceWarriorPostLog(hour, TROOP_BLUE, p_warr);
    }
}

void GameMaster::ProduceWarriorPostLog(int hour, const char* troop,
                                       Warrior* p_warr)
{
    char msg[SIZE_BLAH_BUF];
    int ofst = sprintf(msg, "%03d:00 %s %s %d born\n", hour, troop,
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
            FleeLionPostLog(hour, TROOP_RED, p_lion);
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
                FleeLionPostLog(hour, TROOP_RED, p_lion);
                delete p_warr;
                vec_place_[i].PopRedWarrior();
            }
        }

        p_warr = city.GetBlueWarrior();
        if (p_warr && typeid(*p_warr) == typeid(Lion)) {
            Lion* p_lion = static_cast<Lion*>(p_warr);
            if (!p_lion->IsLoyal()) {
                FleeLionPostLog(hour, TROOP_BLUE, p_lion);
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
            FleeLionPostLog(hour, TROOP_RED, p_lion);
            delete p_warr;
            vec_place_[POS_BLUEHQ].PopBlueWarrior();
        }
    }
}

void GameMaster::FleeLionPostLog(int hour, const char* troop, Lion* p_lion)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:05 %s lion %d ran away\n", hour, troop, p_lion->GetId());
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
        MarchWarriorPostLogToHQ(hour, TROOP_BLUE, p_warr, TROOP_RED);
        if (vec_place_[POS_REDHQ].CountBlueWarrior() == 2)
            MarchWarriorPostLogTakeHQ(hour, TROOP_RED);
    }

    // Handle the warriors ready to march for cities from #1 to #N.
    for (int i = 1 ; i < POS_BLUEHQ ; ++i) {
        p_warr = vec_place_[i - 1].GetRedWarrior();
        if (p_warr && (p_warr->GetWalkStep() == hour)) {
            p_warr->WalkAhead();
            vec_place_[i].SetRedWarrior(p_warr);
            vec_place_[i - 1].PopRedWarrior();
            MarchWarriorPostLogToCity(hour, TROOP_RED, p_warr, i);
        }

        p_warr = vec_place_[i + 1].GetBlueWarrior();
        if (p_warr && (p_warr->GetWalkStep() == hour)) {
            p_warr->WalkAhead();
            vec_place_[i].SetBlueWarrior(p_warr);
            vec_place_[i + 1].PopBlueWarrior();
            MarchWarriorPostLogToCity(hour, TROOP_BLUE, p_warr, i);
        }
    }

    // Handle the warrior in red troop which is ready to march for blue HQ.
    p_warr = vec_place_[POS_BLUEHQ - 1].GetRedWarrior();
    if (p_warr && (p_warr->GetWalkStep() == hour)) {
        p_warr->WalkAhead();
        vec_place_[POS_BLUEHQ].SetRedWarrior(p_warr);
        vec_place_[POS_BLUEHQ - 1].PopRedWarrior();
        MarchWarriorPostLogToHQ(hour, TROOP_RED, p_warr, TROOP_BLUE);
        if (vec_place_[POS_BLUEHQ].CountRedWarrior() == 2)
            MarchWarriorPostLogTakeHQ(hour, TROOP_BLUE);
    }
}

void GameMaster::MarchWarriorPostLogToCity(int hour, const char* troop,
                                           Warrior* p_warr, int idx)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:10 %s %s %d marched to city %d with %d elements and force %d\n",
            hour, troop, p_warr->GetRace().c_str(), p_warr->GetId(),
            idx, p_warr->GetLifePoint(), p_warr->GetAttackPower());
    cout << msg;
}

void GameMaster::MarchWarriorPostLogToHQ(int hour, const char* troop_warr,
                                         Warrior* p_warr, const char* troop_enemy)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:10 %s %s %d reached %s headquarter with %d elements and force %d\n",
            hour, troop_warr, p_warr->GetRace().c_str(), p_warr->GetId(),
            troop_enemy, p_warr->GetLifePoint(), p_warr->GetAttackPower());
    cout << msg;
}

void GameMaster::MarchWarriorPostLogTakeHQ(int hour, const char* troop)
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
            GatherLifePointPostLog(hour, TROOP_RED, p_warr, life);
            continue;
        }
        if ((city.CountRedWarrior() == 0) && (city.CountBlueWarrior() == 1)) {
            Warrior* p_warr = city.GetBlueWarrior();
            int life = city.BeHunted();
            p_bluehq_->IncreaseLifePoint(life);
            GatherLifePointPostLog(hour, TROOP_RED, p_warr, life);
        }
    }
}

void GameMaster::GatherLifePointPostLog(int hour, const char* troop,
                                        Warrior* p_warr, int life)
{
    char msg[SIZE_BLAH_BUF];
    sprintf(msg, "%03d:30 %s %s %d earned %d elements for his headquarter\n",
            hour, troop, p_warr->GetRace().c_str(), p_warr->GetId(), life);
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