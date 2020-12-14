#include <iostream>
#include <array>
#include <string>
#include <sstream>

/////////////////
struct TestCase { std::string name{}; std::ostringstream failures{}; };template <typename T> void operator | (TestCase& testCase, T&& testBody) {    testBody(); auto failures = testCase.failures.str();    if (failures.empty()) std::cout << testCase.name << ": SUCCESS\n";   else std::cerr << testCase.name << ": FAILURE\n" << failures;}void addFailure(std::ostream& os, const char* file, unsigned line, const char* condition) {    os << file << ":" << line << ": Failed: " << condition << "\n";}
#define TEST(name) TestCase name{#name}; name | [&, &failures = name.failures]
#define EXPECT(cond) if (cond) {} else addFailure(failures, __FILE__, __LINE__, #cond)
#define ASSERT(cond) if (cond) {} else return addFailure(failures, __FILE__, __LINE__, #cond)
//////////////////

using LcdSegment = std::string;

/*  1. Podział stringa
    2. Przypisanie do tablicy wielowymiarowej
    3. "sklejanie" kolejnego elementu i porównywanie z przykładami.
*/






int isDivisableBy3(const LcdSegment lcd)
{
    return lcd.length() % 9 ? -1 : lcd.size() / 9;
}

std::string getDigit(const LcdSegment lcd, int digitPosition)
{
    std::string number;
    
    int digits = isDivisableBy3(lcd);
    
    if(digits > 0)
    {
        for(int j = 0; j < 3 ; j++)
        {
            for(int i = ((digitPosition-1)*3); i <= (((digitPosition-1)*3)+2); i++)
            {
                number += lcd[i+j*digits*3];
            }
        }
    }else
        return "";
    
    return number;
}



/* 1) std::string digit getDigit(const LcdSegment lcd, digitNumber);
   2) czy to jest digit?
  */ 
   std::string isDigit(std::string digit) {
    if(digit ==    " _ "
                   "| |"
                   "|_|") {return "0"; }
    if(digit ==    "   "
                   "  |"
                   "  |") {return "1"; }
    if(digit ==    " _ "
                   " _|"
                   "|_ ") {return "2"; }
    if(digit ==    " _ "
                   " _|"
                   " _|") {return "3"; }
    if(digit ==    "   "
                   "|_|"
                   "  |") {return "4"; }
    if(digit ==    " _ "
                   "|_ "
                   " _|") {return "5"; }
    if(digit ==    " _ "
                   "|_ "
                   "|_|") {return "6"; }
    if(digit ==    " _ "
                   "  |"
                   "  |") {return "7"; }
    if(digit ==    " _ "
                   "|_|"
                   "|_|") {return "8"; }
    if(digit ==    " _ "
                   "|_|"
                   " _|") {return "9"; }
    else return "";
}



auto convertLcdToDigits(const LcdSegment& lcdDisplay)
{
    if(lcdDisplay=="") return std::string("");
    
    std::string representation;
    std::string digit;
    
    int no_digits = isDivisableBy3(lcdDisplay);
    if(no_digits > 0)
    {
        for(int i=1; i < no_digits+1; i++){
            
            digit = getDigit(lcdDisplay, i);
            if(isDigit(digit) != ""){
                representation += isDigit(digit);
            }
            else{
                return std::string ("-1");
            }
        }
        return representation;
    }else
        return std::string ("-1");

}





int main()
{
TEST(shallConvertLcdDisplayIntoNumber)
{
        const LcdSegment lcd{" _     _     _    "
                             " _|  || |  || |  |"
                             "|_   ||_|  ||_|  |"};
        std::string s = "210101";
        std::cout << convertLcdToDigits(lcd);
        ASSERT( s == convertLcdToDigits(lcd));
};


TEST(shallReturnError)
{
        const LcdSegment lcd{" _     _     _    "
                             " _|  || |  || |  |ds"
                             "|_   ||_|sd  ||_d|  |"};
        std::string s = "-1";
        std::cout << convertLcdToDigits(lcd);
        ASSERT( s == convertLcdToDigits(lcd));
};

TEST(CorrectNumberofDigitsShallPass)
{   
    const LcdSegment lcd{" _     _     _    "
                        " _|  || |  || |  |"
                        "|_   ||_|  ||_|  |"};
    int number = isDivisableBy3(lcd);
    //std::cout << number <<"\n";
    ASSERT(6 == number);
};
  
TEST(shallReturnEmptyStringOnEmptyLcd)
{
    const LcdSegment lcd{""};
    std::string s = "";
    ASSERT(s == convertLcdToDigits(lcd));
};

TEST(shallReturnCorrectDigitRepresnetation){
    
    const LcdSegment lcd{" _     _     _    "
                         " _|  || |  || |  |"
                         "|_   ||_|  ||_|  |"};
    std::string digit = getDigit(lcd, 2);
    //std::cout << digit;
    ASSERT(digit =="     |  |");
};
    
/*TEST(returnOneWhenStringIsGiven)
{
    //Field field;
    //ASSERT(" " == Field.get());
    const LcdSegment lcd{   "   "
                            "  |"
                            "  |"};
    ASSERT("1" == convertLcdToDigits(lcd));
};
*/
TEST(isDigitTest){
     const LcdSegment lcd{" _     _     _    "
                         " _|  || |  || |  |"
                         "|_   ||_|  ||_|  |"};
    std::string first = getDigit(lcd, 1);
    std::string second = getDigit(lcd, 2);
    std::string third = getDigit(lcd, 3);
    
    std::string d1 = isDigit(first);
    std::string d2 = isDigit(second);
    std::string d3 = isDigit(third);
    
    ASSERT(d1 == "2");
    ASSERT(d2 == "1");
    ASSERT(d3 == "0");
};
    return 0;
}