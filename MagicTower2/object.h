#pragma once
#include<string>
#include<vector>
#include<map>
#include<memory>
#include"global.h"
#include <iostream>
#include<fstream>
#include<initializer_list>
#include<any>
#include<variant>
using namespace std;
#define DEFINE_SETGET(name,type)  type& Get##name(){return name;} \
    void Set##name(const type& name){this->name=name;}
extern int prid;

enum class Type{
    WEAPEON,
    DEFNECER,
    MEDICINE,
};

enum class State{
    SALEING,
    UNSALEING,
    LOADING,
    UNLOADING
};

class VProp{
public:
    virtual void Display()=0;
    virtual void Save(std::fstream& f)  = 0;
    virtual void Load(std::fstream& f) = 0;
};

class Prop
{
public:
    Prop(){}
    DEFINE_SETGET(Money,int)
    DEFINE_SETGET(Name,string)
    DEFINE_SETGET(m_type,Type)
    DEFINE_SETGET(m_state,State)
    DEFINE_SETGET(mprid,int)
    Prop(const string& name,int Money,Type type,State state):Name(name),Money(Money),m_type(type),m_state(state){}
    virtual void Display()=0;
    virtual ~Prop()=default;
    virtual void Save(std::fstream& f)  = 0;
    virtual void Load(std::fstream& f) = 0;
protected:
    int mprid;
    Type m_type;
    int Money;
    State m_state;
    string Name;
};

// class WeaponDecoator:public Prop{
// public:
//     shared_ptr<Prop> mprop;
// public:
//     // DEFINE_SETGET(Money,int)
//     // DEFINE_SETGET(Name,string)
//     // DEFINE_SETGET(m_type,Type)
//     // DEFINE_SETGET(m_state,State)
//     // DEFINE_SETGET(mprid,int)

//     int& GetMoney(){return mprop->GetMoney();}
//     string& GetName(){return mprop->GetName(); }
//     Type& Getm_type(){return mprop->Getm_type();}
//     State& Getm_state(){ return mprop->Getm_state();}
//     int& Getmprid(){return mprop->Getmprid();}
//     WeaponDecoator(shared_ptr<Prop>prop):mprop(prop){}
//     void Display(){ mprop->Display();};
//     void Save(std::fstream& f) {mprop->Save(f);}
//     void Load(std::fstream& f) {mprop->Load(f);}
// };


// class WeaponEnhance:public WeaponDecoator{
// private:
//     int mlifeSteal;
//     int mCritAttack;
// public:
//     DEFINE_SETGET(mlifeSteal,int)
//     DEFINE_SETGET(mCritAttack,int)
//     WeaponEnhance(shared_ptr<Prop> prop): WeaponDecoator(prop){}
//     WeaponEnhance(shared_ptr<Prop> prop,int lifeSteal,int CritAttack):
//     WeaponDecoator(prop),mlifeSteal(lifeSteal),mCritAttack(CritAttack){}
//     void Display();
//     void Save(std::fstream& f)  {
//         WeaponDecoator::Save(f);
//         f.get();
//         f<<" "<<mlifeSteal<<" "<<mCritAttack;
//     }
//     void Load(std::fstream& f) {
//         WeaponDecoator::Load(f);
//     }
// };


class WeaPon:public Prop{
public:
    WeaPon(){}
    WeaPon(const string & name,int money,Type type,State state,int attack):Prop(name,money,type,state),Attack(attack){mprid=prid++;}
    DEFINE_SETGET(Attack,int)
    void Display();
    void Save(std::fstream& f);
    void Load(std::fstream& f);
    
private:
    int Attack;
};

/* 防具 */
class Defencer:public Prop{
public:
    Defencer(){}
    Defencer(const string& name,int money,Type type,State state,int defend):Prop(name,money,type,state),Defend(defend){mprid=prid++;}
    void Display();
    DEFINE_SETGET(Defend,int)
    void Save(std::fstream& f);
    void Load(std::fstream& f);
private:   
    int Defend;
};


class Medicine:public Prop{
public:
    Medicine(){}
    Medicine(const string& name ,int money,Type type,State state,int value):Prop(name,money,type,state),Value(value){mprid=prid++;}
    void Display();
    DEFINE_SETGET(Value,int);
    void Save(std::fstream& f)  ;
    void Load(std::fstream& f);
private:
    int Value;
};

class PropFactory {
public:
    static Prop* GetWeapon(const std::string& name, int money, Type propType, State state, int specialValue){
        // shared_ptr<WeaPon> weapon= make_shared<WeaPon>(name, money, propType, state, specialValue);
        // WeaponEnhance *weapon1=new WeaponEnhance(weapon);
        return new WeaPon(name, money, propType, state, specialValue);
    }
    static Prop* GetDefencer(const std::string& name, int money, Type propType, State state, int specialValue){

        return new Defencer(name, money, propType, state, specialValue);
    }
    static Prop* GeMedicine(const std::string& name, int money, Type propType, State state, int specialValue){
        return new Medicine(name, money, propType, state, specialValue);
    }

};

class Player;

using PropPtr=shared_ptr<Prop>;

class Shop{
public:
    Shop();
    auto& GetMap() const {return goods;};
    void Display(Player&player);
    void PurchaseGoods(int id,Player& player);
    void Save(std::fstream& f) const ;
    void Load(std::fstream& f) ;
private:
   map<string,vector<PropPtr>> goods;

};

class Bag{
public:
    Bag();
    DEFINE_SETGET(mLifeAmount,int)
    DEFINE_SETGET(mAttactAmount,int)
    DEFINE_SETGET(mDefendAmount,int)
    DEFINE_SETGET(Vbag,vector<PropPtr>)
    void Discard(int id);
    bool Insert(PropPtr& prop);
    void Equip(int id);
    void Unquip(int id);
    void Display();
    void UnLoad(const std::string&);
    void Save(std::fstream& f) const;
    void Load(std::fstream& f,Shop& shop) ;
private:
    vector<PropPtr> Vbag;
    int mLifeAmount;
    int mAttactAmount;
    int mDefendAmount;
};


class Point{
public:
    Point(){};
    Point(int x,int y):X(x),Y(y){}
    DEFINE_SETGET(X,int)
    DEFINE_SETGET(Y,int)
protected:
    int X;
    int Y;
};

class VirtualObject:public Point{
public:
    VirtualObject(){}
    DEFINE_SETGET(Symbol,string)
    DEFINE_SETGET(Name,string)
    DEFINE_SETGET(HP,int)
    DEFINE_SETGET(Attack,int)
    DEFINE_SETGET(Defend,int)
    DEFINE_SETGET(Exp,int)
    DEFINE_SETGET(Money,int)
    VirtualObject(int x,int y,const std::string& symbol,const::string&name,int money, int hp,int attack,int defend,int exp)
    :Point(x,y),Symbol(symbol),Name(name),HP(hp),Attack(attack),Defend(defend),Exp(exp),Money(money) {}
    virtual void Display()=0;
   
protected:
    string Symbol;
    string Name;
    int HP;//声明值
    int Attack;//防御力
    int Defend;
    int Exp;
    int Money;
};


class Player:public VirtualObject{
public:
    Player(){};
    DEFINE_SETGET(bag,Bag);
    void PlayerMove(char seletct);
    Player(int x,int y,const string& symbol,const string&name,int money,int hp,int attack,int defend,int exp=0,int level=0)
    :VirtualObject(x,y,symbol,name,money,hp,attack,defend,exp),Level(level){}
    void Display(){
        printf("位置x：%d，位置y:%d，头像：%s,姓名：%s 金币：%d\n 生命：%d 攻击力：%d 防御力：%d 经验：%d 等级：%d\n",X,Y,
        Symbol.c_str(),Name.c_str(),Money,HP,Attack,Defend,Exp,Level);
    }
    void DisPlayList(initializer_list<string> alist);
    void Save(std::fstream& f) const ;

    void Load(std::fstream& f,Shop& shop)  ;
private:
    int Level;
    Bag bag;
};



class BuildPlayer{
public:
    virtual shared_ptr<Player> GetPlayer()=0;
};

class BuildCPlayer:public BuildPlayer{
public: 
    BuildCPlayer(){
        m_player=make_shared<Player>(0,0,"🤓","小明",100,200,10,5,5,0);
    }
    shared_ptr<Player> GetPlayer(){
        return m_player;
    }
private:
    shared_ptr<Player>m_player;
};

class Monster:public VirtualObject{
public:
    Monster(){}
    Monster(int x,int y,const std::string& symbol,const::string&name,int money,int hp,int attack,int defend,int exp=0)
    :VirtualObject(x,y,symbol,name,money,hp,attack,defend,exp){}
    void Display(){
        printf("位置x：%d，位置y:%d，头像：%s,姓名：%s 金币：%d\n 生命：%d 攻击力：%d 防御力：%d 经验：%d\n",X,Y,
        Symbol.c_str(),Name.c_str(),Money,HP,Attack,Defend,Exp);
    }
    void Display(initializer_list<string> alist);

    void Save(std::fstream& f) const ; 

    void Load(std::fstream& f)  ;
   
};


class TreasureBox : public Point {
public:
    TreasureBox(){}
    DEFINE_SETGET(coins,int)
    TreasureBox(int x, int y,const string& symbol):Point(x,y),msymbol(symbol){}
    ~TreasureBox() = default;
    string& GetSysmbol(){return msymbol;}
    // bool isOpened() const { return opened; }
    // void open(Player& player);
    void generateContents(const Shop& shop);
    PropPtr getRandomProp(const Shop& shop);
    void Load(std::fstream& f);

    vector<PropPtr>& GetContents(){return contents;};


private:
    string msymbol;
    std::vector<PropPtr> contents;
    int coins;
};

using STreasureBox=shared_ptr<TreasureBox>;




class BuildMonseter{
public:
    virtual shared_ptr<Monster> GetMonster()=0;
};

class BuildGBL:public BuildMonseter{
public: 
    BuildGBL(){
        m_monster=make_shared<Monster>(0,0,"🤖","格林不",10,50,10,5,5);
    }
    shared_ptr<Monster> GetMonster(){
        return m_monster;
    }
private:
    shared_ptr<Monster>m_monster;
};
class BuildGTQS:public BuildMonseter{
public: 
    BuildGTQS(){
        m_monster=make_shared<Monster>(0,0,"💀","骨头骑士",30,100,20,10,15);
    }
    shared_ptr<Monster> GetMonster(){
        return m_monster;
    }
private:
    shared_ptr<Monster>m_monster;
};

class BuildLW:public BuildMonseter{
public: 
    BuildLW(){
        m_monster=make_shared<Monster>(0,0,"👹","龙王",50,200,50,40,50);
    }
    shared_ptr<Monster> GetMonster(){
        return m_monster;
    }
private:
    shared_ptr<Monster>m_monster;
};




using SMonster=shared_ptr<Monster>;
using SPlayer=shared_ptr<Player>;


class Scence{
public:
    DEFINE_SETGET(shop,Shop);
    std::pair<int, int> GetRandPostion();
    std::pair<int, int> GetRandPostionTreasue();
    void InintMonsters();
    void InintPlayer();
    void InintVTreasureBox();
    void ScenceDisplay(int flag=0);
    void MenuDisplay();
    void GUIRun();
    void PersonalSet();
    void MoveTreasueBox(STreasureBox& box);
    void MoveMonsters(SMonster& box);
    variant<STreasureBox,SMonster,bool> IsPositonEqual( SPlayer& player);
    friend class Menu;
    void SaveMonV(std::fstream &f);
    void LoadMonv(std::fstream &f);
    void SaveVTreaBox(std::fstream &f);
    void LoadVTreaBox(std::fstream &f);
    static Scence* GetSingScence();
    static void Delete();
    void Clear();
    int FightTOFight(SMonster &mster,int&sumPlayATT,int &summsterATT );
private:
    static Scence* scen;
    Scence();
    Scence(const Scence& scen)=delete;
    Scence& operator=(const Scence& scen)=delete;
    string Map[MapSize][MapSize];
    vector<SMonster> m_monster;
    vector<STreasureBox>mVTreasureBox; 
    SPlayer player;
    Shop shop;
};




class Tools{
public:
  static void ProgressBar(const string& str="加载中");
  static void Delaymoment(int ms=2000);
  static void printFormattedOutput(int totalWith, int flag,initializer_list<any>params);
  static void printProgressBar(int totalBytes, char pattern, int current, int total);
};




class Observer{
public:
    virtual void update(const std::string& news) = 0;
};

using SScence=shared_ptr<Scence>;

class Menu:public Observer{
public:
    Menu(SScence pscence):mpscence(pscence){}
    void MenuDisplay();
    void Play();
    void Continue();
    void Archive();
    void EntityDisplay();
    void exit();
    virtual void update(const std::string& news){
        name=news;
    }
private:
    string name;
    SScence mpscence;
};


class Subject{
public:
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notifyObservers(const std::string& news) = 0;
};

class Log:public Subject{
public:
    void Display();
    void Register();
    void Login();
    void Continuegame();
    void Read(const std::string& str());
    virtual void attach(Observer* observer){Obv.push_back(observer);};
    virtual void detach(Observer* observer){
        if(Obv.size()!=0){
            for(auto ite=Obv.begin();ite!=Obv.end();ite++){
                if((*ite)==observer){
                    ite=Obv.erase(ite);
                }
            }
        }
    };

    virtual void notifyObservers(const std::string& news){
        for(auto ob:Obv){
            ob->update(news);
        }
    }
private:
    map<string,vector<PropPtr>> goods;
    vector<SMonster> m_monster;
    SPlayer player;
    vector<Observer*> Obv;
};

