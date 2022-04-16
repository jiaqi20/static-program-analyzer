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

PKB* initialisePkb() {
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

bool contains(list<string>& lst, string str) {
    auto iter = lst.begin();
    for (iter; iter != lst.end(); iter++) {
        if (*iter == str) {
            return true;
        }
    }
    return false;
}

TEST_CASE("PKB and PQL - basic select query without clause") {

    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("Get stmt") {
        std::string query = R"(
        stmt s;
        Select s
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 18);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
        REQUIRE(contains(query_result, "3"));
        REQUIRE(contains(query_result, "4"));
        REQUIRE(contains(query_result, "5"));
        REQUIRE(contains(query_result, "6"));
        REQUIRE(contains(query_result, "7"));
        REQUIRE(contains(query_result, "8"));
        REQUIRE(contains(query_result, "9"));
        REQUIRE(contains(query_result, "10"));
        REQUIRE(contains(query_result, "11"));
        REQUIRE(contains(query_result, "12"));
        REQUIRE(contains(query_result, "13"));
        REQUIRE(contains(query_result, "14"));
        REQUIRE(contains(query_result, "15"));
        REQUIRE(contains(query_result, "16"));
        REQUIRE(contains(query_result, "17"));
        REQUIRE(contains(query_result, "18"));
    }

    SECTION("Get call stmt") {
        std::string query = R"(
        call cl;
        Select cl
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "3"));
        REQUIRE(contains(query_result, "8"));
    }

    SECTION("Get while stmt") {
        std::string query = R"(
        while w;
        Select w
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }


    SECTION("Get read stmt") {
        std::string query = R"(
        read r;
        Select r
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("Get print stmt") {
        std::string query = R"(
        print pn;
        Select pn
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "18"));
    }

    SECTION("Get if stmt") {
        std::string query = R"(
        if ifs;
        Select ifs
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "10"));
    }

    SECTION("Get procedure") {
        std::string query = R"(
        procedure p;
        Select p
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
        REQUIRE(contains(query_result, "First"));
        REQUIRE(contains(query_result, "Second"));
        REQUIRE(contains(query_result, "Third"));
    }
}

SCENARIO("clause either return false or true") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("false clause") {
        std::string query = R"(
        stmt s;
        Select s such that Modifies(1, "z")
        )";
    
        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.empty());
    }

    SECTION("true caluse") {
        std::string query = R"(
        read r;
        Select r such that Modifies(r, "x")
        )";
   
        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "1"));
    }
}

SCENARIO("target in clause, one clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    WHEN("modifies") {
        SECTION("const, syn") {
            std::string query = R"(
            variable v;
            Select v such that Modifies(2, v)
            )";

            list<string> query_result;
            query_result = qp->ProcessQuery(query);
            REQUIRE(query_result.size() == 1);
            REQUIRE(contains(query_result, "z"));
        }
    }
    WHEN("parent") {
        SECTION("syn, syn") {
            std::string query = R"(
            stmt s; if ifs;
            Select s such that Parent(ifs, s)
            )";
            
            list<string> query_result;
            query_result = qp->ProcessQuery(query);
            REQUIRE(query_result.size() == 2);
            REQUIRE(contains(query_result, "11"));
            REQUIRE(contains(query_result, "12"));
        }
    }
}

SCENARIO("Calls clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("(syn, name)") {
        std::string query = R"(
        procedure p;
        Select p such that Calls(p, "Second")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "First"));
    }
    SECTION("(syn, syn)") {
        std::string query = R"(
        procedure p1, p2;
        Select p2 such that Calls(p1, p2)
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "Second"));
        REQUIRE(contains(query_result, "Third"));
    }

    SECTION("wildcard") {
        std::string query = R"(
        procedure p;
        Select p such that Calls(_,_)
        )";
        
        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
        REQUIRE(contains(query_result, "First"));
        REQUIRE(contains(query_result, "Second"));
        REQUIRE(contains(query_result, "Third"));
    }
}

TEST_CASE("PKB and PQL - select query with one clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);
    
    SECTION("modifies") {
        // select modifies
        std::string query = R"(
          variable v;
          Select v such that Modifies(1, v)
          )";

        list<string> query_result;
        qp->ProcessQuery(query);
        //REQUIRE(query_result.size() == 1);
        //REQUIRE(contains(query_result, "x"));
    }

    SECTION("follows") {
        // select follows
        std::string query = R"(
          stmt s;
          Select s such that Follows(s, 2)
        )";

        list<string> query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);

    }

}

TEST_CASE("clause either return false or true") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("false clause") {
        std::string query = R"(
        stmt s;
        Select s such that Modifies(1, "z")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.empty());
    }

    SECTION("true caluse") {
        std::string query = R"(
        stmt s;
        Select s such that Modifies(1, "x")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 18);
        REQUIRE(contains(query_result, "18"));
    }
}

TEST_CASE("target in clause, one clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    WHEN("modifies") {
        SECTION("const, syn") {
            std::string query = R"(
            variable v;
            Select v such that Modifies(2, v)
            )";

            list<string> query_result;
            query_result = qp->ProcessQuery(query);
            REQUIRE(query_result.size() == 1);
            REQUIRE(contains(query_result, "z"));
        }
    }
    WHEN("parent") {
        SECTION("syn, syn") {
            std::string query = R"(
            stmt s; if ifs;
            Select s such that Parent(ifs, s)
            )";
            
            list<string> query_result;
            query_result = qp->ProcessQuery(query);
            REQUIRE(query_result.size() == 2);
            REQUIRE(contains(query_result, "11"));
            REQUIRE(contains(query_result, "12"));
        }
    }
}

TEST_CASE("Calls clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("(syn, name)") {
        std::string query = R"(
        procedure p;
        Select p such that Calls(p, "Second")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "First"));
    }
    SECTION("(syn, syn)") {
        std::string query = R"(
        procedure p1, p2;
        Select p2 such that Calls(p1, p2)
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("procedure with true") {
        std::string query = R"(
        procedure p;
        Select p with p.procName = "First"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "First"));
    }

    SECTION("procedure with false") {
        std::string query = R"(
        procedure p;
        Select p with p.procName = "Fi"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("wildcard") {
        std::string query = R"(
        procedure p;
        Select p such that Calls(_,_)
        )";
        

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
        REQUIRE(contains(query_result, "First"));
        REQUIRE(contains(query_result, "Second"));
        REQUIRE(contains(query_result, "Third"));
    }
}

TEST_CASE("PKB and PQL - basic with clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("read with false") {
        std::string query = R"(
        read r;
        Select r with 4 = 3
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with "test" = "test"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
    }


    SECTION("read with true") {
        std::string query = R"(
        read r; procedure p;
        Select r with p.procName = p.procName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.stmt# = r.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.stmt# = 1
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "1"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.varName = r.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
        REQUIRE(contains(query_result, "1"));
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.varName = "x"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "1"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.varName = "z"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with "z" = r.varName 
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "2"));
    }

    SECTION("read with true") {
        std::string query = R"(
        read r;
        Select r with r.varName = "c"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }


    // Print
    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with pn.stmt# = 18
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "18"));
    }

    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with 18 = pn.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "18"));
    }

    SECTION("print with false") {
        std::string query = R"(
        print pn;
        Select pn with pn.stmt# = 11
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with "x" = pn.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("print with false") {
        std::string query = R"(
        print pn;
        Select pn with pn.varName = "x"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with pn.varName = "v"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
    }

    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with pn.stmt# = pn.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "18"));
    }

    SECTION("print with true") {
        std::string query = R"(
        print pn;
        Select pn with pn.varName = pn.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "18"));
    }

    // While
    SECTION("while with true") {
        std::string query = R"(
        while w;
        Select w with w.stmt# = 6
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }

    SECTION("while with true") {
        std::string query = R"(
        while w;
        Select w with 6 = w.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }

    SECTION("while with true") {
        std::string query = R"(
        while w;
        Select w with 2 = 2
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }

    SECTION("while with true") {
        std::string query = R"(
        while w;
        Select w with "test" = "test"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }

    SECTION("while with false") {
        std::string query = R"(
        while w;
        Select w with w.stmt# = 2
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("while with true") {
        std::string query = R"(
        while w;
        Select w with w.stmt# = w.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "6"));
    }

    // If
    SECTION("If with true") {
        std::string query = R"(
        if ifs;
        Select ifs with ifs.stmt# = 10
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "10"));
    }

    SECTION("If with true") {
        std::string query = R"(
        if ifs;
        Select ifs with 10 = ifs.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "10"));
    }

    SECTION("If with false") {
        std::string query = R"(
        if ifs;
        Select ifs with 2 = ifs.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("If with true") {
        std::string query = R"(
        if ifs;
        Select ifs with ifs.stmt# = ifs.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "10"));
    }

    // Assign
    SECTION("If with true") {
        std::string query = R"(
        assign a;
        Select a with a.stmt# = 5
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "5"));
    }

    SECTION("If with true") {
        std::string query = R"(
        assign a;
        Select a with 5 = a.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "5"));
    }

    SECTION("If with false") {
        std::string query = R"(
        assign a;
        Select a with a.stmt# = 1
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("If with true") {
        std::string query = R"(
        assign a;
        Select a with a.stmt# = a.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 11);
    }

    // Stmt
    SECTION("Stmt with true") {
        std::string query = R"(
        stmt s;
        Select s with s.stmt# = 5
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "5"));
    }

    SECTION("Stmt with false") {
        std::string query = R"(
        stmt s;
        Select s with s.stmt# = 20
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("Stmt with true") {
        std::string query = R"(
        stmt s;
        Select s with s.stmt# = s.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 18);
    }

    // Call
    SECTION("Call with true") {
        std::string query = R"(
        call cl;
        Select cl with cl.stmt# = 3
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "3"));
    }

    SECTION("Call with true") {
        std::string query = R"(
        call cl;
        Select cl with 8 = cl.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "8"));
    }

    SECTION("Call with true") {
        std::string query = R"(
        call cl;
        Select cl with "Second" = cl.procName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "3"));
    }

    SECTION("Call with false") {
        std::string query = R"(
        call cl;
        Select cl with "First" = cl.procName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("Call with true") {
        std::string query = R"(
        call cl;
        Select cl with cl.stmt# = cl.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }
    
    // Variable
    SECTION("variable with true") {
        std::string query = R"(
        variable v;
        Select v with v.varName = "x"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "x"));
    }

    SECTION("variable with true") {
        std::string query = R"(
        variable v;
        Select v with "v" = v.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "v"));
    }

    SECTION("variable with false") {
        std::string query = R"(
        variable v;
        Select v with v.varName = "p"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("variable with true") {
        std::string query = R"(
        variable v;
        Select v with v.varName = v.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 5);
    }

    // Constant
    SECTION("constant with true") {
        std::string query = R"(
        constant c;
        Select c with c.value = 5
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "5"));
    }

    SECTION("constant with true") {
        std::string query = R"(
        constant c;
        Select c with 2 = c.value
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        REQUIRE(contains(query_result, "2"));
    }
    
    SECTION("constant with  false") {
        std::string query = R"(
        constant c;
        Select c with c.value = 10
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("constant with true") {
        std::string query = R"(
        constant c;
        Select c with c.value = c.value
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 4);
    }

    SECTION("constant with true") {
        std::string query = R"(
        constant c; stmt s;
        Select c with c.value = s.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
    }

    SECTION("constant with true") {
        std::string query = R"(
        constant c; stmt s;
        Select s with c.value = s.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
    }

    SECTION("modifies with true") {
        std::string query = R"(
        stmt s; variable v;
        Select s such that Modifies(s, v) with v.varName = "x"
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 6);
    }    
}

TEST_CASE("PKB and PQL - complex with clause") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("stmt with true") {
        std::string query = R"(
        stmt s; read r; 
        Select s with s.stmt# = r.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("stmt with true") {
        std::string query = R"(
        stmt s; constant c; 
        Select s with s.stmt# = c.value
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 3);
    }

    SECTION("read with true") {
        std::string query = R"(
        stmt s; read r; 
        Select r with s.stmt# = r.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("read with true") {
        std::string query = R"(
        stmt s; read r; 
        Select r with r.stmt# = s.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("read with true") {
        std::string query = R"(
        read r; variable v;
        Select r with r.varName = v.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }
    
    SECTION("read with true") {
        std::string query = R"(
        read r; variable v;
        Select r with v.varName = r.varName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("read with true") {
        std::string query = R"(
        call cl; read r; 
        Select r with r.stmt# = cl.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("read with true") {
        std::string query = R"(
        call cl; read r; 
        Select r with r.stmt# = cl.stmt#
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);
    }

    SECTION("proc and call with true") {
        std::string query = R"(
        procedure p, q; call c; 
        Select p with p.procName = c.procName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    SECTION("proc and call with true") {
        std::string query = R"(
        procedure p, q; call c; 
        Select p with c.procName = p.procName
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 2);
    }

    //SECTION("proc call with true") {
    //    std::string query = R"(
    //    procedure p, q; call c; 
    //    Select c.procName with c.procName = p.procName
    //    )";

    //    list<string> query_result;
    //    query_result = qp->ProcessQuery(query);
    //    REQUIRE(query_result.size() == 2);
    //}
    
}

TEST_CASE("PKB and PQL - pattern a") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("basic") {
        std::string query = R"(
        assign a; 
        Select a pattern a("x", "x + 2 * y")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        list<string> exp = { "7" };
        REQUIRE(query_result == exp);

        std::string query2 = R"(
        assign a; 
        Select a pattern a("x", "2 * y")
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 0);

        std::string query3 = R"(
        assign a; 
        Select a pattern a("x", _"2 * y"_)
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 1);
        list<string> exp3 = { "7" };
        REQUIRE(query_result3 == exp3);

        std::string query4 = R"(
        assign a; 
        Select a pattern a("x", "2 * y"_)
        )";

        list<string> query_result4;
        query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.size() == 0);

    }
}

TEST_CASE("PKB and PQL - pattern w") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("BASIC") {
        std::string query = R"(
        while w; 
        Select w pattern w("i", _)
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        list<string> exp = { "6" };
        REQUIRE(query_result == exp);

        std::string query2 = R"(
        while w; variable v;
        Select v pattern w(v, _)
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 1);
        list<string> exp2 = { "i" };
        REQUIRE(query_result2 == exp2);

        std::string query3 = R"(
        while w; variable v;
        Select w pattern w(_, _)
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 1);
        list<string> exp3 = { "6" };
        REQUIRE(query_result3 == exp3);
    }

    SECTION("INVALID") {
        std::string query = R"(
        while w; 
        Select w pattern w ("x", "x")
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);

        std::string query2 = R"(
        while w; 
        Select w pattern w ("x", "x", "x")
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 0);

        std::string query3 = R"(
        while w; variable v;
        Select w pattern w (v, v, v)
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 0);

        std::string query4 = R"(
        while w; variable v;
        Select w pattern w (v, v)
        )";

        list<string> query_result4;
        query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.size() == 0);

        std::string query5 = R"(
        while w; variable v;
        Select w pattern w (_, v)
        )";

        list<string> query_result5;
        query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 0);
    }
}

TEST_CASE("PKB and PQL - pattern ifs") {
    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);

    SECTION("BASIC") {
        std::string query = R"(
        if ifs; 
        Select ifs pattern ifs ("x", _, _)
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 1);
        list<string> exp = { "10" };
        REQUIRE(query_result == exp);

        std::string query2 = R"(
        if ifs; variable v;
        Select v pattern ifs(v, _, _)
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 1);
        list<string> exp2 = { "x" };
        REQUIRE(query_result2 == exp2);

        std::string query3 = R"(
        if ifs; variable v;
        Select ifs pattern ifs(_, _, _)
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 1);
        list<string> exp3 = { "10" };
        REQUIRE(query_result3 == exp3);
    }

    SECTION("INVALID") {
        std::string query = R"(
        if ifs; 
        Select ifs pattern ifs ("x", _)
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);

        std::string query2 = R"(
        if ifs; 
        Select ifs pattern ifs ("x", "x")
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 0);

        std::string query3 = R"(
        if ifs; 
        Select ifs pattern ifs ("x", "x", "x")
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 0);

        std::string query4 = R"(
        if ifs; 
        Select ifs pattern ifs ("x", "x", "x", "x")
        )";

        list<string> query_result4;
        query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.size() == 0);

        std::string query5 = R"(
        if ifs; variable v;
        Select ifs pattern ifs (v, v, v, v)
        )";

        list<string> query_result5;
        query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.size() == 0);

        std::string query6 = R"(
        if ifs; variable v;
        Select ifs pattern ifs (v, v, v)
        )";

        list<string> query_result6;
        query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.size() == 0);

        std::string query7 = R"(
        if ifs; variable v;
        Select ifs pattern ifs (v, v, _)
        )";

        list<string> query_result7;
        query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 0);

        std::string query8 = R"(
        if ifs; variable v;
        Select ifs pattern ifs (v, _, v)
        )";

        list<string> query_result8;
        query_result8 = qp->ProcessQuery(query8);
        REQUIRE(query_result8.size() == 0);

        std::string query9 = R"(
        if ifs; variable v;
        Select ifs pattern ifs (v, "x", v)
        )";

        list<string> query_result9;
        query_result9 = qp->ProcessQuery(query9);
        REQUIRE(query_result9.size() == 0);

        std::string query10 = R"(
        if ifs; variable v;
        Select ifs pattern ifs ("x", "x", _)
        )";

        list<string> query_result10;
        query_result10 = qp->ProcessQuery(query10);
        REQUIRE(query_result10.size() == 0);
    }
}

TEST_CASE("semantically invalid") {
    // for boolean, invalid syntax return nothing, invalid semantics return FALSE, else same logic
    // for others, invalid syntax return nothing, invalid semantics return nothing, else same logic

    PKB* pkb = initialisePkb();
    QueryProcessor* qp = new QueryProcessor(pkb);
    
    SECTION("basic") {
        // no syn declared
        std::string query = R"(
        stmt s1;  
        Select s2
        )";

        list<string> query_result;
        query_result = qp->ProcessQuery(query);
        REQUIRE(query_result.size() == 0);

        // no bracket
        std::string query2 = R"(
        assign a;
        Select BOOLEAN pattern a("abc"
        )";

        list<string> query_result2;
        query_result2 = qp->ProcessQuery(query2);
        REQUIRE(query_result2.size() == 0);

        // invalid pattern
        std::string query3 = R"(
        assign a;
        Select BOOLEAN pattern a("abc")
        )";

        list<string> query_result3;
        query_result3 = qp->ProcessQuery(query3);
        REQUIRE(query_result3.size() == 0);

        // wiki
        std::string query4 = R"(
        variable v;
        Select BOOLEAN such that Uses(_, v)
        )";

        list<string> query_result4;
        query_result4 = qp->ProcessQuery(query4);
        REQUIRE(query_result4.front() == "FALSE");

        std::string query5 = R"(
        stmt s;
        Select BOOLEAN with "First" = s.procName
        )";

        list<string> query_result5;
        query_result5 = qp->ProcessQuery(query5);
        REQUIRE(query_result5.front() == "FALSE");

        std::string query6 = R"(
        stmt s;
        Select BOOLEAN with "First" = s.stmt#
        )";

        list<string> query_result6;
        query_result6 = qp->ProcessQuery(query6);
        REQUIRE(query_result6.front() == "FALSE");

        // invalid syntax
        std::string query7 = R"(
        procedure p;
        Select BOOLEAN with "First" == p.procName
        )";

        list<string> query_result7;
        query_result7 = qp->ProcessQuery(query7);
        REQUIRE(query_result7.size() == 0);

        // invalid semantics for affects
        std::string query8 = R"(
        read re;
        Select BOOLEAN such that Affects(re, _)
        )";

        list<string> query_result8;
        query_result8 = qp->ProcessQuery(query8);
        REQUIRE(query_result8.front() == "FALSE");

        std::string query9 = R"(
        read re;
        Select re such that Affects(re, _)
        )";

        list<string> query_result9;
        query_result9 = qp->ProcessQuery(query9);
        REQUIRE(query_result9.size() == 0);
    }  
    
}

//
//
//TEST_CASE("PKB and PQL - and clause") {
//
//}
TEST_CASE("PKB and PQL - with clause") {

}


TEST_CASE("PKB and PQL - and clause") {

}
