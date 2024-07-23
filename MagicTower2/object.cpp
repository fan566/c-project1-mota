
#include"object.h"
#include<cstdlib>
#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<iomanip>
#include <thread>
#include <chrono>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;
Scence* Scence::scen=nullptr;
std::atomic<bool> should_exit(false);
int prid=1;
Shop::Shop()
{
    vector<PropPtr> v={PropPtr(PropFactory::GetWeapon("破军",30,Type::WEAPEON,State::SALEING,15)),PropPtr(PropFactory::GetWeapon("无尽之刃",30,Type::WEAPEON,State::SALEING,15)),PropPtr(PropFactory::GetWeapon("饮血剑",30,Type::WEAPEON,State::SALEING,15))};
    goods.insert({"Weapon",v});
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][0].get())->SetmCritAttack(10);
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][0].get())->SetmlifeSteal(10);
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][1].get())->SetmCritAttack(20);
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][1].get())->SetmlifeSteal(20);
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][2].get())->SetmCritAttack(30);
    // dynamic_cast<WeaponEnhance*>(goods["Weapon"][2].get())->SetmlifeSteal(30);
    vector<PropPtr> v2={PropPtr(PropFactory::GeMedicine("小药丸",20,Type::MEDICINE,State::SALEING,10)),PropPtr(PropFactory::GeMedicine("大药丸",20,Type::MEDICINE,State::SALEING,10)),PropPtr(PropFactory::GeMedicine("激素",20,Type::MEDICINE,State::SALEING,10))};
    goods.insert({"Medicine",v2});
    vector<PropPtr> v1={PropPtr(PropFactory::GetDefencer("破甲",30,Type::DEFNECER,State::SALEING,20)),PropPtr(PropFactory::GetDefencer("冰寒之盾",30,Type::DEFNECER,State::SALEING,20)),PropPtr(PropFactory::GetDefencer("红莲斗篷",30,Type::DEFNECER,State::SALEING,20))};
    goods.insert({"Defencer",v1});
}

void WeaPon::Display()
{
    // Tools::printFormattedOutput(50,0,{mprid,Name,Money,Attack});
    cout<<left<<setw(10)<<setfill(' ')<<mprid
    <<left<<setw(15)<<setfill(' ')<<Name
    <<left<<setw(10)<<setfill(' ')<<Money
    <<left<<setw(10)<<setfill(' ')<<Attack<<endl;
}

void WeaPon::Save(std::fstream &f) 
{
    f << mprid << " " << Name << " " << Money << " " << static_cast<int>(m_type) << " " << static_cast<int>(m_state) << " " << Attack << std::endl;
}
void Shop::Save(std::fstream& f) const {
        f << goods.size() << std::endl;
        for (const auto& pair : goods) {
            f << pair.first << " " << pair.second.size() << std::endl;
            for (const auto& prop : pair.second) {
                prop->Save(f);
            }
        }
}


void Shop::Load(std::fstream &f)
{
    int goodsSize;
    f >> goodsSize;
    goods.clear();
    for (int i = 0; i < goodsSize; ++i) {
        std::string category;
        int categorySize;
        f >> category >> categorySize;
        std::vector<PropPtr> props;
        Prop* prop = nullptr;
        for (int j = 0; j < categorySize; ++j) {
            // std::string propType;
            // getline(f, propType, ' '); // 读取道具类型
            if (category == "Weapon") {
                prop = new WeaPon;
            } else if (category == "Defencer") {
                prop = new Defencer;
            } else if (category == "Medicine") {
                prop = new Medicine;
            }
            if (prop) {
                prop->Load(f);
                shared_ptr<Prop> sprop(prop);
                props.emplace_back(sprop);
            }
        }
        goods[category]=props;
    }
}

void WeaPon::Load(std::fstream &f)
{
    int typeInt, stateInt;
    f >> mprid >> Name >> Money >> typeInt >> stateInt >> Attack;
    m_type = static_cast<Type>(typeInt);
    m_state = static_cast<State>(stateInt);
}

void Defencer::Display()
{
    cout<<left<<setw(10)<<setfill(' ')<<mprid
    <<left<<setw(15)<<setfill(' ')<<Name
    <<left<<setw(10)<<setfill(' ')<<Money
    <<left<<setw(10)<<setfill(' ')<<Defend<<endl;

}

void Defencer::Save(std::fstream &f) 
{
    f << mprid << " " << Name << " " << Money << " " << static_cast<int>(m_type) << " " << static_cast<int>(m_state) << " " << Defend << std::endl;
}

void Defencer::Load(std::fstream &f)
{
    int typeInt, stateInt;
    f >> mprid >> Name >> Money >> typeInt >> stateInt >> Defend;
    m_type = static_cast<Type>(typeInt);
    m_state = static_cast<State>(stateInt);
}

void Medicine::Display()
{
    cout<<left<<setw(10)<<setfill(' ')<<mprid
    <<left<<setw(10)<<setfill(' ')<<Name
    <<left<<setw(10)<<setfill(' ')<<Money
    <<left<<setw(10)<<setfill(' ')<<Value<<endl;
}

void Medicine::Save(std::fstream &f) 
{
    f << mprid << " " << Name << " " << Money << " " << static_cast<int>(m_type) << " " << static_cast<int>(m_state) << " " << Value << std::endl;
}
void Medicine::Load(std::fstream &f)
 {
    int typeInt, stateInt;
    f >> mprid >> Name >> Money >> typeInt >> stateInt >> Value;
    m_type = static_cast<Type>(typeInt);
    m_state = static_cast<State>(stateInt);
}
Scence::Scence()
{
    flag=nullptr;
    InitAll();
}
void Scence::InitAll()
{
    for(int i=0;i<MapSize;i++){
        for(int j=0;j<MapSize;j++){
            Map[i][j]="🔳";
        }
    }
    InintPlayer();
    InintVTreasureBox();
    InintMonsters();
    int a;
    flag=&a;
}



std::pair<int, int> Scence::GetRandPostion()
{
    while(1){
        int x=rand()%(MapSize);
        int y=rand()%(MapSize);
        int flag=0;
        for(int i=0;i<m_monster.size();i++){
            if((m_monster[i]->GetX()==x&&m_monster[i]->GetY()==y)||(0==x&&0==y)){
                flag=1;
            }
        }
        if(flag==1){
            continue;
        }
        return {x,y}; 
    }
}

void Scence::Clear()
{
     for(int i=0;i<MapSize;i++){
        for(int j=0;j<MapSize;j++){
            Map[i][j]="🔳";
        }
    }
}

std::pair<int, int> Scence::GetRandPostionTreasue()
{
   while(1){
        int x = rand() % MapSize;
        int y = rand() % MapSize;
        
        // 检查是否为 (0,0) 坐标
        if (x == 0 && y == 0) {
            continue;
        }
        
        bool isOverlap = false;
        
        // 检查是否与其他宝箱重叠
        for (const auto& box : mVTreasureBox) {
            if (box->GetX() == x && box->GetY() == y) {
                isOverlap = true;
                break;
            }
        }
        
        // 检查是否与怪物重叠
        for (const auto& monster : m_monster) {
            if (monster->GetX() == x && monster->GetY() == y) {
                isOverlap = true;
                break;
            }
        }
        
        // 如果没有重叠，返回这个坐标
        if (!isOverlap) {
            return {x, y};
        }
    }
}


void Scence::InintMonsters()
{
    int i=0;
    while(i<MONSTER_SIZE){
        if(i<MONSTER_SIZE/6*3){
            BuildMonseter* bmonster=new BuildGBL;
            m_monster.push_back( bmonster->GetMonster());
            pair<int,int> p=GetRandPostion();
            m_monster.back()->SetX(p.first);
            m_monster.back()->SetY(p.second);
            i++;
        }else if(i<MONSTER_SIZE/6*5){
            BuildMonseter* bmonster=new BuildGTQS;
            m_monster.push_back( bmonster->GetMonster());
            pair<int,int> p=GetRandPostion();
            m_monster.back()->SetX(p.first);
            m_monster.back()->SetY(p.second);
            i++;
        }else{
            BuildMonseter* bmonster=new BuildLW;
            m_monster.push_back( bmonster->GetMonster());
            pair<int,int> p=GetRandPostion();
            m_monster.back()->SetX(p.first);
            m_monster.back()->SetY(p.second);
            i++;
        }
    }
}

void Scence::InintPlayer()
{
    BuildPlayer* bplayer=new BuildCPlayer;
    player=bplayer->GetPlayer();
}

void Scence::InintVTreasureBox()
{
//    for(int i=0;i<TreasureBoxCount;i++){
//         pair<int ,int> postion= GetRandPostion();
//         mVTreasureBox.push_back(make_shared<TreasureBox>(postion.first,postion.second,"🎁"));
//         mVTreasureBox.back()->generateContents(shop);
//    }
   for(int i = 0; i < TreasureBoxCount; i++){
        pair<int, int> position = GetRandPostionTreasue();
        mVTreasureBox.push_back(make_shared<TreasureBox>(position.first, position.second, "🎁"));
        mVTreasureBox.back()->generateContents(shop);
   }
}
int Scence::CheckMonstersCount()
{
    return count_if(m_monster.begin(),m_monster.end(),[]( auto & mster){
        return mster->GetHP()==0;
    });
}

void Scence::RefleshMonster()
{
    for(auto & mster:m_monster){
        mster->Reflesh();
        pair p= GetRandPostion();
        mster->GetX()=p.first;
        mster->GetY()=p.second;
    }
    
    
}



void Scence::ScenceDisplay(int flag)
{
    Clear();
    int count= CheckMonstersCount();
    if(count==m_monster.size()){
        RefleshMonster();
    }

    Map[player->GetY()][player->GetX()]=player->GetSymbol();
    for(int i=0;i<mVTreasureBox.size();i++){
        if(mVTreasureBox[i]->Getcoins()>0)
        
        Map[mVTreasureBox[i]->GetY()][mVTreasureBox[i]->GetX()]=mVTreasureBox[i]->GetSysmbol();
    }
    for(int i=0;i<MONSTER_SIZE;i++){
        if(m_monster[i]->GetHP()>0)
        Map[m_monster[i]->GetY()][m_monster[i]->GetX()]=m_monster[i]->GetSymbol();
    }
    if(flag)
    Map[player->GetY()][player->GetY()]="⚔";
    for(int i=0;i<MapSize;i++){
        for(int j=0;j<MapSize;j++){
            printf("%-5s",Map[i][j].c_str());
        }
        cout<<endl;
    }
    Tools::printFormattedOutput(80,1,{"w.向上","s.向下","a.向左","d.向右","v.背包","g.商店","q.退出"});
    // printf("w.向上   s.向下   a.向左   d.向右   v.背包  g.商店    q.推出\n");
    Tools::printFormattedOutput(80,0,{"SYMBOL","NAME","HP","MONEY","ATTACK","DEEFER","EXP","LEVEL"});
    player->DisPlayList({"SYMBOL","NAME","HP","MONEY","ATTACK","DEFEND","EXP","LEVEL"});
}

void Scence::GUIRun()
{
    while(1){ 
        system("clear");
        ScenceDisplay();
        char c;
        cin>>c;
        player->PlayerMove(c);
        switch (c)
        {
        case 'v':
            player->Getbag().Display();
            break;
        case 'g':
            shop.Display(*(player.get()));
            break;
        case 'q':

            return ;
        case 'p':
        
            return ;
        default:
            break;
        }
        system("clear");
        ScenceDisplay();
        variant<STreasureBox,SMonster,bool> TreboxOrMSter= IsPositonEqual(player);
        while(!holds_alternative<bool>(TreboxOrMSter)){
            if(holds_alternative<STreasureBox>(TreboxOrMSter)){
                MoveTreasueBox(get<STreasureBox>(TreboxOrMSter));
            }
            if(holds_alternative<SMonster>(TreboxOrMSter)){
                MoveMonsters(get<SMonster>(TreboxOrMSter));
            }
            TreboxOrMSter=false;
        }
        
    }
}

void Scence::MoveTreasueBox(STreasureBox& box)
{
    
    Tools::ProgressBar("打开宝箱中");
    player->GetMoney()+=box->Getcoins();
    Tools::printFormattedOutput(60,10,{"coin",box->Getcoins()});
    box->Getcoins()=0;
    

    for(auto it:box->GetContents()){
        Tools::printFormattedOutput(60,0,{it->GetName().c_str()});
        if(!player->Getbag().Insert(it)){
            player->GetMoney()+=it->GetMoney();
            string str=it->GetName()+"已存在,置换为金币： " +to_string(it->GetMoney());
            Tools::printFormattedOutput(60,1,{str.c_str()});
        }
    }

    Tools::Delaymoment(2000);
    box->GetContents().clear();
   
}


void Scence::FightTread()
{
    
}

void Scence::CinTread()
{
}

void Scence::MoveMonsters(SMonster &mster)
{
    int playHP=player->GetHP();
    int msterHP=mster->GetHP();
    int  sumPlayATT=0;
    int summsterATT=0;
    while(1){
        system("clear");
        ScenceDisplay(1);
        // Tools::printFormattedOutput(80,0,{"SYMBOL","NAME","HP","MONEY","ATTACK","DEEFER","EXP","LEVEL"});
        // player->DisPlayList({"SYMBOL","NAME","HP","MONEY","ATTACK","DEFEND","EXP","LEVEL"});
        Tools::printFormattedOutput(80,0,{"SYMBOL","NAME","HP","MONEY","ATTACK","DEFEND","EXP"," "});
        mster->Display({"SYMBOL","NAME","HP","MONEY","ATTACK","DEFEND","EXP"});
        int result=FightTOFight(mster,sumPlayATT,summsterATT);
        if(result==1){
            Tools::printFormattedOutput(80,2,{"player fail! ! ！"});
            Tools::Delaymoment(2000);
            break;
        }else if(result==0){
            Tools::printFormattedOutput(80,2,{"player SUCCESS, ! ! ！"});
            Tools::Delaymoment(2000);
            break;
        }else{
            
        }
        Tools::printFormattedOutput(80,2,{player->GetSymbol(),"v  s",mster->GetSymbol()});
        Tools::printProgressBar(80,'#',playHP-summsterATT,playHP);
        Tools::printProgressBar(80,'#',msterHP-sumPlayATT,msterHP);
        Tools::Delaymoment(2000);
    }
}
int Scence::FightTOFight(SMonster &mster,int&sumPlayATT,int &summsterATT )
{
    int playrAttack=std::max(0,player->GetAttack()+player->Getbag().GetmAttactAmount()-mster->GetDefend());
    int monsterAttack=std::max(0,mster->GetAttack()-player->GetDefend()-player->Getbag().GetmDefendAmount());
    summsterATT+=monsterAttack;
    sumPlayATT+=playrAttack;
    if((mster->GetHP()-=playrAttack)<=0){
        player->GetMoney()+=mster->GetMoney();
        mster->GetHP()=0;
        return 0;
    }
    if((player->GetHP()-=monsterAttack)<=0){
        player->GetHP()=0;
        return 1;
    }
    return 2;
}


variant<STreasureBox, SMonster,bool> Scence::IsPositonEqual(SPlayer &player)
{
    for(auto it:m_monster){
        if(it->GetX()==player->GetX()&&it->GetY()==player->GetY()&&it->GetHP()>0){
            return it;
            break;
        }
    }
    for(auto it:mVTreasureBox){
        if(it->GetX()==player->GetX()&&it->GetY()==player->GetY()&&it->Getcoins()>0){
            return it;
        }
    }
    return false;
}

void Scence::SaveMonV(std::fstream &f)
{
    f<<m_monster.size()<<endl;
    for(int i=0;i<m_monster.size();i++){
        m_monster[i]->Save(f);
    }
}

void Scence::LoadMonv(std::fstream &f)
{
    m_monster.clear();
    int monsterSize;
    f>>monsterSize;
    for(int i=0;i<monsterSize;i++){
        SMonster pmonster=make_shared<Monster>();
        pmonster->Load(f);
        m_monster.emplace_back(pmonster);
    }
}

void Scence::SaveVTreaBox(std::fstream &f)
{
    f << mVTreasureBox.size() << endl;
    for (const auto& box :mVTreasureBox) {
        f << box->GetX() << " " << box->GetY() << " " << box->Getcoins() << endl;
        f << box->GetContents().size() << endl;
        for (const auto& prop : box->GetContents()) {
            f<<prop->Getmprid()<<" ";
        }
        f<<endl;
    }
}

void Scence::LoadVTreaBox(std::fstream &f)
{
    mVTreasureBox.clear();
    int treaBoxSize;
    f>>treaBoxSize;
    auto& goods =Scence::GetSingScence()->Getshop().GetMap();
    for(int i=0;i<treaBoxSize;i++){
        STreasureBox box=make_shared<TreasureBox>();
       mVTreasureBox.emplace_back(box)->Load(f);
    }
}

Scence *Scence::GetSingScence()
{
    if(scen==nullptr){
        scen=new Scence();
        return scen;
    }else{
        return scen;
    }   
}

void Scence::Delete()
{
    if(scen!=nullptr){
        delete scen;
    }
}


void Shop::Display(Player&player)
{
    while(1){
        system("clear");
        printf("================================商店===============================\n");
        for(auto it:goods){
            int flag=1;
            for(auto ite:it.second){
                if(flag){
                    if(typeid(*(ite.get()))==typeid(WeaPon)){
                        Tools::printFormattedOutput(50,0,{"TYPE","id","Name","Money","Attact"});
                        // cout<<setw(12)<<left<<"TYPE"<<setw(10)<<left<<"id"<<setw(10)<<left<<"Name"<<setw(10)<<left<<"Money"<<setw(10)<<left<<"Attact"<<endl;
                    }
                    // if(typeid(*(ite.get()))==typeid(WeaponEnhance)){
                    //     cout<<setw(12)<<left<<"TYPE"<<setw(10)<<left<<"id"<<setw(10)<<left<<"Name"
                    //     <<setw(10)<<left<<"Money"<<setw(10)<<left<<"Attact"<<setw(10)<<left<<"LifeSteal"<<setw(10)<<left<<"CritAttack"<<endl;
                    // }
                    if(typeid(*(ite.get()))==typeid(Defencer)){
                       Tools::printFormattedOutput(50,0,{"TYPE","id","Name","Money","Defencer"});
                    }
                    if(typeid(*(ite.get()))==typeid(Medicine)){
                        Tools::printFormattedOutput(50,0,{"TYPE","id","Name","Money","Medicine"});
                      
                    }

                    flag=0;
                }
                // if(typeid(*(ite.get()))==typeid(WeaponDecoator)){
                //     cout<<left<<setw(12)<<it.first;
                //     ite->Display();
                //     // dynamic_cast<WeaponEnhance*>(ite.get())->Display();
                // }else{d<<setw(12)<<it.first;
                    ite->Display();
                // }              
            }
            cout<<"--------------------------------------------------------------------\n";
        }
        Tools::printFormattedOutput(80,0,{"SYMBOL","NAME","HP","MONEY","ATTACK","DEEFER","EXP","LEVEL"});
        player.DisPlayList({"SYMBOL","NAME","HP","MONEY","ATTACK","DEFEND","EXP","LEVEL"});
        cout<<"请输入ID号购买，按0退出"<<endl;
        int c;
        cin>>c;
        if(c!=0){
            PurchaseGoods(c,player);
        }else{
            return ;
        }
    }
}


void Shop::PurchaseGoods(int id, Player &player)
{
    for(auto it:goods){
        for(auto ite:it.second){
            if(ite->Getmprid()==id){
                if(player.GetMoney()>ite->GetMoney()){
                    if(!player.Getbag().Insert(ite)){
                        Tools::printFormattedOutput(60,1,{"该道具已存在"});
                        Tools::Delaymoment(1000);
                    }else{
                        player.GetMoney()-=ite->GetMoney();
                    }
                }else{
                    Tools::printFormattedOutput(60,1,{"you are too poor,not enough to pay"});
                    Tools::Delaymoment(2000);
                }
            }

        }
    }
}

Bag::Bag()
{
    mLifeAmount=0;
    mDefendAmount=0;
    mAttactAmount=0;
}

void Bag::Discard(int id)
{
    // auto it= find_if(Vbag.begin(),Vbag.end(),
    // [id](const auto& prop){return id==prop->Getmprid()});
    // if(it!=Vbag.end()){
    //     Vbag.e
    // }

    auto it = std::find_if(Vbag.begin(), Vbag.end(),
        [id](const auto& prop) { return prop->Getmprid() == id; });
    if (it != Vbag.end()) {
        if((*it)->Getm_state() == State::LOADING){

            Tools::printFormattedOutput(60,0,{"please unequip this prop"});
            Tools::Delaymoment(1000);
            return ;
        }
        std::swap(*it, Vbag.back());
        Vbag.pop_back();
        std::cout << "Item with ID " << id << " has been removed from the bag." << std::endl;
    } else {
        std::cout << "Item with ID " << id << " was not found in the bag." << std::endl;
    }
    Tools::Delaymoment(1000);
}

bool Bag::Insert(PropPtr & prop)
{
    for(auto& it: Vbag){
        if(prop->Getmprid()==it->Getmprid()){
            return false;
        }
    }
    prop->Setm_state(State::UNLOADING);
    Vbag.push_back(prop);
    return true;
}

void Bag::Equip(int id)
{
    auto it=find_if(Vbag.begin(),Vbag.end(),
    [id](const auto& sprop){
        return  sprop->Getmprid()==id;
    });
    if((*it)->Getm_type()==Type::DEFNECER) mDefendAmount+=dynamic_cast<Defencer*>((*it).get())->GetDefend();
    if((*it)->Getm_type()==Type::WEAPEON) mAttactAmount+=dynamic_cast<WeaPon*>((*it).get())->GetAttack();
    if((*it)->Getm_type()==Type::MEDICINE) mLifeAmount+=dynamic_cast<Medicine*>((*it).get())->GetValue();
    (*(it))->Setm_state(State::LOADING);
}

void Bag::Unquip(int id)
{
    auto it=find_if(Vbag.begin(),Vbag.end(),
    [id](const auto& sprop){
      return  sprop->Getmprid()==id;
    });
    if((*it)->Getm_type()==Type::DEFNECER) mDefendAmount-=dynamic_cast<Defencer*>((*it).get())->GetDefend();
    if((*it)->Getm_type()==Type::WEAPEON) mDefendAmount-=dynamic_cast<WeaPon*>((*it).get())->GetAttack();
    if((*it)->Getm_type()==Type::MEDICINE) mDefendAmount-=dynamic_cast<Medicine*>((*it).get())->GetValue();
    (*(it))->Setm_state(State::UNLOADING);
}

void Bag::Display()
{
    while(1){
        system("clear");
        cout<<"===================背包系统=========================\n";
        cout<<setw(10)<<left<<"id"<<setw(10)<<left<<"Name"<<setw(10)<<left<<"Money"<<setw(10)<<left<<"specialvalue"<<endl;
        for(auto ite:Vbag){
            ite->Display();
            
            // if(typeid(*(ite))==typeid(WeaponDecoator)){
            //     dynamic_cast<WeaponEnhance*>(ite.get())->Display();
            // }
        }

        cout<<"==================已装备===========================\n";
        for(auto ite:Vbag){
            if(ite->Getm_state()==State::LOADING){
                ite->Display();
            }
            // if(typeid(*(ite))==typeid(WeaponDecoator)){
            //     dynamic_cast<WeaponEnhance*>(ite.get())->Display();
            // }
        }
        cout << "=============道具属性加成==========================\n";
        cout << "|Attack: " <<setw(10) << mAttactAmount 
        << "Defend: " <<setw(10) << mDefendAmount 
        << setw(12) << "Life : " <<right<<mLifeAmount << "|\n";
        cout << "===================================================\n";
        cout<<"1.丢弃   2.装备   3.卸载   4.退出"<<endl;
        char c;
        cin>>c;
        char id;
        switch (c-'0')
        {
        case 1:
            cout<<"输入id:  ";
            fflush(stdout);
            cin>>id;
            Discard(id-'0');
            break;
        case 2:
            cout<<"输入id:  ";
            fflush(stdout);
            cin>>id;
            Equip(id-'0');
            break;
        case 3:
            cout<<"输入id:  ";
            fflush(stdout);
            cin>>id;
            Unquip(id-'0');
            break;
        case 4:
            Tools::ProgressBar("正在退出背包系统");
            return ;
        default:
            break;
        }
    }
}

void Bag::Save(std::fstream &f) const
 {
    f << Vbag.size() << std::endl;
    for (const auto& prop : Vbag) {
        f << prop->Getmprid() << " "; 
    }
    f << std::endl;
    f <<mAttactAmount<<" "<<mDefendAmount<<" "<<mLifeAmount<<endl;
}

void Bag::Load(std::fstream &f,Shop& shop)
{
    int size;
    f >> size;
    Vbag.clear();
    for (int i = 0; i < size; ++i) {
        int propId;
        f >> propId;
        // 根据 propId 从某个地方（例如全局道具列表）加载道具
        auto& goods=shop.GetMap();
        for(auto& it:goods){
            for(auto& ite:it.second){
                if(ite->Getmprid()==propId){
                    Vbag.emplace_back(ite);  
                    break;
                }
            }
        }
        
    }
    f>>mAttactAmount>>mDefendAmount>>mLifeAmount;
}

void Log::Display()
{
    system("clear");
    std::cout << "==============================================" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "|          Welcome to MagicTower             |" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "|           1.    Login                      |" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "|           2.    Register                   |" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "|           3.    Exit                       |" << std::endl;
    std::cout << "|                                            |" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Please enter your choice: \n";
    char c;
    cin>>c;
    switch (c-'0')
    {
    case 1:Login();
        break;
    case 2:Register();
        break;
    case 3:exit(-1);
        break;
    default :
        break;
    }
}



void Log::Register()
{
    cout<<"请输入你的姓名：";
    fflush(stdout);
    string str1;
    cin>>str1;
    cout<<"请输入你密码：";
    fflush(stdout);
    string str2;
    cin>>str2;
    fstream f("./文档/玩家信息.txt",ios::out|ios::app);
    if(!f.is_open()){
        cout<<"文件打开失败！";
        exit(1);
    }
    f<<str1<<" "<<str2<<endl;
    f.close();
    Tools::ProgressBar("正在注册中");
    return ;
}

void Log::Login()
{
    while (1)
    {
        cout << "请输入你的姓名：";
        fflush(stdout);
        string str1;
        cin >> str1;
        cout << "请输入你的密码：";
        fflush(stdout);
        string str2;
        cin >> str2;
        fstream f("./文档/玩家信息.txt", ios::in);
    
        string str3;
        string fstr1;
        string fstr2;
        while (getline(f, str3))
        {
            istringstream iss(str3);
            iss >> fstr1 >> fstr2;
            if (str1 == fstr1 && str2 == fstr2)
            {
                notifyObservers(str1);
                Tools::ProgressBar("正在进入游戏");
                return;
            }
        }
        f.close();
        cout << "重新输入：-----按q退出！" << endl;
        char c;
        cin >> c;
        if (c == 'q')
            Display();
    }
}

void Log::Continuegame()
{

}



void Tools::ProgressBar(const string& str)
{
    std::cout << std::setw((46-str.size()/2)/2) << std::setfill('-') << "" // 左填充
              << str // 打印字符串
              <<  std::setw((46-str.size())/2) << std::setfill('-') << "" // 右填充
              << std::endl;
    for(int i=0;i<44;i++){
        std::cout<<"=";
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    cout<<endl;
}

void Tools::Delaymoment(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Tools::printFormattedOutput(int totalWidth, int flag,initializer_list<any> params)
{
    int totalCount = params.size();
    int size = totalWidth / totalCount;
    if(flag==1)
    std::cout << std::string(totalWidth + 2, '=') << std::endl;
    if(flag==2)
    std::cout << std::string(totalWidth + 2, '-') << std::endl;
    if(flag)
    std::cout << '|';
    for (const auto& it : params) {
        std::string value;
        if (it.type() == typeid(const char*)) {
            value = std::any_cast<const char*>(it);
        } else if (it.type() == typeid(int)) {
            value = std::to_string(std::any_cast<int>(it));
        }else if (it.type() == typeid(string)) {
            value = (std::any_cast<string>(it));
        }else if (it.type() == typeid(double)) {
            value = std::to_string(std::any_cast<double>(it));
        } else {
            value = "Unknown type";
        }

        if (value.length() > size - 1) {
            value = value.substr(0, size - 4) + "...";
        }

        std::cout << std::left <<setfill(' ') <<std::setw(size)<< value;
    }
    if(flag)
    std::cout << '|' ;
    std::cout<< std::endl;
    if(flag==1)
    std::cout << std::string(totalWidth + 2, '=') << std::endl;
    if(flag==2)
    std::cout << std::string(totalWidth + 2, '-') << std::endl;
}

void Tools::printProgressBar(int totalBytes, char pattern, int current, int total)
{
    if (total <= 0 || current < 0 || current > total) {
        std::cerr << "Invalid input parameters" << std::endl;
        return;
    }

    int progressChars = static_cast<int>((static_cast<double>(current) / total) * totalBytes);
    
    std::cout << "[";
    
    // 打印进度字符
    for (int i = 0; i < progressChars; ++i) {
        std::cout << pattern;
    }
    
    // 打印剩余空白
    for (int i = progressChars; i < totalBytes; ++i) {
        std::cout << " ";
    }
    
    std::cout << "] ";
    

    // 打印百分比
    double percentage = (static_cast<double>(current) / total) * 100;
    std::cout << std::fixed << std::setprecision(2) << percentage << "%\n";
    std::cout.flush();
}

void Menu::MenuDisplay()
{
    while(1){
        system("clear");
        std::cout << "==============================================" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|              MenuDisplay                   |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "==============================================" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|           0.    Play                       |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|           1.    Continue                   |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|           2.    Archive                    |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|           3.    exit                       |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << "Please enter your choice: \n"; 
        char c;
        cin>>c;
        switch (c-'0')
        {
        case 0:Play();
            return ;
            break;
        case 1:Continue();
            /* code */
            return ;
            break;
        case 2:Archive();
            /* code */
            break;
        // case 3:EntityDisplay();
        //     /* code */
        //     break;
        case 3:
            std::exit(0);
            /* code */
            break;
        
        default:
            break;
        }
    }
}

void Menu::Play()
{
    // Scence::scen->InitAll();
}

void Menu::Continue()
{
    string dir = "./文档/" + name;
    if (system(("mkdir -p " + dir).c_str()) != 0) {
        perror ( "无法创建目录" );
        return;
    }

    fstream f1(dir + "/player", ios::in);
    fstream f2(dir + "/monster", ios::in);
    fstream f3(dir + "/goods", ios::in);
    fstream f4(dir + "/treasurebox", ios::in);


    if (!f1.is_open() || !f2.is_open() || !f3.is_open() || !f4.is_open()) {
        cerr << "无法打开文件" << endl;
        return;
    }

    mpscence->player->Load(f1,mpscence->shop);
    mpscence->LoadMonv(f2);
    // mpscence->shop.Save(f3);
    mpscence->LoadVTreaBox(f4);

    f1.close();
    f2.close();
    f3.close();
    f4.close();
    // 启动游戏
    
}

void Menu::Archive()
{

    // int flag=1;
    // int result= system(("mkdir ./文档/"+name).c_str());
    // if(result==-1)flag=0;
    // fstream f1("./文档/"+name+"/player",ios::out);
    // if(!f1.is_open()){
    //     cerr<<"无法打开文件";
    // }
    // fstream f2("./文档/"+name+"/monster",ios::out);
    // if(!f1.is_open()){
    //     cerr<<"无法打开文件";
    // }
    // fstream f3("./文档/"+name+"/goods",ios::out);
   
    // if(!f1.is_open()){
    //     cerr<<"无法打开文件";
    // }
    // fstream f4("./文档/"+name+"/treasurebox", ios::out);
    // if(!f4.is_open()){
    //     cerr<<"无法打开文件";
    // }
    // mpscence->player->Save(f1);
    // mpscence->shop.Save(f3);
    // mpscence->SaveMonV(f2);
    // f1.close();
    // f2.close();
    // f3.close();
    // if(flag)
    // Tools::ProgressBar("正在保存中");
    // else
    // Tools::Delaymoment();


    // if(mpscence->flag==nullptr){
    //     Tools::printFormattedOutput(80,0,{"未有玩家数据更新，请进入新游戏"});
    //     Tools::Delaymoment(1000);
    //     return ;
    // }
    string dir = "./文档/" + name;
    if (system(("mkdir -p " + dir).c_str()) != 0) {
        perror ( "无法创建目录" );
        return;
    }

    fstream f1(dir + "/player", ios::out);
    fstream f2(dir + "/monster", ios::out);
    fstream f3(dir + "/goods", ios::out);
    fstream f4(dir + "/treasurebox", ios::out);


    if (!f1.is_open() || !f2.is_open() || !f3.is_open() || !f4.is_open()) {
        cerr << "无法打开文件" << endl;
        return;
    }

    mpscence->player->Save(f1);
    mpscence->SaveMonV(f2);
    mpscence->shop.Save(f3);
    mpscence->SaveVTreaBox(f4);

    f1.close();
    f2.close();
    f3.close();
    f4.close();

    Tools::ProgressBar("游戏数据存储中");

}

void Menu::EntityDisplay()
{
//     fstream ifs("./文档/player");
//     mpscence->player->Load(ifs);
//     mpscence->player->Display();
//     ifs.close(); 
//     fstream ifm("./文档/monsters");
//     mpscence->LoadMonv(ifm);
//     for(auto& it:mpscence->m_monster){
//         it->Display();
//     }
//     ifm.close(); 
//     getchar();
//     getchar();
    // fstream ifg("./文档/goods",ios::out);
    // mpscence->shop.Load(ifg);
    // Player player=*(mpscence->player);
    // mpscence->shop.Display(player);
    // ifs.close(); 
}

void Menu::exit()
{
}



void Player::PlayerMove(char select)
{   
    switch (select)
    {
        case 'w':SetY(GetY()-1);break;
        case 's':SetY(GetY()+1);break;
        case 'a':SetX(GetX()-1);break;
        case 'd':SetX(GetX()+1);break;
    }
    if(GetX()<0)SetX(0);
    if(GetY()<0)SetY(0);
    if(GetX()>=MapSize)SetX(MapSize-1);
    if(GetY()>=MapSize)SetY(MapSize-1);
}

void Player::DisPlayList(initializer_list<string> alist)
{
    initializer_list<any>list;
    for(auto& it:alist){
        if("X"==any_cast<string>(it)){
            cout<<setw(13)<<left<<setfill(' ')<<X;
        }
        if("Y"==any_cast<string>(it)){
            cout<<setw(12)<<left<<setfill(' ')<<Y;
        }
        if("NAME"==any_cast<string>(it)){
            cout<<setw(12)<<left<<setfill(' ')<<Name;
        }
        if("SYMBOL"==any_cast<string>(it)){
            cout<<setw(13)<<left<<setfill(' ')<<Symbol;
        }
        if("HP"==any_cast<string>(it)){
            cout<<setw(10)<<left<<setfill(' ')<<HP;
        }
        if("MONEY"==any_cast<string>(it)){
            cout<<setw(10)<<left<<setfill(' ')<<Money;
        }
        if("ATTACK"==any_cast<string>(it)){
            cout<<setw(12)<<left<<setfill(' ')<<Attack;
        }
        if("DEFEND"==any_cast<string>(it)){
            cout<<setw(10)<<left<<setfill(' ')<<Defend;
        }
        if("EXP"==any_cast<string>(it)){
            cout<<setw(10)<<left<<setfill(' ')<<Exp;
        }
        if("LEVEL"==any_cast<string>(it)){
            cout<<setw(10)<<left<<setfill(' ')<<Level;
        }
    }
    cout<<endl;
}

void Player::Save(std::fstream &f) const
{
    
    f << X << " " << Y << " "
        << Symbol << " " << Name << " "
        << Money << " " << HP << " "
        << Attack << " " << Defend << " "
        << Exp << " " << Level << std::endl;
    bag.Save(f);
}

void Player::Load(std::fstream &f,Shop& shop)
{
    f >> X >> Y >> Symbol >> Name >> Money >> HP >> Attack >> Defend >> Exp >> Level;
    bag.Load(f,shop);
}

void Monster::Display(initializer_list<string> alist)
{
         initializer_list<any>list;
        for(auto& it:alist){
            if("X"==any_cast<string>(it)){
                cout<<setw(12)<<left<<setfill(' ')<<X;
            }
            if("Y"==any_cast<string>(it)){
                cout<<setw(12)<<left<<setfill(' ')<<Y;
            }
            if("NAME"==any_cast<string>(it)){
                cout<<setw(14)<<left<<setfill(' ')<<Name;
            }
            if("HP"==any_cast<string>(it)){
                cout<<setw(12)<<left<<setfill(' ')<<HP;
            }
            if("SYMBOL"==any_cast<string>(it)){
                cout<<setw(13)<<left<<setfill(' ')<<Symbol;
            }
            if("MONEY"==any_cast<string>(it)){
                cout<<setw(11)<<left<<setfill(' ')<<Money;
            }
            if("ATTACK"==any_cast<string>(it)){
                cout<<setw(10)<<left<<setfill(' ')<<Attack;
            }
            if("DEFEND"==any_cast<string>(it)){
                cout<<setw(8)<<left<<setfill(' ')<<Defend;
            }
            if("EXP"==any_cast<string>(it)){
                cout<<setw(12)<<left<<setfill(' ')<<Exp;
            }
            // if("LEVLE"==any_cast<string>(it)){
            //     cout<<setw(12)<<left<<setfill(' ')<<Level;
            // }
        }
        cout<<endl;
    }

void Monster::Save(std::fstream &f) const
{
    f << X << " " << Y << " "
        << Symbol << " " << Name << " "
        << Money << " " << HP << " "
        << Attack << " " << Defend << " "
        << Exp << std::endl;
}

void Monster::Load(std::fstream &f)
{
    f >> X >> Y >> Symbol >> Name >> Money >> HP >> Attack >> Defend >> Exp;
}

// void WeaponEnhance::Display()
// {
//     WeaponDecoator::Display();
//     cout<<setw(10)<<left<<mlifeSteal<<setw(10)<<left<<mCritAttack<<endl;
// }

void TreasureBox::generateContents(const Shop& shop)
{
    // opened=false;
    coins=rand()%100+1;
    int propCount=rand()%BOX_CONTENT_SIZE+1;
    for(int i=0;i<propCount;i++){
        contents.push_back(getRandomProp(shop));
    }
}

PropPtr TreasureBox::getRandomProp(const Shop &shop)
{
    // auto goodsMap = shop.GetMap();
    // auto it = goodsMap.begin();
    // std::advance(it, rand() % goodsMap.size());
    
    // const auto& propList = it->second;
    // return propList[rand() % propList.size()];

    auto goodsMap=shop.GetMap();
    auto it=goodsMap.begin();
    advance(it,rand()%goodsMap.size());
    const auto& it1=it->second;
    return it1[rand()%it1.size()];
}

void TreasureBox::Load(std::fstream &f)
{
    auto& goods =Scence::GetSingScence()->Getshop().GetMap();
    f>>X;
    f>>Y;
    f>>coins;
    GetSysmbol()="🎁";
    int contentSize;
    f>>contentSize;
    int prid;
    for(int i=0;i<contentSize;i++){
        f>>prid;
        for(auto& it :goods){
            int flag=1;
            for(auto& ite:it.second ){
                if(ite->Getmprid()==prid){
                    contents.emplace_back(ite);
                    flag=0;
                    break;
                }
            }
            if(flag=0)break;
        }
    }
}

