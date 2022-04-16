
#include <string>
#include <set>
#include <list>

#include "PKB/Attribute/StatementNumber.h"
#include "PKB/Attribute/StatementNumber.cpp"

#include "../src/PKB/Attribute/VarName.h"
#include "../src/PKB/Attribute/VarName.cpp"

#include "../src/PKB/PKB.h"
#include "../src/PKB/PKB.cpp"
#include "../src/SourceProcessor/SourceParser.cpp"
#include "../src/SourceProcessor/Lexer.cpp"
#include "../src/SourceProcessor/DesignExtractor.cpp"
#include "catch.hpp"

using namespace std;

TEST_CASE("Table and PKB - Modify table") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertModifies(StatementNumber(1), VarName("test")));
    REQUIRE(testPKB.InsertModifies(StatementNumber(1), VarName("test1")));
    REQUIRE(testPKB.InsertModifies(StatementNumber(2), VarName("test3")));
    REQUIRE(testPKB.InsertModifies(StatementNumber(2), VarName("test2")));
    REQUIRE(testPKB.InsertModifies(StatementNumber(3), VarName("test")));
    REQUIRE(!testPKB.InsertModifies(StatementNumber(2), VarName("test2")));
    set<VarName> exp1{ VarName("test"), VarName("test1") };
    REQUIRE(testPKB.GetModifiesByStatement(StatementNumber(1)) == exp1);
    set<StatementNumber> exp2{ StatementNumber(1), StatementNumber(3) };
    REQUIRE(testPKB.GetModifiesByVar(VarName("test")) == exp2);
}


TEST_CASE("Table and PKB - Use table") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertUses(StatementNumber(1), VarName("test")));
    REQUIRE(testPKB.InsertUses(StatementNumber(1), VarName("test1")));
    REQUIRE(testPKB.InsertUses(StatementNumber(2), VarName("test3")));
    REQUIRE(testPKB.InsertUses(StatementNumber(2), VarName("test2")));
    REQUIRE(testPKB.InsertUses(StatementNumber(3), VarName("test")));
    REQUIRE(!testPKB.InsertUses(StatementNumber(2), VarName("test2")));
    set<VarName> exp1{ VarName("test"), VarName("test1") };
    REQUIRE(testPKB.GetUsesByStatement(StatementNumber(1)) == exp1);
    set<StatementNumber> exp2{ StatementNumber(1), StatementNumber(3) };
    REQUIRE(testPKB.GetUsesByVar(VarName("test")) == exp2);
}

TEST_CASE("Table and PKB - Insert Follow") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertFollows(StatementNumber(1), StatementNumber(1)));
    REQUIRE(testPKB.InsertFollows(StatementNumber(2), StatementNumber(2)));
    REQUIRE(testPKB.InsertFollows(StatementNumber(3), StatementNumber(3)));
    REQUIRE(testPKB.InsertFollows(StatementNumber(3), StatementNumber(1)));
    REQUIRE(!testPKB.InsertFollows(StatementNumber(3), StatementNumber(1)));
    set<StatementNumber> expectedSet{ StatementNumber(1), StatementNumber(3) };
    REQUIRE(testPKB.GetStatementsFollows(StatementNumber(3)) == expectedSet);
    REQUIRE(testPKB.GetStatementsFollowedBy(StatementNumber(1)) == expectedSet);
}

TEST_CASE("Table and PKB - Insert Parent") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertParent(StatementNumber(1), StatementNumber(1)));
    REQUIRE(testPKB.InsertParent(StatementNumber(2), StatementNumber(2)));
    REQUIRE(testPKB.InsertParent(StatementNumber(3), StatementNumber(3)));
    REQUIRE(testPKB.InsertParent(StatementNumber(3), StatementNumber(1)));
    REQUIRE(!testPKB.InsertParent(StatementNumber(3), StatementNumber(1)));
    set<StatementNumber> expectedSet{ StatementNumber(1), StatementNumber(3) };
    REQUIRE(testPKB.GetStatementsParent(StatementNumber(3)) == expectedSet);
    REQUIRE(testPKB.GetStatementsParentedBy(StatementNumber(1)) == expectedSet);
}

TEST_CASE("Table and PKB: Parent and Parent*") {
    PKB testPKB = PKB();
    testPKB.InsertParent(StatementNumber(1), StatementNumber(2));
    testPKB.InsertParent(StatementNumber(2), StatementNumber(3));
    testPKB.InsertParent(StatementNumber(1), StatementNumber(4));
    testPKB.InitParentStarTable();
    set<StatementNumber> expectedSet{ StatementNumber(2), StatementNumber(3), StatementNumber(4) };
    REQUIRE(testPKB.GetStatementsParentStar(StatementNumber(1)) == expectedSet);
    set<StatementNumber> expectedSet2{ StatementNumber(1), StatementNumber(2) };
    REQUIRE(testPKB.GetStatementsParentedByStar(StatementNumber(3)) == expectedSet2);
    set<StatementNumber> expectedSet3{ StatementNumber(3) };
    REQUIRE(testPKB.GetStatementsParent(StatementNumber(2)) == expectedSet3);
    set<StatementNumber> expectedSet4{ StatementNumber(2) };
    REQUIRE(testPKB.GetStatementsParentedBy(StatementNumber(3)) == expectedSet4);
}

TEST_CASE("Table and PKB: Follows and Follows*") {
    PKB testPKB = PKB();
    testPKB.InsertFollows(StatementNumber(1), StatementNumber(2));
    testPKB.InsertFollows(StatementNumber(2), StatementNumber(3));
    testPKB.InsertFollows(StatementNumber(1), StatementNumber(4));
    testPKB.InitFollowsStarTable();
    set<StatementNumber> expectedSet{ StatementNumber(2), StatementNumber(3), StatementNumber(4) };
    REQUIRE(testPKB.GetStatementsFollowsStar(StatementNumber(1)) == expectedSet);
    set<StatementNumber> expectedSet2{ StatementNumber(1), StatementNumber(2) };
    REQUIRE(testPKB.GetStatementsFollowedByStar(StatementNumber(3)) == expectedSet2);
    set<StatementNumber> expectedSet3{ StatementNumber(3) };
    REQUIRE(testPKB.GetStatementsFollows(StatementNumber(2)) == expectedSet3);
    set<StatementNumber> expectedSet4{ StatementNumber(2) };
    REQUIRE(testPKB.GetStatementsFollowedBy(StatementNumber(3)) == expectedSet4);
}

TEST_CASE("Table and PKB: while statement") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(1), VarName("x")));
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(1), VarName()));
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(5), VarName("y")));
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(10), VarName("y")));
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(10), VarName("x")));
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(20), VarName()));
    set<StatementNumber> expectedSet{ StatementNumber(1), StatementNumber(5), StatementNumber(10), StatementNumber(20) };
    set<StatementNumber> expectedSet1{ StatementNumber(1), StatementNumber(5), StatementNumber(10) };
    set<StatementNumber> expectedSet2{ StatementNumber(1), StatementNumber(10) };
    set<StatementNumber> expectedSet3{ StatementNumber(5), StatementNumber(10) };
    set<VarName> expectedSet4{ VarName("x") };
    set<VarName> expectedSet5{ VarName("x"), VarName("y")};
    REQUIRE(testPKB.GetAllWhileStatements() == expectedSet);
    REQUIRE(testPKB.GetAllWhileStatementsWithControlVar() == expectedSet1);
    REQUIRE(testPKB.GetWhileStatementsByVar(VarName("x")) == expectedSet2);
    REQUIRE(testPKB.GetWhileStatementsByVar(VarName("y")) == expectedSet3);
    REQUIRE(testPKB.GetWhileControlVarsByStatement(StatementNumber(1)) == expectedSet4);
    REQUIRE(testPKB.GetWhileControlVarsByStatement(StatementNumber(10)) == expectedSet5);
    REQUIRE(testPKB.InsertWhileStatement(StatementNumber(20), VarName("z")));
    REQUIRE(testPKB.GetAllWhileStatementsWithControlVar() == expectedSet);
}

TEST_CASE("Table and PKB: if statement") {
    PKB testPKB = PKB();
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(1), VarName("x")));
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(1), VarName()));
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(5), VarName("y")));
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(10), VarName("y")));
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(10), VarName("x")));
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(20), VarName()));
    set<StatementNumber> expectedSet{ StatementNumber(1), StatementNumber(5), StatementNumber(10), StatementNumber(20) };
    set<StatementNumber> expectedSet1{ StatementNumber(1), StatementNumber(5), StatementNumber(10) };
    set<StatementNumber> expectedSet2{ StatementNumber(1), StatementNumber(10) };
    set<StatementNumber> expectedSet3{ StatementNumber(5), StatementNumber(10) };
    set<VarName> expectedSet4{ VarName("x") };
    set<VarName> expectedSet5{ VarName("x"), VarName("y") };
    REQUIRE(testPKB.GetAllIfStatements() == expectedSet);
    REQUIRE(testPKB.GetAllIfStatementsWithControlVar() == expectedSet1);
    REQUIRE(testPKB.GetIfStatementsByVar(VarName("x")) == expectedSet2);
    REQUIRE(testPKB.GetIfStatementsByVar(VarName("y")) == expectedSet3);
    REQUIRE(testPKB.GetIfControlVarsByStatement(StatementNumber(1)) == expectedSet4);
    REQUIRE(testPKB.GetIfControlVarsByStatement(StatementNumber(10)) == expectedSet5);
    REQUIRE(testPKB.InsertIfStatement(StatementNumber(20), VarName("z")));
    REQUIRE(testPKB.GetAllIfStatementsWithControlVar() == expectedSet);
}

TEST_CASE("Table and PKB: Modify Table") {
    PKB* testpkb = new PKB();
    set<VarName> set1;
    set<StatementNumber> set2;
    set1.insert(VarName("x"));
    set2.insert(StatementNumber(2));
    REQUIRE(testpkb->InsertModifies(StatementNumber(2), VarName("x")));
    REQUIRE(testpkb->GetModifiesByStatement(StatementNumber(2)) == set1);
    REQUIRE(testpkb->GetModifiesByVar(VarName("x")) == set2);
    REQUIRE(testpkb->InsertModifies(StatementNumber(2), VarName("y")));
    set1.insert(VarName("y"));
    REQUIRE(testpkb->GetModifiesByStatement(StatementNumber(2)) == set1);
}

TEST_CASE("PKB: procedure table") {
    PKB* testpkb = new PKB();
    testpkb->InsertProcedureName("main");
    set<string> expSet{ "main" };
    REQUIRE(testpkb->GetAllProcedureNames() == expSet);
}

TEST_CASE("Table and PKB: constant table") {
    PKB* testpkb = new PKB();
    REQUIRE(testpkb->InsertConstant(StatementNumber(2), "10"));
    REQUIRE(testpkb->InsertConstant(StatementNumber(2), "11"));
    REQUIRE(testpkb->InsertConstant(StatementNumber(1), "1"));
    REQUIRE(testpkb->InsertConstant(StatementNumber(3), "3"));
    REQUIRE(testpkb->InsertConstant(StatementNumber(3), "10"));
    REQUIRE(testpkb->InsertConstant(StatementNumber(3), "0"));
    set<Constant> expect{ "0", "1", "3", "10", "11" };
    set<StatementNumber> expect1{ StatementNumber(2), StatementNumber(3) };
    set<Constant> expect2{ "10", "11" };
    REQUIRE(testpkb->GetAllConstants() == expect);
    REQUIRE(testpkb->GetStatementsByConstant("10") == expect1);
    REQUIRE(testpkb->GetConstantsByStatement(StatementNumber(2)) == expect2);
}

TEST_CASE("PKB: assign table") {
    PKB testpkb;
    SECTION("simple test") {
        DesignExtractor de = DesignExtractor();
        string rpn1 = de.Rpn("x+y+z");
        string rpn2 = de.Rpn("x+(y+z)");
        testpkb.InsertAssign(StatementNumber(1), rpn1);
        testpkb.InsertAssign(StatementNumber(2), rpn2);
        vector<StatementNumber> exp1{ StatementNumber(1) };
        vector<StatementNumber> exp2{ StatementNumber(2) };
        REQUIRE(testpkb.GetStatementsBySameExpr("(x+y)+z") == exp1);
        REQUIRE(testpkb.GetStatementsBySubExpr("x+y") == exp1);
        REQUIRE(testpkb.GetStatementsBySubExpr("y+z") == exp2);
        vector<StatementNumber> exp3{ StatementNumber(1) , StatementNumber(2) };
        REQUIRE(testpkb.GetAllAssignStatements() == exp3);
    }
    SECTION("exapmle from wiki") {
        DesignExtractor de = DesignExtractor();
        string rpn1 = de.Rpn("v + x * y + z * t");
        testpkb.InsertAssign(StatementNumber(1), rpn1);
        testpkb.InsertAssign(StatementNumber(2), rpn1);
        vector<StatementNumber> exp{ StatementNumber(1), StatementNumber(2) };
        vector<StatementNumber> empty;
        REQUIRE(testpkb.GetStatementsBySameExpr("v+x*y+z*t") == exp);
        REQUIRE(testpkb.GetStatementsBySameExpr("v") == empty);
        REQUIRE(testpkb.GetStatementsBySubExpr("v") == exp);
        REQUIRE(testpkb.GetStatementsBySubExpr("x*y") == exp);
        REQUIRE(testpkb.GetStatementsBySubExpr("v+x") == empty);
        REQUIRE(testpkb.GetStatementsBySubExpr("v+x*y") == exp);
        REQUIRE(testpkb.GetStatementsBySubExpr("y+z*t") == empty);
        REQUIRE(testpkb.GetStatementsBySubExpr("x*y+z*t") == empty);
        REQUIRE(testpkb.GetStatementsBySubExpr("v+x*y+z*t") == exp);
        REQUIRE(testpkb.GetAssignByStatement(StatementNumber(1)) == rpn1);
        vector<pair<StatementNumber, string>> all{ {StatementNumber(1),rpn1}, 
            {StatementNumber(2), rpn1} };
        REQUIRE(testpkb.GetAllAssigns() == all);
    }
}

TEST_CASE("PKB: variable table") {
    PKB testpkb;
    REQUIRE(testpkb.InsertVariable(VarName("x")));
    REQUIRE(testpkb.InsertVariable(VarName("xxxxxxx")));
    REQUIRE(testpkb.InsertVariable(VarName("xx")));
    REQUIRE(testpkb.InsertVariable(VarName("variable")));
    REQUIRE(!testpkb.InsertVariable(VarName("x")));
    set<VarName> exp{ VarName("x"), VarName("xxxxxxx"), VarName("xx"), VarName("variable") };
    REQUIRE(testpkb.GetAllVarNames() == exp);
}

TEST_CASE("PKB: modifyP_table and useP_table") {
    PKB testpkb;
    testpkb.InsertProcedureName("main");
    testpkb.InsertProcedureName("first");
    testpkb.InsertProcedureName("second");
    testpkb.InsertProcedureName("third");
    testpkb.InsertVariable(VarName("x"));
    testpkb.InsertVariable(VarName("y"));
    testpkb.InsertVariable(VarName("z"));
    SECTION("Test ModifiesP") {
        REQUIRE(testpkb.InsertModifiesP("main", VarName("x")));
        REQUIRE(testpkb.InsertModifiesP("main", VarName("y")));
        REQUIRE(testpkb.InsertModifiesP("main", VarName("z")));
        REQUIRE(!testpkb.InsertModifiesP("main", VarName("y")));
        REQUIRE(testpkb.InsertModifiesP("first", VarName("x")));
        REQUIRE(testpkb.InsertModifiesP("first", VarName("y")));
        REQUIRE(testpkb.InsertModifiesP("second", VarName("x")));
        REQUIRE(testpkb.InsertModifiesP("second", VarName("z")));

        set<ProcedureName> exp1{ "main", "first", "second" };
        set<ProcedureName> exp2{ "main", "first" };
        set<ProcedureName> exp3{ "main", "second" };
        set<ProcedureName> empty;
        REQUIRE(testpkb.GetModifiesPByVar(VarName("x")) == exp1);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("y")) == exp2);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("z")) == exp3);
        REQUIRE(testpkb.GetModifiesPByVar(VarName(" ")) == empty);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("a")) == empty);

        set<VarName> exp4{ VarName("x"), VarName("y"), VarName("z") };
        set<VarName> exp5{ VarName("x"), VarName("y") };
        set<VarName> exp6{ VarName("x"), VarName("z") };
        set<VarName> emptyV;
        REQUIRE(testpkb.GetModifiesPByProcedure("main") == exp4);
        REQUIRE(testpkb.GetModifiesPByProcedure("first") == exp5);
        REQUIRE(testpkb.GetModifiesPByProcedure("second") == exp6);
        REQUIRE(testpkb.GetModifiesPByProcedure("") == emptyV);
        REQUIRE(testpkb.GetModifiesPByProcedure("dummy") == emptyV);
        REQUIRE(testpkb.GetModifiesPByProcedure("third") == emptyV);

        set<pair<ProcedureName, VarName>> all{ {"main", VarName("x")}, {"main", VarName("y")}, {"main", VarName("z")},
            {"first", VarName("x")}, {"first", VarName("y")}, {"second", VarName("x")}, {"second", VarName("z")} };
        vector<pair<ProcedureName, VarName>> temp = testpkb.GetAllModifiesP();
        set<pair<ProcedureName, VarName>> actual = set(temp.begin(), temp.end());
        REQUIRE(actual == all);
    }

    SECTION("Test UsesP") {
        REQUIRE(testpkb.InsertUsesP("main", VarName("x")));
        REQUIRE(testpkb.InsertUsesP("main", VarName("y")));
        REQUIRE(testpkb.InsertUsesP("main", VarName("z")));
        REQUIRE(!testpkb.InsertUsesP("main", VarName("y")));
        REQUIRE(testpkb.InsertUsesP("first", VarName("x")));
        REQUIRE(testpkb.InsertUsesP("first", VarName("y")));
        REQUIRE(testpkb.InsertUsesP("second", VarName("x")));
        REQUIRE(testpkb.InsertUsesP("second", VarName("z")));

        set<ProcedureName> exp1{ "main", "first", "second" };
        set<ProcedureName> exp2{ "main", "first" };
        set<ProcedureName> exp3{ "main", "second" };
        set<ProcedureName> empty;
        REQUIRE(testpkb.GetUsesPByVar(VarName("x")) == exp1);
        REQUIRE(testpkb.GetUsesPByVar(VarName("y")) == exp2);
        REQUIRE(testpkb.GetUsesPByVar(VarName("z")) == exp3);
        REQUIRE(testpkb.GetUsesPByVar(VarName(" ")) == empty);
        REQUIRE(testpkb.GetUsesPByVar(VarName("a")) == empty);

        set<VarName> exp4{ VarName("x"), VarName("y"), VarName("z") };
        set<VarName> exp5{ VarName("x"), VarName("y") };
        set<VarName> exp6{ VarName("x"), VarName("z") };
        set<VarName> emptyV;
        REQUIRE(testpkb.GetUsesPByProcedure("main") == exp4);
        REQUIRE(testpkb.GetUsesPByProcedure("first") == exp5);
        REQUIRE(testpkb.GetUsesPByProcedure("second") == exp6);
        REQUIRE(testpkb.GetUsesPByProcedure("") == emptyV);
        REQUIRE(testpkb.GetUsesPByProcedure("dummy") == emptyV);

        set<pair<ProcedureName, VarName>> all{ {"main", VarName("x")}, {"main", VarName("y")}, {"main", VarName("z")},
            {"first", VarName("x")}, {"first", VarName("y")}, {"second", VarName("x")}, {"second", VarName("z")} };
        vector<pair<ProcedureName, VarName>> temp = testpkb.GetAllUsesP();
        set<pair<ProcedureName, VarName>> actual = set(temp.begin(), temp.end());
        REQUIRE(actual == all);
    }
}

TEST_CASE("PKB: Calls and Calls*") {
    PKB testpkb;
    testpkb.InsertProcedureName("main");
    testpkb.InsertProcedureName("first");
    testpkb.InsertProcedureName("second");
    testpkb.InsertProcedureName("third");
    REQUIRE(testpkb.InsertCalls("main", "first"));
    REQUIRE(testpkb.InsertCalls("main", "second"));
    REQUIRE(testpkb.InsertCalls("first", "second"));
    REQUIRE(testpkb.InsertCalls("second", "third"));
    set<ProcedureName> exp1{ "first", "second" };
    set<ProcedureName> exp2{ "main", "first" };
    REQUIRE(testpkb.GetCalls("second", "third"));
    REQUIRE(!testpkb.GetCalls("first", "third"));
    REQUIRE(testpkb.GetProcedureNamesCalls("main") == exp1);
    REQUIRE(testpkb.GetProcedureNamesCalledBy("second") == exp2);
    set<pair<ProcedureName, ProcedureName>> all_calls{ {"main", "first"}, { "main", "second" }, 
        { "first", "second" }, { "second", "third" }};
    vector<pair<ProcedureName, ProcedureName>>temp = testpkb.GetAllCalls();
    set<pair<ProcedureName, ProcedureName>> actual = set(temp.begin(), temp.end());
    REQUIRE(actual == all_calls);
    testpkb.InitCallsStarTable();
    set<ProcedureName> exp3{ "first", "second", "third"};
    REQUIRE(testpkb.GetProcedureNamesCallsStar("main") == exp3);
    set<ProcedureName> exp4{ "first", "second", "main" };
    REQUIRE(testpkb.GetProcedureNamesCalledStarBy("third") == exp4);
    REQUIRE(testpkb.GetCallsStar("first", "third"));
    set<pair<ProcedureName, ProcedureName>> all_calls_star{ {"main", "first"}, { "main", "second" }, { "main", "third" },
        { "first", "second" }, { "first", "third" } , { "second", "third" } };
    vector<pair<ProcedureName, ProcedureName>>temp2 = testpkb.GetAllCallsStar();
    set<pair<ProcedureName, ProcedureName>> actual2 = set(temp2.begin(), temp2.end());
    REQUIRE(actual2 == all_calls_star);
}

TEST_CASE("PKB: Cyclic Calls") {
    PKB testpkb;
    testpkb.InsertProcedureName("main");
    testpkb.InsertProcedureName("first");
    testpkb.InsertProcedureName("second");
    testpkb.InsertProcedureName("third");
    REQUIRE(testpkb.InsertCalls("main", "first"));
    REQUIRE(testpkb.InsertCalls("main", "second"));
    REQUIRE(testpkb.InsertCalls("first", "second"));
    REQUIRE(testpkb.InsertCalls("second", "third"));
    SECTION("Cyclic Calls 1") {
        REQUIRE(testpkb.InsertCalls("third", "main"));
        REQUIRE(!testpkb.InitCallsStarTable());
    }
    SECTION("Cyclic Calls 2") {
        REQUIRE(testpkb.InsertCalls("test1", "test2"));
        REQUIRE(testpkb.InsertCalls("test2", "test3"));
        REQUIRE(testpkb.InsertCalls("test2", "test4"));
        REQUIRE(testpkb.InsertCalls("third", "main"));
        REQUIRE(!testpkb.InitCallsStarTable());
    }
}

TEST_CASE("PKB: Calls and ModifiesP, UsesP") {
    PKB testpkb;
    testpkb.InsertProcedureName("main");
    testpkb.InsertProcedureName("first");
    testpkb.InsertProcedureName("second");
    testpkb.InsertProcedureName("third");
    testpkb.InsertProcedureName("forth");
    REQUIRE(testpkb.InsertCalls("main", "first"));
    REQUIRE(testpkb.InsertCalls("main", "second"));
    REQUIRE(testpkb.InsertCalls("first", "second"));
    REQUIRE(testpkb.InsertCalls("second", "third"));
    REQUIRE(testpkb.InsertCalls("main", "forth"));
    testpkb.InsertVariable(VarName("x"));
    testpkb.InsertVariable(VarName("y"));
    testpkb.InsertVariable(VarName("z"));
    testpkb.InsertVariable(VarName("a"));
    SECTION("Calls and ModifiesP") {
        REQUIRE(testpkb.InsertModifiesP("main", VarName("x")));
        REQUIRE(testpkb.InsertModifiesP("first", VarName("x")));
        REQUIRE(testpkb.InsertModifiesP("first", VarName("y")));
        REQUIRE(testpkb.InsertModifiesP("second", VarName("z")));
        REQUIRE(testpkb.InsertModifiesP("forth", VarName("a")));
        testpkb.UpdateNestedModifiesP();
        // Expected:
        // main modifies: x, y, z, a
        // first modifies: x, y, z
        // second modifies: z
        // third modfies: (none)
        // forth modfies: a

        set<pair<ProcedureName, VarName>> all{ {"main", VarName("x")}, {"main", VarName("y")}, {"main", VarName("z")},
            {"main", VarName("a")}, {"first", VarName("x")}, {"first", VarName("y")}, {"first", VarName("z")},
            {"second", VarName("z")}, {"forth", VarName("a")} };
        vector<pair<ProcedureName, VarName>> temp = testpkb.GetAllModifiesP();
        set<pair<ProcedureName, VarName>> actual = set(temp.begin(), temp.end());
        REQUIRE(actual == all);

        set<ProcedureName> exp1{ "main", "first", "second" };
        set<ProcedureName> exp2{ "main", "first" };
        set<ProcedureName> exp3{ "main", "forth" };
        set<ProcedureName> empty;
        REQUIRE(testpkb.GetModifiesPByVar(VarName("x")) == exp2);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("y")) == exp2);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("z")) == exp1);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("a")) == exp3);
        REQUIRE(testpkb.GetModifiesPByVar(VarName("b")) == empty);
        REQUIRE(testpkb.GetModifiesPByVar(VarName(" ")) == empty);

        set<VarName> exp4{ VarName("x"), VarName("y"), VarName("z"), VarName("a") };
        set<VarName> exp5{ VarName("x"), VarName("y"), VarName("z") };
        set<VarName> exp6{ VarName("z") };
        set<VarName> exp7{ VarName("a") };
        set<VarName> emptyV;
        REQUIRE(testpkb.GetModifiesPByProcedure("main") == exp4);
        REQUIRE(testpkb.GetModifiesPByProcedure("first") == exp5);
        REQUIRE(testpkb.GetModifiesPByProcedure("second") == exp6);
        REQUIRE(testpkb.GetModifiesPByProcedure("forth") == exp7);
        REQUIRE(testpkb.GetModifiesPByProcedure("") == emptyV);
        REQUIRE(testpkb.GetModifiesPByProcedure("dummy") == emptyV);
        REQUIRE(testpkb.GetModifiesPByProcedure("third") == emptyV);
    }
    SECTION("Calls and UsesP") {
        REQUIRE(testpkb.InsertUsesP("main", VarName("x")));
        REQUIRE(testpkb.InsertUsesP("first", VarName("x")));
        REQUIRE(testpkb.InsertUsesP("first", VarName("y")));
        REQUIRE(testpkb.InsertUsesP("second", VarName("z")));
        REQUIRE(testpkb.InsertUsesP("forth", VarName("a")));
        testpkb.UpdateNestedUsesP();
        // Expected:
        // main uses: x, y, z, a
        // first uses: x, y, z
        // second uses: z
        // third uses: (none)
        // forth uses: a

        set<pair<ProcedureName, VarName>> all{ {"main", VarName("x")}, {"main", VarName("y")}, {"main", VarName("z")},
            {"main", VarName("a")}, {"first", VarName("x")}, {"first", VarName("y")}, {"first", VarName("z")},
            {"second", VarName("z")}, {"forth", VarName("a")} };
        vector<pair<ProcedureName, VarName>> temp = testpkb.GetAllUsesP();
        set<pair<ProcedureName, VarName>> actual = set(temp.begin(), temp.end());
        REQUIRE(actual == all);

        set<ProcedureName> exp1{ "main", "first", "second" };
        set<ProcedureName> exp2{ "main", "first" };
        set<ProcedureName> exp3{ "main", "forth" };
        set<ProcedureName> empty;
        REQUIRE(testpkb.GetUsesPByVar(VarName("x")) == exp2);
        REQUIRE(testpkb.GetUsesPByVar(VarName("y")) == exp2);
        REQUIRE(testpkb.GetUsesPByVar(VarName("z")) == exp1);
        REQUIRE(testpkb.GetUsesPByVar(VarName("a")) == exp3);
        REQUIRE(testpkb.GetUsesPByVar(VarName("b")) == empty);
        REQUIRE(testpkb.GetUsesPByVar(VarName(" ")) == empty);

        set<VarName> exp4{ VarName("x"), VarName("y"), VarName("z"), VarName("a") };
        set<VarName> exp5{ VarName("x"), VarName("y"), VarName("z") };
        set<VarName> exp6{ VarName("z") };
        set<VarName> exp7{ VarName("a") };
        set<VarName> emptyV;
        REQUIRE(testpkb.GetUsesPByProcedure("main") == exp4);
        REQUIRE(testpkb.GetUsesPByProcedure("first") == exp5);
        REQUIRE(testpkb.GetUsesPByProcedure("second") == exp6);
        REQUIRE(testpkb.GetUsesPByProcedure("forth") == exp7);
        REQUIRE(testpkb.GetUsesPByProcedure("") == emptyV);
        REQUIRE(testpkb.GetUsesPByProcedure("dummy") == emptyV);
        REQUIRE(testpkb.GetUsesPByProcedure("third") == emptyV);
    }
}

TEST_CASE("PKB: Next simple") {
    PKB testpkb;
    SECTION("Only one level") {
        /*
        * procedure main {
        *   read x;
        *   read y;
        *   x = x+1;
        *   y = y+1;
        *   print x;
        *   print y;}
        * procedure first {
        *   print x;
        *   print y;
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 6);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(2));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertFollows(StatementNumber(3), StatementNumber(4));
        testpkb.InsertFollows(StatementNumber(4), StatementNumber(5));
        testpkb.InsertFollows(StatementNumber(5), StatementNumber(6));
        testpkb.InsertProcedureName("first");
        testpkb.SetProcedure("first", 7, 8);
        testpkb.InsertFollows(StatementNumber(7), StatementNumber(8));
        testpkb.InitFollowsStarTable();
        testpkb.SetMaxStmtNumber(8);
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
        set<pair<StatementNumber, StatementNumber>> exp{ {StatementNumber(1), StatementNumber(2)},
            {StatementNumber(2), StatementNumber(3)},
            {StatementNumber(3), StatementNumber(4)},
            {StatementNumber(4), StatementNumber(5)},
            {StatementNumber(5), StatementNumber(6)},
            {StatementNumber(7), StatementNumber(8)}
        };
        set<pair<StatementNumber, StatementNumber>> actual(v.begin(), v.end());
        REQUIRE(actual == exp);
        
        SECTION("Test next*") {
            testpkb.InitParentStarTable();
            set<StatementNumber> exp2{ StatementNumber(2), StatementNumber(3), StatementNumber(4), StatementNumber(5),
            StatementNumber(6) };
            REQUIRE(testpkb.GetStatementsNextStarAfter(StatementNumber(1)) == exp2);
            set<StatementNumber> exp3{ StatementNumber(3), StatementNumber(4), StatementNumber(5),
            StatementNumber(6) };
            REQUIRE(testpkb.GetStatementsNextStarAfter(StatementNumber(2)) == exp3);
            set<StatementNumber> exp4{ StatementNumber(1), StatementNumber(2), StatementNumber(3), StatementNumber(4), StatementNumber(5),
            StatementNumber(7) };
            REQUIRE(testpkb.GetAllStatementsNextStarAfter() == exp4);
            testpkb.ClearCache();
            REQUIRE(testpkb.GetAllStatementsNextStarAfter() == exp4);
        }
    }

    SECTION("single level while") {
        /*
        * procedure main {
        * 1  x = x+1;
        * 2  while (1) {
        * 3     x=x+2;
        * 4     y=x;}
        * 5  b=1;
        * 6  while (1) {
        * 7     print y;}
        * }
        * procedure first {
        * 8 while (1) {
        * 9     c++;}
        * }
        * procedure second{
        * 10 while(1) {
        * 11    c++;
        * 12    b++;
        * 13    a++;}
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 7);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(2));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(5));
        testpkb.InsertWhileStatement(StatementNumber(2), VarName());
        testpkb.InsertParent(StatementNumber(2), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(2), StatementNumber(4));
        testpkb.InsertFollows(StatementNumber(3), StatementNumber(4));
        testpkb.InsertFollows(StatementNumber(5), StatementNumber(6));
        testpkb.InsertWhileStatement(StatementNumber(6), VarName());
        testpkb.InsertParent(StatementNumber(6), StatementNumber(7));
        testpkb.InsertProcedureName("first");
        testpkb.SetProcedure("first", 8, 9);
        testpkb.InsertParent(StatementNumber(8), StatementNumber(9));
        testpkb.InsertWhileStatement(StatementNumber(8), VarName());
        testpkb.InsertProcedureName("second");
        testpkb.SetProcedure("second", 10, 13);
        testpkb.InsertWhileStatement(StatementNumber(10), VarName());
        testpkb.InsertParent(StatementNumber(10), StatementNumber(11));
        testpkb.InsertParent(StatementNumber(10), StatementNumber(12));
        testpkb.InsertParent(StatementNumber(10), StatementNumber(13));
        testpkb.InsertFollows(StatementNumber(11), StatementNumber(12));
        testpkb.InsertFollows(StatementNumber(12), StatementNumber(13));
        testpkb.SetMaxStmtNumber(13);
        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
        set<pair<StatementNumber, StatementNumber>> exp{ {StatementNumber(1), StatementNumber(2)},
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
        /*
        * procedure main {
        * 1 x = 1;
        * 2 y = 1;
        * 3 if (1) then {
        * 4     x=1;
        * 5     y=1;
        * 6     z=1;
        * 7     p=1;}
        * else {
        * 8     y=1;}
        * 9 x=1;
        * 10 y=1;
        * }
        * procedure first {
        * 11 if (1) then {
        * 12    x=1;}
        * else {
        * 13    y=1;}
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 10);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(2));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertFollows(StatementNumber(3), StatementNumber(9));
        testpkb.InsertIfStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(6));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(8));
        testpkb.InsertFollows(StatementNumber(4), StatementNumber(5));
        testpkb.InsertFollows(StatementNumber(5), StatementNumber(6));
        testpkb.InsertFollows(StatementNumber(6), StatementNumber(7));
        testpkb.InsertFollows(StatementNumber(9), StatementNumber(10));

        testpkb.InsertProcedureName("first");
        testpkb.SetProcedure("first", 11, 13);
        testpkb.InsertIfStatement(StatementNumber(11), VarName());
        testpkb.InsertParent(StatementNumber(11), StatementNumber(12));
        testpkb.InsertParent(StatementNumber(11), StatementNumber(13));
        testpkb.SetMaxStmtNumber(13);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();

        auto v = testpkb.GetAllNext();
        //testpkb.PrintAllRelations(v);
        set<pair<StatementNumber, StatementNumber>> exp{ {StatementNumber(1), StatementNumber(2)},
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
        /*
        * procedure main {
        * 1 if (1) then {
        * 2     x=1;
        * 3     y=1;}
        * else {
        * 4     z=1;}
        * 5 while (0) {
        * 6     x=1;}
        * 7 if (0) then {
        * 8     x=1;}
        * else {
        * 9     y=1;}
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 9);
        testpkb.InsertIfStatement(StatementNumber(1), VarName());
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(5));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(4));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertWhileStatement(StatementNumber(5), VarName());
        testpkb.InsertFollows(StatementNumber(5), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(5), StatementNumber(6));
        testpkb.InsertIfStatement(StatementNumber(7), VarName());
        testpkb.InsertParent(StatementNumber(7), StatementNumber(8));
        testpkb.InsertParent(StatementNumber(7), StatementNumber(9));
        testpkb.SetMaxStmtNumber(9);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();

        auto v = testpkb.GetAllNext();
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

TEST_CASE("PKB: Next combined") {
    PKB testpkb;
    SECTION("double level nested while") {
        /*
        * procedure main {
        * 1 while(1) {
        * 2     x=1;
        * 3     while (0) {
        * 4         y=1;
        * 5         z=1;}
        * 6     a=1;}
        * 7 y=1;
        * }
        * procedure first {
        * 8 while(1) {
        * 9     while(0) {
        * 10        x=1;}}
        * }
        * procedure second {
        * 11 while(1) {
        * 12    while(0) {
        * 13        z=1;}
        * 14    x=1;}
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 7);
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertWhileStatement(StatementNumber(1), VarName());
        testpkb.InsertParent(StatementNumber(1), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(6));
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(7));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertFollows(StatementNumber(3), StatementNumber(6));
        testpkb.InsertWhileStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.InsertFollows(StatementNumber(4), StatementNumber(5));
        testpkb.InsertProcedureName("first");
        testpkb.SetProcedure("first", 8, 10);
        testpkb.InsertParent(StatementNumber(8), StatementNumber(9));
        testpkb.InsertWhileStatement(StatementNumber(8), VarName());
        testpkb.InsertParent(StatementNumber(9), StatementNumber(10));
        testpkb.InsertWhileStatement(StatementNumber(9), VarName());
        testpkb.InsertProcedureName("second");
        testpkb.SetProcedure("second", 11, 14);
        testpkb.InsertWhileStatement(StatementNumber(11), VarName());
        testpkb.InsertParent(StatementNumber(11), StatementNumber(12));
        testpkb.InsertParent(StatementNumber(11), StatementNumber(14));
        testpkb.InsertFollows(StatementNumber(12), StatementNumber(14));
        testpkb.InsertWhileStatement(StatementNumber(12), VarName());
        testpkb.InsertParent(StatementNumber(12), StatementNumber(13));
        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        testpkb.SetMaxStmtNumber(14);
        auto v = testpkb.GetAllNext();

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
        /*
        * procedure main {
        * 1 if (0) then {
        * 2     x=1;
        * 3     if (1) then {
        * 4         y=1;}
        *       else {
        * 5         x=1;}
        * else {
        * 6     y=1;}
        * 7 z=1;
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 7);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(7));
        testpkb.InsertIfStatement(StatementNumber(1), VarName());
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(6));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertIfStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.SetMaxStmtNumber(7);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
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
        /*
        * procedure main {
        * 1 if (0) then {
        * 2     x=1;
        * 3     if (1) then {
        * 4         y=1;}
        *       else {
        * 5         x=1;}
        * else {
        * 6     if (1) then {
        * 7         x=1;}
        *       else {
        * 8         a=1;
        * 9         y=1;}}
        * 10 z=1;
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 10);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(10));
        testpkb.InsertIfStatement(StatementNumber(1), VarName());
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(6));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertIfStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.InsertIfStatement(StatementNumber(6), VarName());
        testpkb.InsertParent(StatementNumber(6), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(8));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(9));
        testpkb.InsertFollows(StatementNumber(8), StatementNumber(9));
        testpkb.SetMaxStmtNumber(10);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
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
        /*
        * procedure main {
        * 1 if (0) then {
        * 2     x=1;
        * 3     if (1) then {
        * 4         y=1;}
        *       else {
        * 5         x=1;}
        * else {
        * 6     if (1) then {
        * 7         x=1;}
        *       else {
        * 8         a=1;
        * 9         y=1;}}
        * 10    x=1;
        * 11 z=1;
        * }
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 11);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(11));
        testpkb.InsertIfStatement(StatementNumber(1), VarName());
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(3));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(6));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(10));
        testpkb.InsertFollows(StatementNumber(2), StatementNumber(3));
        testpkb.InsertIfStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.InsertIfStatement(StatementNumber(6), VarName());
        testpkb.InsertFollows(StatementNumber(6), StatementNumber(10));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(8));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(9));
        testpkb.InsertFollows(StatementNumber(8), StatementNumber(9));
        testpkb.SetMaxStmtNumber(11);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
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

    SECTION("double level nested while and if 1") {
        /*
        * procedure main {
        * 1 while(1) {
        * 2     if (1) then {
        * 3         if (0) then {
        * 4             x=1;}
        *           else {
        * 5             y=1;}
        *       else {
        * 6         if (0) then {
        * 7             x=1;}
        *           else {
        * 8             y=1;}}}
        * 9 x=1;
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 9);
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(9));
        testpkb.InsertWhileStatement(StatementNumber(1), VarName());
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));

        testpkb.InsertIfStatement(StatementNumber(2), VarName());
        testpkb.InsertParent(StatementNumber(2), StatementNumber(6));
        testpkb.InsertParent(StatementNumber(2), StatementNumber(3));
        testpkb.InsertIfStatement(StatementNumber(3), VarName());
        testpkb.InsertParent(StatementNumber(3), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(3), StatementNumber(5));
        testpkb.InsertIfStatement(StatementNumber(6), VarName());
        testpkb.InsertParent(StatementNumber(6), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(6), StatementNumber(8));
        testpkb.SetMaxStmtNumber(9);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
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

    SECTION("double level nested while and if 2") {
        /*
        * procedure main {
        * 1 if (1) then {
        * 2     while (2) {
        * 3         x=1;}
        * else {
        * 4     while(1) {
        * 5         x=1;}
        * 6     y=1;}
        * 7 x=1;
        */
        testpkb.InsertProcedureName("main");
        testpkb.SetProcedure("main", 1, 7);
        testpkb.InsertIfStatement(StatementNumber(1), VarName());
        testpkb.InsertFollows(StatementNumber(1), StatementNumber(7));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(2));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(4));
        testpkb.InsertParent(StatementNumber(1), StatementNumber(6));

        testpkb.InsertWhileStatement(StatementNumber(2), VarName());
        testpkb.InsertParent(StatementNumber(2), StatementNumber(3));
        testpkb.InsertWhileStatement(StatementNumber(4), VarName());
        testpkb.InsertParent(StatementNumber(4), StatementNumber(5));
        testpkb.InsertFollows(StatementNumber(4), StatementNumber(6));
        testpkb.SetMaxStmtNumber(7);

        testpkb.InitFollowsStarTable();
        testpkb.InitNextTable();
        auto v = testpkb.GetAllNext();
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
}