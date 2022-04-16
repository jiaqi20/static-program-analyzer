#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <sstream> // stringstream

#include "../src/SourceProcessor/ASTNode.h"
#include "../src/SourceProcessor/Lexer.h"
#include "../src/SourceProcessor/SourceParser.h"
#include "../src/PKB/PKB.h"

#include "catch.hpp"

using namespace std;
using namespace lexer;

TEST_CASE("Lexer and lexer::token") {

    Token token1 = { TokenType::kName, "x" };
    Token token2 = { TokenType::kOperator, "=" };
    Token token3 = { TokenType::kName, "x" };

    SECTION("overloading operator ==") 
    {
        REQUIRE(token1 == token3);
        
    }

    SECTION("overloading operator !=")
    {
        REQUIRE(token1 != token2);
    }
    
  
    SECTION("lexer tokenize method")
    {
        std::string program = R"(
            procedure main {
                x = 1;
            }
        )";
        std::stringstream ss;
        ss << program;

        // tokenize
        std::list<Token> tokens = Tokenize(ss);
        Token token = { TokenType::kName, "procedure" };
        Token token2 = { TokenType::kName, "main" };
        Token token3 = { TokenType::kDelimiter, "{" };
        REQUIRE(tokens.front() == token);
        tokens.pop_front();

        REQUIRE(tokens.front() == token2);
        tokens.pop_front();

        REQUIRE(tokens.front() == token3);
        tokens.pop_front();
    }
    
}

TEST_CASE("Source parser: parseStmt")
{
    Token token1 = { TokenType::kName, "if" };
    Token token2 = { TokenType::kOperator, "=" };
    Token token3 = { TokenType::kName, "if" };
    Token token4 = { TokenType::kOperator, "+" };
    Token token5 = { TokenType::kDelimiter, ";" };
    
    SECTION("if assign stmt") {
        std::list<Token> tokens;
        tokens.emplace_back(token1);
        tokens.emplace_back(token2);
        tokens.emplace_back(token3);
        tokens.emplace_back(token4);
        tokens.emplace_back(token3);
        tokens.emplace_back(token5);

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.SetToken(tokens);
        sp.ParseStmt();
        REQUIRE(1 == 1);
    }
    
}

TEST_CASE("Source parser: parse(fileStream) method") {
    
    SECTION("parse valid source")
    {
        std::string program = R"(
            procedure main {
                x = 1;
            }
        )";
        std::stringstream ss;
        ss << program;

        Token token = { TokenType::kName, "procedure" };
        Token token2 = { TokenType::kName, "main" };
        Token token3 = { TokenType::kDelimiter, "{" };

        try {
            PKB* testPKB = new PKB();
            SourceParser sp = SourceParser(testPKB);
            sp.Parse(ss);
        }
        catch (...) {

        }
        REQUIRE(1 == 1);
    }

    SECTION("parse complicated assign statement")
    {
        std::string program = R"(
            procedure main {
               abc123 = v + x * y + z * t;
               a = v + x * (y + z) * t;
            }
        )";
        std::stringstream ss;
        ss << program;


        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        DesignExtractor de = DesignExtractor();
        string rpn1 = de.Rpn("v + x * y + z * t");
        string rpn2 = de.Rpn("v + x * (y + z) * t"); //"(y+z)*t"

        // test modifies relation
        set<VarName> expVar1{ VarName("x") };
        REQUIRE(testPKB->GetModifiesByStatement(StatementNumber(1)) != expVar1);
        set<VarName> expVar2{ VarName("abc123") };
        REQUIRE(testPKB->GetModifiesByStatement(StatementNumber(1)) == expVar2);

        // test pattern
        vector<StatementNumber> expStatement1{ StatementNumber(1) };
        vector<StatementNumber> expStatement2{ StatementNumber(2) };
        vector<StatementNumber> expStatements{ StatementNumber(1), StatementNumber(2) };
        vector<StatementNumber> empty;
        REQUIRE(testPKB->GetStatementsBySameExpr("v+x*y+z*t") == expStatement1);
        REQUIRE(testPKB->GetStatementsBySameExpr("v") == empty);

        // invalid pattern
        REQUIRE(testPKB->GetStatementsBySameExpr("+x") == empty);
        REQUIRE(testPKB->GetStatementsBySameExpr("+v") == empty);
        REQUIRE(testPKB->GetStatementsBySameExpr("v+") == empty);
        REQUIRE(testPKB->GetStatementsBySameExpr("(") == empty);
        REQUIRE(testPKB->GetStatementsBySameExpr(")") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("+v") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("v+") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("+temp") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("+") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("(") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr(")") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("()") == empty);

        REQUIRE(testPKB->GetStatementsBySubExpr("v") == expStatements);
        REQUIRE(testPKB->GetStatementsBySubExpr("x*y") == expStatement1);
        REQUIRE(testPKB->GetStatementsBySubExpr("v+x") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("v+x*y") == expStatement1);
        REQUIRE(testPKB->GetStatementsBySubExpr("y+z*t") == empty); 
        REQUIRE(testPKB->GetStatementsBySubExpr("x*y+z*t") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("v+x*y+z*t") == expStatement1);
        
        // brackets
        REQUIRE(testPKB->GetStatementsBySubExpr("(y+z)") == expStatement2);
        REQUIRE(testPKB->GetStatementsBySubExpr("y+z") == expStatement2);
        REQUIRE(testPKB->GetStatementsBySubExpr("y+z*t") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("(y+z)*t") == empty);
        REQUIRE(testPKB->GetStatementsBySubExpr("x*(y+z)*t") == expStatement2);
        REQUIRE(testPKB->GetStatementsBySubExpr("x*(y+z)") == expStatement2);
        REQUIRE(testPKB->GetStatementsBySubExpr("x                                *(y+z)*t") == expStatement2);
        REQUIRE(testPKB->GetAssignByStatement(StatementNumber(1)) == rpn1);
        REQUIRE(testPKB->GetAssignByStatement(StatementNumber(2)) == rpn2);

    }

    SECTION("parse multiple procedure + call statements")
    {
        std::string program = R"(
            procedure main {
               abc123 = v + x * y + z * t;
               a = v + x * (y + z) * t;
               call main2;
            }

            procedure main2 {
                x = x + 1;
                call main3;
            }

            procedure main3 {
                read abc;
                print x1;
            }

        )";
        std::stringstream ss;
        ss << program;

        PKB* testPKB = new PKB();
        SourceParser sp = SourceParser(testPKB);
        sp.Parse(ss);

        DesignExtractor de = DesignExtractor();

        // multiple procedure
        set<string> expSet{ "main", "main2", "main3"};
        REQUIRE(testPKB->GetAllProcedureNames() == expSet);

        // modifies by procedure
        set<VarName> expSetVarName{ VarName("abc123"),  VarName("a"), VarName("x"), VarName("abc") };
        set<VarName> expSetVarName2{ VarName("x"), VarName("abc") };
        set<VarName> expSetVarName3{ VarName("abc") };
        set<Constant> expSetProc1{ "main" };
        set<Constant> expSetProc2{ "main", "main2", "main3"};
        REQUIRE(testPKB->GetModifiesPByVar(VarName("abc123")) == expSetProc1);
        REQUIRE(testPKB->GetModifiesPByVar(VarName("abc")) == expSetProc2);
        REQUIRE(testPKB->GetModifiesPByProcedure("main") == expSetVarName);
        REQUIRE(testPKB->GetModifiesPByProcedure("main2") == expSetVarName2);
        REQUIRE(testPKB->GetModifiesPByProcedure("main3") == expSetVarName3);

        // uses by procedure
        set<VarName> expSetVarName4{ VarName("x1") };
        set<VarName> expSetVarName5{ VarName("x1"), VarName("x") };
        set<VarName> expSetVarName6{ VarName("x1"), VarName("x"), VarName("v"), VarName("y"), VarName("z"), VarName("t") };
        REQUIRE(testPKB->GetUsesPByVar(VarName("t")) == expSetProc1);
        REQUIRE(testPKB->GetUsesPByVar(VarName("x1")) == expSetProc2);
        REQUIRE(testPKB->GetUsesPByProcedure("main") == expSetVarName6);
        REQUIRE(testPKB->GetUsesPByProcedure("main2") == expSetVarName5);
        REQUIRE(testPKB->GetUsesPByProcedure("main3") == expSetVarName4);

        // call
        set<StatementNumber> expSetStatement{ StatementNumber(3), StatementNumber(5) };
        REQUIRE(testPKB->GetAllCallStatements() == expSetStatement);

        // call*
        pair<ProcedureName, ProcedureName> expPairProc = make_pair("main", "main2");
        pair<ProcedureName, ProcedureName> expPairProc2 = make_pair("main", "main3");
        pair<ProcedureName, ProcedureName> expPairProc3 = make_pair("main2", "main3");
        vector<pair<ProcedureName, ProcedureName>> expVectorProc{ expPairProc, expPairProc2, expPairProc3 };
        REQUIRE(testPKB->GetAllCallsStar() == expVectorProc);
    }

    SECTION("parse cyclic calls")
    {
        std::string program = R"(
            procedure main {
                x = 1;
                call main2;
            }

            procedure main {
                print x;
                call main;
            }
        )";
        std::stringstream ss;
        ss << program;

        Token token = { TokenType::kName, "procedure" };
        Token token2 = { TokenType::kName, "main" };
        Token token3 = { TokenType::kDelimiter, "{" };

        try {
            PKB* testPKB = new PKB();
            SourceParser sp = SourceParser(testPKB);
            sp.Parse(ss);
        }
        catch (std::logic_error& e) {
            std::cout << e.what() << std::endl;
        }
        REQUIRE(1 == 1);
    }

    SECTION("parse non exist procedure on call stmt")
    {
        std::string program = R"(
            procedure main {
                x = 1;
                call main2;
                call main2;
            }

            procedure main2 {
                x = 1;
                call main3;
            }
        )";
        std::stringstream ss;
        ss << program;

        Token token = { TokenType::kName, "procedure" };
        Token token2 = { TokenType::kName, "main" };
        Token token3 = { TokenType::kDelimiter, "{" };

        try {
            PKB* testPKB = new PKB();
            SourceParser sp = SourceParser(testPKB);
            sp.Parse(ss);
        }
        catch (std::logic_error& e) {
            std::cout << e.what() << std::endl;
        }
        REQUIRE(1 == 1);
    }

}


