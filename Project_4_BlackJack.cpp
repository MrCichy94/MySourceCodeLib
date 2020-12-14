#include <iostream>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <stdlib.h> 
#include <memory>
#include <stack>
#include <vector>

/*
    BLACKJACK
    
        1. DECK
            -getCard
        
        2. PLAYER -> human, dealer
            -std::vector<Card> _card
            
            -hit()
            -stand()
            
            priv:
            -calcSum()
            -sum
        
        
        3. COMPARATOR
        4. WIN LOGIC
        
        5. INPUTER
        6. PRINTER
        
*/
class Card
{
    int _value;
    
public:    
    Card(int value) : _value(value) {}
    
    int getValue() const
    {
        return _value;
    }
};


class Deck 
{
    private: 
    
    std::stack<std::unique_ptr<Card>> cards;
    std::unique_ptr<Card> karta;
    
    public:
    
    Deck() 
    {
        cards.push(std::make_unique<Card>(5));
        cards.push(std::make_unique<Card>(10));
        cards.push(std::make_unique<Card>(6));
    }
    
    std::unique_ptr<Card> getCard()
    {
        auto karta = std::move(cards.top());
        cards.pop();
        return karta;
    }
};

class Player
{
    std::vector<std::unique_ptr<Card>> playersCards;
    unsigned sum=0;
    
public: 

    void hit(std::unique_ptr<Card> card)
    {
        
        if(card->getValue()==11 && sum > 10)
            sum=sum+1;
            else sum=sum+card->getValue();
            
        playersCards.push_back(std::move(card));
        
    }
    
    unsigned getSum ()
    {
        return sum;
    }
    
};

class Dealer
{
    private:
    
    unsigned sum=0;
    std::unique_ptr<Player> _player;
    
    public:
    
    
    Dealer(std::unique_ptr<Player> player) : _player(std::move(player)) {}
    
    void makeMove(std::unique_ptr<Card> card)
    {
        if(_player->getSum()<18)
        {
            _player->hit(std::move(card));
        }
    }
    
    unsigned getSum()
    {
        return _player->getSum();
    }
    
    
};


// int sum(int a, int b) {
//     return a + b;
// }

// You can write test cases here (Catch2 framework)
/*
TEST_CASE("should add two numbers") {
    // you can write to stdout for debugging purposes, e.g.
    int x = rand() % 10;
    std::cout << "This is a debug message " << x<< std::endl;
    SECTION("SECTION EXAMPLE"){
        CHECK( sum(20, 17) == 37 );
    }
}
*/

TEST_CASE("getCard return card")
{
    Deck deck;
    std::unique_ptr<Card> karta;
    karta= deck.getCard();
    REQUIRE (karta->getValue() == 6);
    karta= deck.getCard();
    REQUIRE (karta->getValue() == 10);
    karta= deck.getCard();
    REQUIRE (karta->getValue() == 5);
}

TEST_CASE("Player draws card from deck")
{
    
    Deck deck;
    Player player;
    
    player.hit(deck.getCard());
    player.hit(deck.getCard());
    player.hit(deck.getCard());
    
    REQUIRE (player.getSum() == 21);
  
}

TEST_CASE("Dealer knows how to play")
{
    auto player = std::make_unique<Player>();
    
    Dealer dealer(std::move(player));
    Deck deck;
    
    dealer.makeMove(deck.getCard());
    dealer.makeMove(deck.getCard());
    dealer.makeMove(deck.getCard());
    
    REQUIRE (dealer.getSum() == 21);
    
}