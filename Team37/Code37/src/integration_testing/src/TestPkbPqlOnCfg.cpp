#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <sstream>
#include <set>

#include "../src/PKB/PKB.h"
#include "../src/QueryProcessing/QueryProcessor.h"

#include "catch.hpp"

using namespace std;

PKB* initialisePkbForCfg() {
    PKB* testPKB = new PKB();

    /*procedure First{
          read x;
          read z;
          call Second; }

      procedure Second{
01        x = 0;
02        i = 5;
03        while (i != 0) {
04            x = x + 2 * y;
05            call Third;
06            i = i - 1;
          }
07        if (x == 1) then {
08            x = x + 1; }
          else {
09            z = 1;
          }
10        z = z + x + i;
11        y = z + 2;
12        x = x * y + z; }

      procedure Third{
          z = 5;
          v = z;
          print v; }*/

          // insert statement types
    testPKB->InsertReadStatement(StatementNumber(1), VarName("x"));
    testPKB->InsertReadStatement(StatementNumber(2), VarName("z"));
    testPKB->InsertCallStatement(StatementNumber(3), "Second");
    testPKB->InsertAssign(StatementNumber(4), "0|");
    testPKB->InsertAssign(StatementNumber(5), "1|");
    testPKB->InsertWhileStatement(StatementNumber(6), VarName("i"));
    testPKB->InsertAssign(StatementNumber(7), "x|2|y|*+");
    testPKB->InsertCallStatement(StatementNumber(8), "Third");
    testPKB->InsertAssign(StatementNumber(9), "i|1|-");
    testPKB->InsertIfStatement(StatementNumber(10), VarName("x"));
    testPKB->InsertAssign(StatementNumber(11), "x|1|+");
    testPKB->InsertAssign(StatementNumber(12), "1|");
    testPKB->InsertAssign(StatementNumber(13), "z|x|+i|+");
    testPKB->InsertAssign(StatementNumber(14), "z|2|+");
    testPKB->InsertAssign(StatementNumber(15), "x|y|*z|+");
    testPKB->InsertAssign(StatementNumber(16), "5|");
    testPKB->InsertAssign(StatementNumber(17), "z|");
    testPKB->InsertPrintStatement(StatementNumber(18), VarName("v"));

    // insert procedure: First, Second, Third
    testPKB->InsertProcedureName(ProcedureName("First"));
    testPKB->InsertProcedureName(ProcedureName("Second"));
    testPKB->InsertProcedureName(ProcedureName("Third"));

    testPKB->SetProcedure(ProcedureName("First"), 1, 3);
    testPKB->SetProcedure(ProcedureName("Second"), 4, 15);
    testPKB->SetProcedure(ProcedureName("Third"), 16, 18);

    // insert variables v, x, y, z, i
    testPKB->InsertVariable(VarName("v"));
    testPKB->InsertVariable(VarName("x"));
    testPKB->InsertVariable(VarName("y"));
    testPKB->InsertVariable(VarName("z"));
    testPKB->InsertVariable(VarName("i"));

    // insert constants
    testPKB->InsertConstant(StatementNumber(4), Constant("0"));
    testPKB->InsertConstant(StatementNumber(5), Constant("5"));
    testPKB->InsertConstant(StatementNumber(6), Constant("0"));
    testPKB->InsertConstant(StatementNumber(7), Constant("2"));
    testPKB->InsertConstant(StatementNumber(9), Constant("1"));
    testPKB->InsertConstant(StatementNumber(10), Constant("1"));
    testPKB->InsertConstant(StatementNumber(11), Constant("1"));
    testPKB->InsertConstant(StatementNumber(12), Constant("1"));
    testPKB->InsertConstant(StatementNumber(14), Constant("2"));
    testPKB->InsertConstant(StatementNumber(16), Constant("5"));

    // insert uses relationship
    testPKB->InsertUses(StatementNumber(6), VarName("i"));
    testPKB->InsertUses(StatementNumber(7), VarName("x"));
    testPKB->InsertUses(StatementNumber(7), VarName("y"));
    testPKB->InsertUses(StatementNumber(9), VarName("i"));
    testPKB->InsertUses(StatementNumber(10), VarName("x"));
    testPKB->InsertUses(StatementNumber(11), VarName("x"));
    testPKB->InsertUses(StatementNumber(13), VarName("x"));
    testPKB->InsertUses(StatementNumber(13), VarName("z"));
    testPKB->InsertUses(StatementNumber(14), VarName("z"));
    testPKB->InsertUses(StatementNumber(15), VarName("x"));
    testPKB->InsertUses(StatementNumber(15), VarName("y"));
    testPKB->InsertUses(StatementNumber(15), VarName("z"));
    testPKB->InsertUses(StatementNumber(18), VarName("v"));

    testPKB->InsertUsesP(ProcedureName("First"), VarName("i"));
    testPKB->InsertUsesP(ProcedureName("First"), VarName("x"));
    testPKB->InsertUsesP(ProcedureName("First"), VarName("y"));
    testPKB->InsertUsesP(ProcedureName("First"), VarName("z"));
    testPKB->InsertUsesP(ProcedureName("First"), VarName("v"));
    testPKB->InsertUsesP(ProcedureName("Second"), VarName("i"));
    testPKB->InsertUsesP(ProcedureName("Second"), VarName("x"));
    testPKB->InsertUsesP(ProcedureName("Second"), VarName("y"));
    testPKB->InsertUsesP(ProcedureName("Second"), VarName("z"));
    testPKB->InsertUsesP(ProcedureName("Second"), VarName("v"));
    testPKB->InsertUsesP(ProcedureName("Third"), VarName("v"));

    // insert modifies relationship
    testPKB->InsertModifies(StatementNumber(1), VarName("x"));
    testPKB->InsertModifies(StatementNumber(2), VarName("z"));
    testPKB->InsertModifies(StatementNumber(4), VarName("x"));
    testPKB->InsertModifies(StatementNumber(5), VarName("i"));
    testPKB->InsertModifies(StatementNumber(7), VarName("x"));
    testPKB->InsertModifies(StatementNumber(9), VarName("i"));
    testPKB->InsertModifies(StatementNumber(11), VarName("x"));
    testPKB->InsertModifies(StatementNumber(12), VarName("z"));
    testPKB->InsertModifies(StatementNumber(13), VarName("z"));
    testPKB->InsertModifies(StatementNumber(14), VarName("y"));
    testPKB->InsertModifies(StatementNumber(15), VarName("x"));
    testPKB->InsertModifies(StatementNumber(16), VarName("z"));
    testPKB->InsertModifies(StatementNumber(17), VarName("v"));

    testPKB->InsertModifiesP(ProcedureName("First"), VarName("x"));
    testPKB->InsertModifiesP(ProcedureName("First"), VarName("z"));
    testPKB->InsertModifiesP(ProcedureName("First"), VarName("i"));
    testPKB->InsertModifiesP(ProcedureName("First"), VarName("y"));
    testPKB->InsertModifiesP(ProcedureName("First"), VarName("v"));
    testPKB->InsertModifiesP(ProcedureName("Second"), VarName("x"));
    testPKB->InsertModifiesP(ProcedureName("Second"), VarName("i"));
    testPKB->InsertModifiesP(ProcedureName("Second"), VarName("z"));
    testPKB->InsertModifiesP(ProcedureName("Second"), VarName("y"));
    testPKB->InsertModifiesP(ProcedureName("Second"), VarName("v"));
    testPKB->InsertModifiesP(ProcedureName("Third"), VarName("z"));
    testPKB->InsertModifiesP(ProcedureName("Third"), VarName("v"));

    // insert follows relationship
    testPKB->InsertFollows(StatementNumber(1), StatementNumber(2));
    testPKB->InsertFollows(StatementNumber(2), StatementNumber(3));
    testPKB->InsertFollows(StatementNumber(4), StatementNumber(5));
    testPKB->InsertFollows(StatementNumber(5), StatementNumber(6));
    testPKB->InsertFollows(StatementNumber(7), StatementNumber(8));
    testPKB->InsertFollows(StatementNumber(8), StatementNumber(9));
    testPKB->InsertFollows(StatementNumber(6), StatementNumber(10));
    testPKB->InsertFollows(StatementNumber(10), StatementNumber(13));
    testPKB->InsertFollows(StatementNumber(13), StatementNumber(14));
    testPKB->InsertFollows(StatementNumber(14), StatementNumber(15));
    testPKB->InsertFollows(StatementNumber(16), StatementNumber(17));
    testPKB->InsertFollows(StatementNumber(17), StatementNumber(18));

    testPKB->InitFollowsStarTable();

    // insert parent relationship
    testPKB->InsertParent(StatementNumber(6), StatementNumber(7));
    testPKB->InsertParent(StatementNumber(6), StatementNumber(8));
    testPKB->InsertParent(StatementNumber(6), StatementNumber(9));
    testPKB->InsertParent(StatementNumber(10), StatementNumber(11));
    testPKB->InsertParent(StatementNumber(10), StatementNumber(12));

    testPKB->InitParentStarTable();

    // insert calls relationship
    testPKB->InsertCalls(ProcedureName("First"), ProcedureName("Second"));
    testPKB->InsertCalls(ProcedureName("Second"), ProcedureName("Third"));

    testPKB->SetMaxStmtNumber(18);
    testPKB->InitCallsStarTable();
    testPKB->UpdateNestedModifiesP();
    testPKB->UpdateNestedUsesP();
    testPKB->UpdateUsesForCallStatements();
    testPKB->UpdateModifiesForCallStatements();
    testPKB->InitNextTable();

    return testPKB;
}

bool containsAns(list<string>& lst, string str) {
    auto iter = lst.begin();
    for (iter; iter != lst.end(); iter++) {
        if (*iter == str) {
            return true;
        }
    }
    return false;
}

TEST_CASE("Next") {
    PKB* pkb = initialisePkbForCfg();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("next") {
        // select next: syn, const
        std::string query = R"(
          stmt s;
          Select s such that Next(s, 2)
        )";

        // select next: const, syn
        std::string query2 = R"(
          stmt s;
          Select s such that Next(2, s)
        )";

        // select next: syn, syn
        std::string query3 = R"(
          stmt s, s1;
          Select s such that Next(s, s1)
        )";

        // select next: syn, syn
        std::string query4 = R"(
          stmt s, s1;
          Select s such that Next(s, s)
        )";

        // select next: const, const
        std::string query5 = R"(
          stmt s;
          Select s such that Next(1, 2)
        )";

        // select next: _, _
        std::string query6 = R"(
          stmt s;
          Select s such that Next(_, _)
        )";

        // select next: syn, _
        std::string query7 = R"(
          stmt s;
          Select s such that Next(s, _)
        )";

        // select next: _, syn
        std::string query8 = R"(
          stmt s;
          Select s such that Next(_, s)
        )";


        list<string> query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);

        list<string> query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 1);

        list<string> query_result3 = qp->ProcessQuery(query3);
        list<string> exp = { "1", "2", "4", "5", "6", "6", "7", "8", "9", "10", "10", "11", "12", "13", "14", "16", "17" };
        REQUIRE(query_result3.size() == 17);

        list<string> query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.size() == 0);

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 18);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 18);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 15);

        list<string> query_result8 = qp->ProcessQuery(query8);
        list<string> exp2 = { "2", "3", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "17", "18" };
        REQUIRE(query_result8.size() == 15);

        // invalid query
        std::string query9 = R"(
          stmt s;
          Select s such that Next(, s)
        )";

        // invalid query
        std::string query10 = R"(
          variable v;
          Select v such that Next(v, s)
        )";

        REQUIRE(qp->ProcessQuery(query9).size() == 0);
        REQUIRE(qp->ProcessQuery(query10).size() == 0);

    }

    
}

TEST_CASE("Next*") {
    PKB* pkb = initialisePkbForCfg();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("next*") {
        // select nextStar: syn, const
        std::string query = R"(
          stmt s;
          Select s such that Next*(s, 2)
        )";
        list<string> query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);

        // select nextStar: const, syn
        std::string query2 = R"(
          stmt s;
          Select s such that Next*(2, s)
        )";

        list<string> query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 1);

        // select nextStar: syn, syn
        std::string query3 = R"(
          stmt s, s1;
          Select s such that Next*(s, s1)
        )";

        list<string> query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 81);
        list<string> exp = {
            "1","1",
            "2",
            "4","4","4","4","4","4","4","4","4","4","4",
            "5","5","5","5","5","5","5","5","5","5",
            "6","6","6","6","6","6","6","6","6","6",
            "7","7","7","7","7","7","7","7","7","7",
            "8","8","8","8","8","8","8","8","8","8",
            "9","9","9","9","9","9","9","9","9","9",
            "10","10","10","10","10",
            "11","11","11",
            "12","12","12",
            "13","13",
            "14",
            "16","16",
            "17" };

        // select nextStar: syn, syn
        std::string query4 = R"(
          stmt s, s1;
          Select s such that Next*(s, s)
        )";

        // select nextStar: const, const
        std::string query5 = R"(
          stmt s;
          Select s such that Next*(1, 2)
        )";

        // select nextStar: _, _
        std::string query6 = R"(
          stmt s;
          Select s such that Next*(_, _)
        )";

        // select nextStar: syn, _
        std::string query7 = R"(
          stmt s;
          Select s such that Next*(s, _)
        )";

        // select nextStar: _, syn
        std::string query8 = R"(
          stmt s;
          Select s such that Next*(_, s)
        )";

        list<string> query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.size() == 4);

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 18);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 18);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 15);

        list<string> query_result8 = qp->ProcessQuery(query8);
        REQUIRE(query_result8.size() == 15);

        // invalid query
        std::string query9 = R"(
          stmt s;
          Select s such that Next*(, s)
        )";

        // invalid query
        std::string query10 = R"(
          variable v;
          Select v such that Next*(v, s)
        )";

        REQUIRE(qp->ProcessQuery(query9).size() == 0);
        REQUIRE(qp->ProcessQuery(query10).size() == 0);

        std::string query11 = R"(
          stmt s1; 
          Select BOOLEAN such that Next* (2, 3)
        )";

        list<string> query_result11 = qp->ProcessQuery(query11);
        list<string> true_result11 = { "TRUE" };
        REQUIRE(query_result11 == true_result11);

        std::string query12 = R"(
          stmt s1, s2;
          Select BOOLEAN such that Next* (s1, s2)
        )";

        list<string> query_result12 = qp->ProcessQuery(query12);
        list<string> true_result12 = { "TRUE" };
        REQUIRE(query_result12 == true_result12);

        std::string query13 = R"(
          stmt s1;
          Select BOOLEAN such that Next* (s1, s1)
        )";

        list<string> query_result13 = qp->ProcessQuery(query13);
        list<string> true_result13 = { "TRUE" };
        REQUIRE(query_result13 == true_result13);
    }
}

TEST_CASE("Affects") {
    PKB* pkb = initialisePkbForCfg();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("affects") {
        // select Affects: syn, const
        std::string query = R"(
          stmt s;
          Select s such that Affects(s, 7)
        )";

        // select Affects: const, syn
        std::string query2 = R"(
          stmt s;
          Select s such that Affects(4, s)
        )";

        // select Affects: syn, syn
        std::string query3 = R"(
          stmt s, s1;
          Select s such that Affects(s, s1)
        )";

        // select Affects: syn, syn
        std::string query4 = R"(
          stmt s, s1;
          Select s such that Affects(s, s)
        )";

        // select Affects: const, const
        std::string query5 = R"(
          stmt s;
          Select s such that Affects(4, 15)
        )";

        // select Affects: _, _
        std::string query6 = R"(
          stmt s;
          Select s such that Affects(_, _)
        )";

        // select Affects: syn, _
        std::string query7 = R"(
          stmt s;
          Select s such that Affects(s, _)
        )";

        // select Affects: _, syn
        std::string query8 = R"(
          stmt s;
          Select s such that Affects(_, s)
        )";

        list<string> query_result = qp->ProcessQuery(query);
        list<string> exp = { "4", "7" };
        REQUIRE(query_result.size() == 2);
        REQUIRE(query_result == exp);

        list<string> query_result2 = qp->ProcessQuery(query2);
        list<string> exp2 = { "7", "11" , "13", "15"};
        REQUIRE(query_result2.size() == 4);
        REQUIRE(query_result2 == exp2);

        list<string> query_result3 = qp->ProcessQuery(query3);
        list<string> exp3 = { "4", "4", "4", "4", "5", "7", "7", "7", "7", "9", "11", "11", "12", "13", "13", "14" };
        REQUIRE(query_result3.size() == 16);
        for (string answer : exp3) {
            REQUIRE(containsAns(query_result3, answer));
        }

        list<string> query_result4 = qp->ProcessQuery(query4);
        list<string> exp4 = { "7", "9" };
        REQUIRE(query_result4.size() == 2);
        for (string answer : exp4) {
            REQUIRE(containsAns(query_result4, answer));
        }

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 18);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 18);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 8);

        list<string> query_result8 = qp->ProcessQuery(query8);
        list<string> exp8 = { "7", "11", "13", "14", "15", "9" };
        REQUIRE(query_result8.size() == 6);
        for (string answer : exp8) {
            REQUIRE(containsAns(query_result8, answer));
        }

        // invalid query
        std::string query9 = R"(
          stmt s;
          Select s such that Affects(, s)
        )";

        // invalid query
        std::string query10 = R"(
          variable v; stmt s;
          Select v such that Affects(v, s)
        )";

        // invalid query
        std::string query11 = R"(
          if ifs; stmt s;
          Select ifs such that Affects(ifs, s)
        )";

        REQUIRE(qp->ProcessQuery(query9).size() == 0);
        REQUIRE(qp->ProcessQuery(query10).size() == 0);
        REQUIRE(qp->ProcessQuery(query11).size() == 0);

    }
    SECTION("ASSIGN") {
        // select Affects: syn, const
        std::string query = R"(
          assign a;
          Select a such that Affects(a, 7)
        )";

        // select Affects: const, syn
        std::string query2 = R"(
          assign a;
          Select a such that Affects(4, a)
        )";

        // select Affects: syn, syn
        std::string query3 = R"(
          assign a, a1;
          Select a such that Affects(a, a1)
        )";

        // select Affects: syn, syn
        std::string query4 = R"(
          assign a, a1;
          Select a such that Affects(a, a)
        )";

        // select Affects: const, const
        std::string query5 = R"(
          assign a;
          Select a such that Affects(4, 15)
        )";

        // select Affects: _, _
        std::string query6 = R"(
          assign a;
          Select a such that Affects(_, _)
        )";

        // select Affects: syn, _
        std::string query7 = R"(
          assign a;
          Select a such that Affects(a, _)
        )";

        // select Affects: _, syn
        std::string query8 = R"(
          assign a;
          Select a such that Affects(_, a)
        )";

        list<string> query_result = qp->ProcessQuery(query);
        list<string> exp = { "4", "7" };
        REQUIRE(query_result.size() == 2);
        REQUIRE(query_result == exp);

        list<string> query_result2 = qp->ProcessQuery(query2);
        list<string> exp2 = { "7", "11" , "13", "15" };
        REQUIRE(query_result2.size() == 4);
        REQUIRE(query_result2 == exp2);

        list<string> query_result3 = qp->ProcessQuery(query3);
        list<string> exp3 = { "4", "4", "4", "4", "5", "7", "7", "7", "7", "9", "11", "11", "12", "13", "13", "14" };
        REQUIRE(query_result3.size() == 16);
        for (string answer : exp3) {
            REQUIRE(containsAns(query_result3, answer));
        }

        list<string> query_result4 = qp->ProcessQuery(query4);
        list<string> exp4 = { "7", "9"};
        REQUIRE(query_result4.size() == 2);
        for (string answer : exp4) {
            REQUIRE(containsAns(query_result4, answer));
        }

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 11);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 11);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 8);

        list<string> query_result8 = qp->ProcessQuery(query8);
        list<string> exp8 = { "7", "11", "13", "14", "15", "9"};
        REQUIRE(query_result8.size() == 6);
        for (string answer : exp8) {
            REQUIRE(containsAns(query_result8, answer));
        }

        // select Affects: _, syn
        std::string query9 = R"(
          assign a;
          Select a such that Affects(1, a)
        )";
        list<string> query_result9 = qp->ProcessQuery(query9);
        REQUIRE(query_result9.size() == 0);
    }
}

TEST_CASE("Affects*") {
    PKB* pkb = initialisePkbForCfg();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("affects*") {
        // select Affects: syn, const
        std::string query = R"(
          stmt s;
          Select s such that Affects*(s, 7)
        )";

        // select Affects: const, syn
        std::string query2 = R"(
          stmt s;
          Select s such that Affects*(4, s)
        )";

        // select Affects: syn, syn
        std::string query3 = R"(
          stmt s, s1;
          Select s such that Affects*(s, s1)
        )";

        // select Affects: syn, syn
        std::string query4 = R"(
          stmt s, s1;
          Select s such that Affects*(s, s)
        )";

        // select Affects: const, const
        std::string query5 = R"(
          stmt s;
          Select s such that Affects*(4, 15)
        )";

        // select Affects: _, _
        std::string query6 = R"(
          stmt s;
          Select s such that Affects*(_, _)
        )";

        // select Affects: syn, _
        std::string query7 = R"(
          stmt s;
          Select s such that Affects*(s, _)
        )";

        // select Affects: _, syn
        std::string query8 = R"(
          stmt s;
          Select s such that Affects*(_, s)
        )";

        list<string> query_result = qp->ProcessQuery(query);
        list<string> exp = { "4", "7" };
        REQUIRE(query_result.size() == 2);
        REQUIRE(query_result == exp);

        list<string> query_result2 = qp->ProcessQuery(query2);
        list<string> exp2 = { "7", "11", "13", "14", "15" };
        REQUIRE(query_result2.size() == 5);
        REQUIRE(query_result2 == exp2);

        list<string> query_result3 = qp->ProcessQuery(query3);
        list<string> exp3 = { "4", "4", "4", "4", "4", "5", "7", "7", "7", "7", "7", "9", "11", "11", "11", "12", "12", "12", "13", "13", "14" };
        REQUIRE(query_result3.size() == 21);
        for (string answer : exp3) {
            REQUIRE(containsAns(query_result3, answer));
        }

        list<string> query_result4 = qp->ProcessQuery(query4);
        list<string> exp4 = { "7", "9" };
        REQUIRE(query_result4.size() == 2);
        for (string answer : exp4) {
            REQUIRE(containsAns(query_result4, answer));
        }

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 18);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 18);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 8);

        list<string> query_result8 = qp->ProcessQuery(query8);
        list<string> exp8 = { "7", "11", "13", "14", "15", "9"};
        REQUIRE(query_result8.size() == 6);
        for (string answer : exp8) {
            REQUIRE(containsAns(query_result8, answer));
        }

        // invalid query
        std::string query9 = R"(
          stmt s;
          Select s such that Affects*(, s)
        )";

        // invalid query
        std::string query10 = R"(
          variable v; stmt s;
          Select v such that Affects*(v, s)
        )";

        // invalid query
        std::string query11 = R"(
          if ifs; stmt s;
          Select ifs such that Affects*(ifs, s)
        )";

        REQUIRE(qp->ProcessQuery(query9).size() == 0);
        REQUIRE(qp->ProcessQuery(query10).size() == 0);
        REQUIRE(qp->ProcessQuery(query11).size() == 0);

    }

    SECTION("ASSIGN") {
        // select Affects: syn, const
        std::string query = R"(
          assign a;
          Select a such that Affects*(a, 7)
        )";

        // select Affects: const, syn
        std::string query2 = R"(
          assign a;
          Select a such that Affects*(4, a)
        )";

        // select Affects: syn, syn
        std::string query3 = R"(
          assign a, a1;
          Select a such that Affects*(a, a1)
        )";

        // select Affects: syn, syn
        std::string query4 = R"(
          assign a, a1;
          Select a such that Affects*(a, a)
        )";

        // select Affects: const, const
        std::string query5 = R"(
          assign a;
          Select a such that Affects*(4, 15)
        )";

        // select Affects: _, _
        std::string query6 = R"(
          assign a;
          Select a such that Affects*(_, _)
        )";

        // select Affects: syn, _
        std::string query7 = R"(
          assign a;
          Select a such that Affects*(a, _)
        )";

        // select Affects: _, syn
        std::string query8 = R"(
          assign a;
          Select a such that Affects*(_, a)
        )";

        list<string> query_result = qp->ProcessQuery(query);
        list<string> exp = { "4", "7" };
        REQUIRE(query_result.size() == 2);
        REQUIRE(query_result == exp);

        list<string> query_result2 = qp->ProcessQuery(query2);
        list<string> exp2 = { "7", "11", "13", "14", "15" };
        REQUIRE(query_result2.size() == 5);
        REQUIRE(query_result2 == exp2);

        list<string> query_result3 = qp->ProcessQuery(query3);
        list<string> exp3 = { "4", "4", "4", "4", "4", "5", "7", "7", "7", "7", "7", "9", "11", "11", "11", "12", "12", "12", "13", "13", "14" };
        REQUIRE(query_result3.size() == 21);
        for (string answer : exp3) {
            REQUIRE(containsAns(query_result3, answer));
        }

        list<string> query_result4 = qp->ProcessQuery(query4);
        list<string> exp4 = { "7", "9" };
        REQUIRE(query_result4.size() == 2);
        for (string answer : exp4) {
            REQUIRE(containsAns(query_result4, answer));
        }

        list<string> query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 11);

        list<string> query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 11);

        list<string> query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 8);

        list<string> query_result8 = qp->ProcessQuery(query8);
        list<string> exp8 = { "7", "11", "13", "14", "15", "9" };
        REQUIRE(query_result8.size() == 6);
        for (string answer : exp8) {
            REQUIRE(containsAns(query_result8, answer));
        }

        // select Affects: _, syn
        std::string query9 = R"(
          assign a;
          Select a such that Affects*(1, a)
        )";
        list<string> query_result9 = qp->ProcessQuery(query9);
        REQUIRE(query_result9.size() == 0);
    }
}