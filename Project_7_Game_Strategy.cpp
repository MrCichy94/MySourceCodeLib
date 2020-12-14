#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <memory>

/*MODUS OPERANDI

1. HERO

    _name
    _currentWeapon
    _str
    _agility

    a) switchWeapon(unique_ptr<Weapon>)
    b) attackMonster(uniqe_ptr<Monster>)

2. Weapon Interface

3. Monster

*/


class Monster
{
public:
    Monster(const std::string& name, int HP): _name(name), _HP(HP)
    {}

    void receiveDamage(int damage)
    {
        _HP=_HP - damage;

    }

    int getHP()
    {
        return _HP;
    }
private:
    std::string _name;
    int _HP;

};


class Weapon
{
public:
    virtual int getDamage(int agility, int attack) = 0;
    virtual ~Weapon() = default;
};

class Hero {

public:

    int _agility;
    int _attack;
    std::string _name;
    Weapon* _weapon;


    Hero(const std::string& name, int agility, int attack, Weapon *weapon=nullptr) :  _name(name),_agility(agility),_attack(attack), _weapon(weapon)
    {}

    void attack(Monster* monster)
    {
        if(_weapon==nullptr)
        {
            return;
        }
        else{
            monster->receiveDamage(_weapon->getDamage(_agility, _attack));
        }
    }

    void switchWeapon(Weapon* weapon)
    {
        delete _weapon;
        _weapon = weapon;
    }

    ~Hero()
    {
        delete _weapon;
    }
};

class Sword : public Weapon
{
    int getDamage(int agility, int attack) override
    {
        return (agility+attack)/2;
    }
};

class Spear : public Weapon
{
    int getDamage(int agility, int attack) override
    {
        return agility*2;
    }
};

class Hammer : public Weapon
{
    int getDamage(int agility, int attack) override
    {
        return attack*2;
    }
};

// int sum(int a, int b) {
//     return a + b;
// }

// // You can write test cases here (Catch2 framework)
// TEST_CASE("should add two numbers") {
//     // you can write to stdout for debugging purposes, e.g.
//     std::cout << "This is a debug message" << std::endl;

//     REQUIRE( sum(20, 17) == 37 );
// SECTION( "resizing bigger changes size and capacity" ) {}
// }

TEST_CASE("should create Weapons objects"){
Weapon* sword = new Sword();
Weapon* hammer = new Hammer();
Weapon* spear = new Spear();

REQUIRE (sword != nullptr);
REQUIRE (hammer != nullptr);
REQUIRE (spear != nullptr);
delete spear;
delete hammer;
delete sword;
// CHECK(TRUE)
}

TEST_CASE("should return correct damage"){

Weapon* sword = new Sword() ;
Weapon* hammer = new Hammer();
Weapon* spear = new Spear();
int agility ;
int attack ;

SECTION( "should take attack and agility and return damage - sword" ) {

agility = 2;
attack = 5;

REQUIRE (sword->getDamage(agility, attack ) == 3);

agility = 0;
attack = 0;

REQUIRE (sword->getDamage(agility, attack ) == 0);

agility = 0;
attack = 2;

REQUIRE (sword->getDamage(agility, attack ) == 1);

agility = 4;
attack = 0;

REQUIRE (sword->getDamage(agility, attack ) == 2);

}
SECTION( "should take attack and agility and return damage - spear" ) {

agility = 2;
attack = 5;

REQUIRE (spear->getDamage(agility, attack ) == 4);

agility = 0;
attack = 0;

REQUIRE (spear->getDamage(agility, attack ) == 0);

agility = 0;
attack = 2;

REQUIRE (spear->getDamage(agility, attack ) == 0);

agility = 4;
attack = 0;

REQUIRE (spear->getDamage(agility, attack ) == 8);

}
SECTION( "should take attack and agility and return damage - hammer" ) {

agility = 2;
attack = 5;

REQUIRE (hammer->getDamage(agility, attack ) == 10);

agility = 0;
attack = 0;

REQUIRE (hammer->getDamage(agility, attack ) == 0);

agility = 0;
attack = 2;

REQUIRE (hammer->getDamage(agility, attack ) == 4);

agility = 4;
attack = 0;

REQUIRE (hammer->getDamage(agility, attack ) == 0);

}

delete spear;
delete hammer;
delete sword;
}

TEST_CASE("should Monster receive damage"){
Monster monster("monster",200);
monster.receiveDamage(50);
REQUIRE(monster.getHP()==150);
}

TEST_CASE("should create Hero with attributes and wapon"){
Hero hero("Hero_name", 10, 5);
hero.switchWeapon(new Sword());
REQUIRE(hero._name=="Hero_name");
REQUIRE(hero._attack==5);
REQUIRE(hero._agility==10);
REQUIRE(hero._weapon!=nullptr);
}

TEST_CASE("Hero should gain correct amount of HP from monster")
{
Hero hero("Hero_name", 3, 3);
Monster monster("Troll",200);

hero.switchWeapon(new Sword());
hero.attack(&monster);
REQUIRE(monster.getHP()==197);

hero.switchWeapon(new Spear());
hero.attack(&monster);
REQUIRE(monster.getHP()==191);

hero.switchWeapon(new Hammer());
hero.attack(&monster);
REQUIRE(monster.getHP()==185);
}
