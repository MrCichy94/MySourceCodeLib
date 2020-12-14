#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>

/*MOUDS OPERANDI
    1) KLASA PROGRAMISTY
        a) KONSTRUKCJE
            stan & state;
            void Kitchen() { state.Kitchen(); }
            void computer(){ state.Computer(); }
            void walk(stan S) { state.Switch(S); }
            
    2) INTERFEJS STANU
        &programisty
        virtual Kitchen() =0;
        virtual Computer()
        
    3) KLASY STANÓW
        
        a) KONSTRUKTORY
        b) funkcje -> co robi na komputerze, co robi w kuchni ?
        c) funkcja zmiany stanu
Dom
Praca
*/



class Programer 
{
public:
    Place* place_;
    
    Programer()
    {
        this->place_ = new Home();
    }

};

class Place 
{
protected:
    Programer* programer_;
public:

    virtual std::string useComputer() = 0;
    virtual std::string useKitchen() = 0;
    virtual ~Place() = default;
    void setProgramer(Programer* programer){ this->programer_ = programer; };
    Programer* getProgramer() { return programer_; };
};


class Work : public Place
{
public:
    
    std::string useComputer() override { return std::string("Coding"); };
    std::string useKitchen() override { return std::string("Lunch"); };
};

class Home : public Place
{
public:

    std::string useComputer() override { return std::string("Playing"); };
    std::string useKitchen() override { return std::string("Dinner"); };
    
};

// You can write test cases here (Catch2 framework)
TEST_CASE("should creatre Programmer") {
    Programer programer;
}

TEST_CASE("should Work exist") {
    Work work;
}

TEST_CASE("should Home exist") {
    Home home;
}

TEST_CASE("Work should use computer to code"){
    Work work;
    CHECK("Coding" == work.useComputer());
}

TEST_CASE("Place should use computer properly"){
    
    Place* work = new Work();
    Place* home = new Home();
    
    CHECK("Coding" == work->useComputer());
    CHECK("Playing" == home->useComputer());
    
    delete(work);
    delete(home);
}

TEST_CASE("Place should use kitchen properly"){
    
    Place* work = new Work();
    Place* home = new Home();
    
    CHECK("Lunch" == work->useKitchen());
    CHECK("Dinner" == home->useKitchen());
    
    delete(work);
    delete(home);
}



TEST_CASE("Should create state with reference"){
    Programer* programer = new Programer();
    
    Place* work = new Work();
    Place* home = new Home();
    home->setProgramer(programer);
    work->setProgramer(programer);
    
    CHECK(home->getProgramer() == work->getProgramer());
    
    delete(programer);
    delete(work);
    delete(home);
}

TEST_CASE("Should programmer wake up in Home"){
    Programer programer;
    
   // CHECK(typeid(programer.place_) == typeid(new Home()));
    
}



/*

// int sum(int a, int b) {
//     return a + b;
// }

// // You can write test cases here (Catch2 framework)
// TEST_CASE("should add two numbers") {
//     // you can write to stdout for debugging purposes, e.g.
//     std::cout << "This is a debug message" << std::endl;
        // int i=9;
//     REQUIRE( sum(20, 17) == 37 );
    // SECTION( "resizing bigger changes size and capacity" ) {
    //     v.resize( 10 );
    // }
    
    //  SECTION( "resizing bigger changes size and capacity" ) {
    //     v.resize( 10 );
    // }
// }
*/