#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>


#define TEST_CASE(n) std::cout << "TEST: " << n << '\n';
#define EXPECT(p) if (p) {} else std::cout << __LINE__ << ": FAILED: " << #p << '\n'
#define ASSERT(p) if (p) {} else do { std::cout << __LINE__ << ":FAILED: " << #p << '\n'; return; } while (false)

class PatternGeneratorI {
public:
    virtual ~PatternGeneratorI() = default;
    virtual std::string nextPattern() = 0;
};


class PatternGenerator : public PatternGeneratorI {
private:
    std::string m_letters;
public:
    PatternGenerator(std::string s) : m_letters(s) {}
    
    std::string nextPattern() override  {
        std::string temp = ""    ;
        for (int i = 0; i < 4; i++)
        {
            int position = rand() % m_letters.length();
            temp += m_letters[position];
        }
        return temp;
    } 
    
};


class PatternGeneratorFake : public PatternGeneratorI {
public:
    std::string nextPattern() override  { return "abba"; } 
};

class Player {
private:
    std::istream& m_is;
public:
   Player(std::istream& is) : m_is(is) {}
   
   std::string nextGuess(){
       //todo: add validation for number of pattern
       std::string userGuess;
       m_is >> userGuess;
       return userGuess;
   }

};

struct MatchResult {
    int exact = 0; 
    int partial = 0;
    bool fullMatch = false;
};

class Matcher {
public:
   MatchResult match(std::string pattern, std::string guess) {
       
       int exact = exactMatch(pattern, guess);
       int partial = partialMatch(pattern, guess) - exact;
       bool fullMatch = isFullMatch(exact, guess);
       
       return MatchResult{exact, partial, fullMatch};
      
   }
   
   
   private :
   
   int exactMatch(std::string pattern, std::string guess){
       int exact = 0;
       for (auto itP = pattern.begin(), itG = guess.begin(); itP != pattern.end(); ++itP, ++itG){
           if(*itP == * itG){
               exact++;
           }
       }
      return exact;
   }
   
   int partialMatch(std::string pattern, std::string guess){
       int partial = 0;
       for (auto itP : pattern){
            auto i = std::find(guess.begin(), guess.end(), itP);
            if(i != guess.end())
            {
                guess.erase(i);
                ++partial;
            }
       }
       return partial;
   } 
   
   bool isFullMatch(int exact, std::string guess){
      return guess.length() == exact;
   }
};

void test_Matcher() {
    TEST_CASE("test_matcher");
    Matcher matcher;
    
    
    EXPECT(matcher.match("abab","aaaa").exact == 2);
    EXPECT(matcher.match("abab","abab").exact == 4);
    EXPECT(matcher.match("abab","abab").fullMatch == true);
    EXPECT(matcher.match("abab","aaaa").fullMatch == false);
    EXPECT(matcher.match("abba","aabb").partial == 2);
    EXPECT(matcher.match("abba","abab").partial == 2);
}



class Display {
private:
    std::ostream& m_os;

public:
   Display(std::ostream& os): m_os (os) {}
   
   void printWon (){
    m_os << "You Won!\n";
   }
   
   void printResult (MatchResult result){
        m_os << " >>> " << std::string(result.exact, '*') << std::string(result.partial, 'o') << std::endl;
    
   }
   
   void printPattern (std::string pattern) {
       m_os << std::string(pattern.length(), '#') << std::endl;
   }
   
   
   
};

class Game {
private:
    PatternGeneratorI& m_patternGenerator;
    Player& m_player;
    Matcher& m_matcher;
    Display& m_display;
public:
   Game(PatternGeneratorI& pG,
        Player& p,
        Matcher& m,
        Display& d): m_patternGenerator(pG), m_player(p), m_matcher(m), m_display(d) {}
    void run() {
        std::string pattern = m_patternGenerator.nextPattern();
        m_display.printPattern(pattern);
        
        while(1){
            
            std::string guess = m_player.nextGuess();
            MatchResult match = m_matcher.match(pattern, guess);
            m_display.printResult(match);
            if (match.fullMatch){
                m_display.printWon();
                break;
            }
        }
    }
};



void test_scenario1() {
    TEST_CASE("Scenario 1")
    std::istringstream input("aaaa\naabb\nabab\nabba\n");
    Player player(input);
    std::ostringstream output;
    std::string expectedOutput("####\n >>> **\n >>> **oo\n >>> **oo\n >>> ****\nYou Won!\n");
    Display display(output);
    PatternGeneratorFake generator;
    Matcher matcher;
    Game game(generator, player, matcher, display);
    game.run();
    EXPECT(output.str() == expectedOutput);
    std::cout << "Expected: " << expectedOutput;
    std::cout << "Actual: " << output.str();
}

void tests() {
    test_Matcher();
    test_scenario1();
}

void realPlay() {
    TEST_CASE("Real play!");
    std::srand(std::time(0));
    Player player(std::cin);
    Display display(std::cout);
    PatternGenerator generator("abcdef");
    Matcher matcher;
    Game game(generator, player, matcher, display);
    game.run();
}

int main() {
    tests();
    realPlay();
}
