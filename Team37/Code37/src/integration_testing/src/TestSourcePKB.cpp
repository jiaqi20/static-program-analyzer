#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <sstream>
#include <set>

#include "../src/SourceProcessor/ASTNode.h"
#include "../src/SourceProcessor/Lexer.h"
#include "../src/SourceProcessor/SourceParser.h"
#include "../src/PKB/PKB.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Source Parser and PKB - demo") {
    
    std::string program = R"(
        procedure main {
            x = 1;
        }
    )";
    std::stringstream ss;
    ss << program;
    
    PKB* testPKB = new PKB();
    SourceParser sp = SourceParser(testPKB);
    sp.Parse(ss);

    set<VarName> set1;
    set<StatementNumber> set2;
    set1.insert(VarName("x"));
    set2.insert(StatementNumber(1));
        
    // test modify
    REQUIRE(testPKB->GetModifies(StatementNumber(1), VarName("x")));
    REQUIRE(testPKB->GetModifiesByStatement(StatementNumber(1)) == set1);
    REQUIRE(testPKB->GetModifiesByVar(VarName("x")) == set2);

    // test max stmt number
    REQUIRE(testPKB->GetMaxStmtNumber() == 1);
}

TEST_CASE("Source Parser and PKB - All design abstraction") {

    std::string program = R"(
        procedure main {
            read x;
            print abc123;
            x = x + x * y;
            
            while (x <= y) {
                print Hello;
            }  
            
            if (x == y) then {
                x = 1;
            } else {
                read x;
            }
            
        }
    )";
    std::stringstream ss;
    ss << program;

    PKB* testPKB = new PKB();
    SourceParser sp = SourceParser(testPKB);
    sp.Parse(ss);
     
    // test max stmt number
    REQUIRE(testPKB->GetMaxStmtNumber() == 8);

    // test procedure
    set<ProcedureName> expProcedure{ "main" };
    REQUIRE(testPKB->GetAllProcedureNames() == expProcedure);

    // test read
    set<StatementNumber> expRead{ StatementNumber(1), StatementNumber(8) };
    REQUIRE(testPKB->GetAllReadStatements() == expRead);

    // test print
    set<StatementNumber> expPrint{ StatementNumber(2), StatementNumber(5) };
    REQUIRE(testPKB->GetAllPrintStatements() == expPrint);

    // test modifies relation
    set<VarName> expModifies{ VarName("x") };
    REQUIRE(testPKB->GetModifiesByStatement(StatementNumber(3)) == expModifies);

    // test uses relation
    set<VarName> expUses{ VarName("x"), VarName("y")};
    REQUIRE(testPKB->GetUsesByStatement(StatementNumber(3)) == expUses);


}

TEST_CASE("Source Parser and PKB - Next relation 1 level") {
    
    SECTION("Only one level")
    {
        
        std::string program = R"(
            procedure main {
               read x;
               read y;
               x = x+1;
               y = y+1;
               print x;
               print y;}
            procedure first {
               print x;
               print y;
             }

        )";
        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        DesignExtractor de = DesignExtractor();

        auto v = testPKB->GetAllNext();
        set<pair<StatementNumber, StatementNumber>> exp{ 
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(7), StatementNumber(8)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
        
    }

    SECTION("single level while") {       
        
        std::string program = R"(
            procedure main {
                x = x+1;
                while (1) {
                    x=x+2;
                    y=x;}
                b=1;
                while (1) {
                    print y;}
            }
            procedure first {
                while (1) {
                    c = c + 1;}
            }
            procedure second{
                while(1) {
                    c = c + 1;
                    b = c + 1;
                    a = c + 1;}
            }
        )";
        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        
        auto v = testPKB->GetAllNext();
        set<pair<StatementNumber, StatementNumber>> exp{ 
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(2)},
            {StatementNumber(2), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(7), StatementNumber(6)},
            {StatementNumber(8), StatementNumber(9)},
            {StatementNumber(9), StatementNumber(8)},
            {StatementNumber(10), StatementNumber(11)},
            {StatementNumber(11), StatementNumber(12)},
            {StatementNumber(12), StatementNumber(13)},
            {StatementNumber(13), StatementNumber(10)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("single level if") {

        std::string program = R"(

            procedure main {
                x = 1;
                y = 1;
                if (1) then {
                    x=1;
                    y=1;
                    z=1;
                    p=1;}
                else {
                    y=1;}
                    x=1;
                    y=1;
            }
            procedure first {
                if (1) then {
                    x=1;}
                else {
                    y=1;}
            }
        
        )";
        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);
        set<pair<StatementNumber, StatementNumber>> exp{ 
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(7), StatementNumber(9)},
            {StatementNumber(3), StatementNumber(8)},
            {StatementNumber(8), StatementNumber(9)},
            {StatementNumber(9), StatementNumber(10)},
            {StatementNumber(11), StatementNumber(12)},
            {StatementNumber(11), StatementNumber(13)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("single level while and if") {
        
        std::string program = R"(

            procedure main {
                if (1) then {
                    x=1;
                    y=1;}
                else {
                    z=1;}
                while (0) {
                    x=1;}
                if (0) then {
                    x=1;}
                else {
                    y=1;}
           }
    
        
        )";
        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(7)},
            {StatementNumber(7), StatementNumber(8)},
            {StatementNumber(7), StatementNumber(9)},
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

}

TEST_CASE("Source Parser and PKB - Next relation nested level") {
    SECTION("double level nested while") {

        std::string program = R"(
   
            procedure main {
                while(1) {
                    x=1;
                    while (0) {
                        y=1;
                        z=1;}
                    a=1;}
                y=1;
            }

            procedure first {
                 while(1) {
                      while(0) {
                         x=1;}}
            }

            procedure second {
                while(1) {
                    while(0) {
                        z=1;}
                    x=1;}
            }   
        
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        auto v = testPKB->GetAllNext();

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(7)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(1)},
            {StatementNumber(8), StatementNumber(9)},
            {StatementNumber(9), StatementNumber(8)},
            {StatementNumber(9), StatementNumber(10)},
            {StatementNumber(10), StatementNumber(9)},
            {StatementNumber(11), StatementNumber(12)},
            {StatementNumber(12), StatementNumber(13)},
            {StatementNumber(13), StatementNumber(12)},
            {StatementNumber(12), StatementNumber(14)},
            {StatementNumber(14), StatementNumber(11)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("double level nested if 1") {

        std::string program = R"(
                      
            procedure main {
                if (1) then {
                    x=1;

                    if (0) then {
                        y=1;}
                    else {
                        x=1;}}
                else {
                    y=1;}
                
                z=1;
            }
          
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        
        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(6)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(7)},
            {StatementNumber(5), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(7)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("double level nested if 2") {
        
        std::string program = R"(
                                 
            procedure main {
                if (0) then {
                    x=1;

                    if (1) then {
                        y=1;}
                    else {
                        x=1;}}
                else {

                    if (1) then {
                        x=1;}
                    else {
                        a=1;
                        y=1;}}
                z=1;
            }
               
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        
        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(6)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(10)},
            {StatementNumber(5), StatementNumber(10)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(8)},
            {StatementNumber(8), StatementNumber(9)},
            {StatementNumber(7), StatementNumber(10)},
            {StatementNumber(9), StatementNumber(10)},
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("double level nested if 3") {

        std::string program = R"(
                                                    
            procedure main {
                if (0) then {
                    x=1;
                    if (1) then {
                        y=1;}
                    else {
                        x=1;}}
                else {
                    if (1) then {
                        x=1;}
                    else {
                        a=1;
                        y=1;}
                    x=1;}
                z=1;
            }
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        
        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(6)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(11)},
            {StatementNumber(5), StatementNumber(11)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(8)},
            {StatementNumber(8), StatementNumber(9)},
            {StatementNumber(7), StatementNumber(10)},
            {StatementNumber(9), StatementNumber(10)},
            {StatementNumber(10), StatementNumber(11)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    } 

    SECTION("double level nested while and if") {
       
        std::string program = R"(
                                                                          
        procedure main {
            if (1) then {
                while (2) {
                    x=1;}}
            else {
                while(1) {
                    x=1;}
                y=1;}
            x=1;
        }
                           
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(2), StatementNumber(7)},
            {StatementNumber(3), StatementNumber(2)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(6)},
            {StatementNumber(5), StatementNumber(4)},
            {StatementNumber(6), StatementNumber(7)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("triple level nested while and if") {

        std::string program = R"(
                                                              
            procedure main {
                while(1) {
                    if (1) then {
                        if (0) then {
                           x=1;}
                        else {
                            y=1;}}
                    else {
                        if (0) then {
                            x=1;}
                        else {
                            y=1;}}}
                x=1;  
            }    
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        auto v = testPKB->GetAllNext();
        //testpkb.PrintAllRelations(v);

        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(1), StatementNumber(9)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(2), StatementNumber(6)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(1)},
            {StatementNumber(5), StatementNumber(1)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(8)},
            {StatementNumber(8), StatementNumber(1)},
            {StatementNumber(7), StatementNumber(1)},
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    
}

TEST_CASE("Source Parse and PKB: simple Affects") {
    SECTION("Code 7 from wiki") {
        std::string program = R"(
                                                              
            procedure alpha {
                x = 1;
                if ( i != 2 ) then {
                    x = a + 1; }
                  else {
                    a = b; }
                a = x; }   
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        REQUIRE(testPKB->GetAffects(StatementNumber(1), StatementNumber(5)));
        REQUIRE(testPKB->GetAffects(StatementNumber(3), StatementNumber(5)));
    }

    SECTION("Code 9 from wiki") {
        std::string program = R"(
                                                              
            procedure p {
                x = 1;
                y = 2;
                z = y;
                call q;
                z = x + y + z; }

            procedure q {
                x = 5;
                t = 4;
                if ( z > 0 ) then {
                    t = x + 1; }
                   else {
                   y = z + x; }
              x = t + 1; }   
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        REQUIRE(!testPKB->GetAffects(StatementNumber(1), StatementNumber(5)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(2), StatementNumber(5)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(3), StatementNumber(10)));
        testPKB->ClearCache();
        auto v = testPKB->GetAllAffects();
        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(5)},
            {StatementNumber(6), StatementNumber(9)},
            {StatementNumber(6), StatementNumber(10)},
            {StatementNumber(7), StatementNumber(11)},
            {StatementNumber(9), StatementNumber(11)}
        };
        set<pair<StatementNumber, StatementNumber>> actual = set<pair<StatementNumber, StatementNumber>>(v.begin(), v.end());
        REQUIRE(actual == exp);
    }

    SECTION("Code 10 from wiki") {
        std::string program = R"(
                                                              
            procedure alpha {
                x = 1;
                call beta;
                a = x; }

            procedure beta {
                if ( i != 2 ) then {
                    x = a + 1; }
                  else {
                    a = b; } }  
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        REQUIRE(!testPKB->GetAffects(StatementNumber(1), StatementNumber(3)));
    }

    SECTION("Code 11 from wiki") {
        std::string program = R"(
                                                              
            procedure p {
                x = a;
                read x;
                v = x; }  
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        REQUIRE(!testPKB->GetAffects(StatementNumber(1), StatementNumber(3)));
    }

    SECTION("Example from tutor") {
        std::string program = R"(
            procedure One {
                a=b*c+d;
                while (c>0) {
                    if (d>0) then {
                        b = a-1;}
                    else {
                        c=b*d;}
                    d=b+c;
                    c=d*a;
                    b=d-c;}}
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        set<pair<StatementNumber, StatementNumber>>exp{
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(1), StatementNumber(7)},
            {StatementNumber(4), StatementNumber(6)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(8)},
            {StatementNumber(7), StatementNumber(8)},
            {StatementNumber(6), StatementNumber(5)},
            {StatementNumber(8), StatementNumber(6)},
            {StatementNumber(8), StatementNumber(5)},
            {StatementNumber(7), StatementNumber(6)}
        };
        auto v = testPKB->GetAllAffects();
        set<pair<StatementNumber, StatementNumber>> actual = set<pair<StatementNumber, StatementNumber>>(v.begin(), v.end());
        REQUIRE(actual == exp);
        testPKB->ClearCache();

        // Affects*
        auto v1 = testPKB->GetAllAffectsStar();
        set<pair<StatementNumber, StatementNumber>>exp1{
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(1), StatementNumber(7)},
            {StatementNumber(4), StatementNumber(6)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(8)},
            {StatementNumber(7), StatementNumber(8)},
            {StatementNumber(6), StatementNumber(5)},
            {StatementNumber(8), StatementNumber(6)},
            {StatementNumber(8), StatementNumber(5)},
            {StatementNumber(7), StatementNumber(6)},
            {StatementNumber(1), StatementNumber(6)},
            {StatementNumber(1), StatementNumber(8)},
            {StatementNumber(1), StatementNumber(5)},
            {StatementNumber(4), StatementNumber(7)},
            {StatementNumber(4), StatementNumber(8)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(7)},
            {StatementNumber(5), StatementNumber(8)},
            {StatementNumber(5), StatementNumber(5)},
            {StatementNumber(6), StatementNumber(6)},
            {StatementNumber(7), StatementNumber(5)},
            {StatementNumber(7), StatementNumber(7)},
            {StatementNumber(8), StatementNumber(7)},
            {StatementNumber(8), StatementNumber(8)},
        };
        //testPKB->PrintAllRelations(v1);
        set<pair<StatementNumber, StatementNumber>> actual1 = set<pair<StatementNumber, StatementNumber>>(v1.begin(), v1.end());
        REQUIRE(actual1 == exp1);
    }
}

TEST_CASE("Source Parser and PKB: Affects with nested level") {
    SECTION("2 levels nested while") {
        std::string program = R"(
   
            procedure main {
                x = 1;
                while(1) {
                    y=x;
                    z=y;
                    while (0) {
                        x=y;
                        y=z;}
                    a=1;}
                y=x;
                x=z;
            }  
        
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        auto v = testPKB->GetAllAffects();
        //testPKB->PrintAllRelations(v);
        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(3)},
            {StatementNumber(1), StatementNumber(9)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(6)},
            {StatementNumber(4), StatementNumber(7)},
            {StatementNumber(4), StatementNumber(10)},
            {StatementNumber(7), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(3)},
            {StatementNumber(6), StatementNumber(9)},
        };
        set<pair<StatementNumber, StatementNumber>> actual = set<pair<StatementNumber, StatementNumber>>(v.begin(), v.end());
        REQUIRE(actual == exp);
        set<pair<StatementNumber, StatementNumber>> exp1{
            {StatementNumber(1), StatementNumber(3)},
            {StatementNumber(1), StatementNumber(9)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(3), StatementNumber(6)},
            {StatementNumber(4), StatementNumber(7)},
            {StatementNumber(4), StatementNumber(10)},
            {StatementNumber(7), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(3)},
            {StatementNumber(6), StatementNumber(9)},
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(1), StatementNumber(6)},
            {StatementNumber(1), StatementNumber(7)},
            {StatementNumber(1), StatementNumber(10)},
            {StatementNumber(3), StatementNumber(7)},
            {StatementNumber(3), StatementNumber(10)},
            {StatementNumber(3), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(9)},
            {StatementNumber(4), StatementNumber(6)},
            {StatementNumber(4), StatementNumber(3)},
            {StatementNumber(4), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(9)},
            {StatementNumber(7), StatementNumber(3)},
            {StatementNumber(7), StatementNumber(4)},
            {StatementNumber(7), StatementNumber(7)},
            {StatementNumber(7), StatementNumber(9)},
            {StatementNumber(7), StatementNumber(10)},
            {StatementNumber(6), StatementNumber(4)},
            {StatementNumber(6), StatementNumber(6)},
            {StatementNumber(6), StatementNumber(7)},
            {StatementNumber(6), StatementNumber(10)},
        };
        auto v1 = testPKB->GetAllAffectsStar();
        set<pair<StatementNumber, StatementNumber>> actual1 = set<pair<StatementNumber, StatementNumber>>(v1.begin(), v1.end());
        REQUIRE(actual1 == exp1);
    }

    SECTION("if nested in while: Code 6 from wiki") {
        std::string program = R"(
                                                             

                  procedure Second {
                    x = 0;
                    i = 5;
                    while (i!=0) {
                        x = x + 2*y;
                        call Third;
                        i = i - 1; }
                    if (x==1) then {
                        x = x+1; }
                      else {
                        z = 1; }
                    z = z + x + i;
                    y = z + 2;
                    x = x * y + z; }

                  procedure Third {
                      z = 5;
                      v = z;
                      print v; }   
                      
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        REQUIRE(testPKB->GetAffects(StatementNumber(2), StatementNumber(6)));
        REQUIRE(testPKB->GetAffects(StatementNumber(4), StatementNumber(8)));
        REQUIRE(testPKB->GetAffects(StatementNumber(4), StatementNumber(10)));
        REQUIRE(testPKB->GetAffects(StatementNumber(6), StatementNumber(6)));
        REQUIRE(testPKB->GetAffects(StatementNumber(1), StatementNumber(4)));
        REQUIRE(testPKB->GetAffects(StatementNumber(1), StatementNumber(8)));
        REQUIRE(testPKB->GetAffects(StatementNumber(1), StatementNumber(10)));
        REQUIRE(testPKB->GetAffects(StatementNumber(1), StatementNumber(12)));
        REQUIRE(testPKB->GetAffects(StatementNumber(2), StatementNumber(10)));
        REQUIRE(testPKB->GetAffects(StatementNumber(9), StatementNumber(10)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(9), StatementNumber(11)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(9), StatementNumber(12)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(2), StatementNumber(3)));
        REQUIRE(!testPKB->GetAffects(StatementNumber(9), StatementNumber(6)));
        testPKB->ClearCache();
        set<pair<StatementNumber, StatementNumber>> exp{
            {StatementNumber(1), StatementNumber(4)},
            {StatementNumber(1), StatementNumber(8)},
            {StatementNumber(1), StatementNumber(10)},
            {StatementNumber(1), StatementNumber(12)},
            {StatementNumber(2), StatementNumber(6)},
            {StatementNumber(2), StatementNumber(10)},
            {StatementNumber(4), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(8)},
            {StatementNumber(4), StatementNumber(10)},
            {StatementNumber(4), StatementNumber(12)},
            {StatementNumber(6), StatementNumber(10)},
            {StatementNumber(6), StatementNumber(6)},
            {StatementNumber(8), StatementNumber(10)},
            {StatementNumber(8), StatementNumber(12)},
            {StatementNumber(9), StatementNumber(10)},
            {StatementNumber(10), StatementNumber(11)},
            {StatementNumber(10), StatementNumber(12)},
            {StatementNumber(11), StatementNumber(12)},
            {StatementNumber(13), StatementNumber(14)},
        };
        auto v = testPKB->GetAllAffects();
        set<pair<StatementNumber, StatementNumber>> actual = set<pair<StatementNumber, StatementNumber>>(v.begin(), v.end());
        REQUIRE(actual == exp);
        testPKB->ClearCache();
        set<StatementNumber> exp1{ StatementNumber(1), StatementNumber(2), StatementNumber(4), StatementNumber(6),
        StatementNumber(8), StatementNumber(9), StatementNumber(10), StatementNumber(11), StatementNumber(13) };
        REQUIRE(testPKB->GetAllStatementsAffects() == exp1);
        set<StatementNumber> exp2{ StatementNumber(12), StatementNumber(14), StatementNumber(4), StatementNumber(6),
        StatementNumber(8), StatementNumber(10), StatementNumber(11) };
        REQUIRE(testPKB->GetAllStatementsAffected() == exp2);
    }
}

TEST_CASE("Source Parser and PKB: iter2") {
    SECTION("all") {
        std::string program = R"(
   
            procedure procA {
	read a;
	print b;
	call procB;
	if(x == y) then {
		y = z;
	} else {
		z = 11 + 13 + 15 + 16;
	}
	while(a1 == b1) {
		n = q;
	}
	while((!(file > a)) || ((result >= 1) && (m % 24134 < p))) {
		c = o;
	}
	if((ifVar <= 1 - (ifVar2 - ifVar)) || ((!(ifVar == 2)) && (ifVar != ifVar3 + ifVar))) then {
		ifVar = ifVar3;}
	else{
		if((max <= 1 - (b - a )) || ((!(r == 2)) && (max != z + min))) then {
		b = a;
 	} else {
		print  ai;
	}
		a = b;
	}
	a= b + c;
	a = b + (c - d);	a = (b + c - (abc %d)) / e;
		a = (b +c - d) / e % a + (b + c - d) / e;
a = b+c-d;
		a = b + (c-d);
	a = (b + c -d / (ab % d)) / e;
	a = (b + c*(b -q / (abc % d))) / e %a;
}

procedure procB {
	print bA;
	if(bA == 0) then {
		bA = 1234;
		if(bBBB != bAAA) then {
			call procD;
			read uniA;
		} else {
			call procE;
			print uniB;
		}
	} else {
		bA = bC / 2;
		while(bA == bB) {
			bA = 10;
			x =10;
			call procD;
			if(bD == (bA + bB + bC)) then {
				while( bD == 4321 ) {
					call procE;
					while (bA >=0){
						print bD;
					}
				}
			} else {
				print bD;
			}
		}
		bD = bC / 2;
	}
	call procC;
}

procedure procE {
	print EEEEE;
}

procedure procC {
	if( x != 0 )then {
		if (y != 1) then {
			while ( y!= 1) {
				z = x + y;}}
		else {
			z = 0;}
		while (x !=1) {
			y = 1;}
		x = x + 1;}
	else {
	while (x != 9) {
                                  x = x-1;
	while (y != 0) {
	y = y - 1;}}
		k = 1;}
	file = k + y - x;
	print cA;
}

	procedure procD {
	while(dAAA==9){
	dAAA = dBBB;
	call procE;
}
print uniA;
dA = dB / dC + uniA;
dA = dB / dc % dD - dE + dF;
}

procedure ifInIf {
	read uniC;
	if(ifA == ifB) then {
		ifA = 0;
	} else {
		ifA = 1;
		if(ifA == ifC) then {
			ifA = 2;
		} else {
			if(ifA == ifD) then {
				ifB = 3;
			} else {
				ifA = 5;
			}
			ifA = 3;
		}
	}
	print uniC;
}

procedure withTest {
	procC = 1;
}

procedure ifA{
read x;
}  
        
        )";

        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);
        auto v = testPKB->GetAllAffects();
        //testPKB->PrintAllRelations(v);
        auto v1 = testPKB->GetAllAffectsStar();
        //testPKB->PrintAllRelations(v1);
    }
}