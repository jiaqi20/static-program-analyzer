#include <string>
#include <vector>

#include "catch.hpp"
#include "../src/QueryProcessing/QueryEvaluator.h"
#include "../src/QueryProcessing/QueryEvaluator.cpp"
#include "../src/QueryProcessing/QueryProcessor.h"
#include "../src/QueryProcessing/QueryProcessor.cpp"

using namespace std;

TEST_CASE("Test Order Build") {

    SECTION( "Contains Call only" ) {
        string test_string = "procedure p; call c; assign a; while w; Select p such that Calls (\"Second\", p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        PKB pkb = PKB();
        QueryEvaluator qe = QueryEvaluator(&pkb);
        string expectedResult = "Current result lists are: \n"
                                " block0: r0  \n"
                                " ";
        REQUIRE(expectedResult == qe.PrintBuildSequence(&result));
    }

    SECTION( "Contains Call* only" ) {
        string test_string = "procedure p; call c; assign a; while w; Select p such that Calls* (\"Second\", p)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        PKB pkb = PKB();
        QueryEvaluator qe = QueryEvaluator(&pkb);
        string expectedResult = "Current result lists are: \n"
                                " block0: r0  \n"
                                " ";
        REQUIRE(expectedResult == qe.PrintBuildSequence(&result));
    }

    SECTION( "Contains Call and With" ) {
        string test_string = "procedure p; call c; assign a; while w; Select p such that Calls (\"Second\", p) and Parent (w, c) with c.procName = p.procName";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        PKB pkb = PKB();
        QueryEvaluator qe = QueryEvaluator(&pkb);
        string expectedResult = "Current result lists are: \n"
                                " block0: r0 r1 w0  \n"
                                " ";
        REQUIRE(expectedResult == qe.PrintBuildSequence(&result));
    }

    SECTION( "Contains Call and Parent" ) {
        string test_string = "variable v, v1; print pn; call c, c1; assign a, a1, a2; while w, w1; procedure p; if ifs, ifs1; Select <a, p> such that Calls (\"Second\", p) and Parent (w1, c) and Parent (w, c) and Calls (\"Second\", p) ";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        PKB pkb = PKB();
        QueryEvaluator qe = QueryEvaluator(&pkb);
        string expectedResult = "Current result lists are: \n"
                                " block0: r1 r2  \n"
                                " block1: r0 r3  \n"
                                " ";
        REQUIRE(expectedResult == qe.PrintBuildSequence(&result));
    }

    SECTION( "Contains Multiple Patterns" ) {
        string test_string = "variable v, v1; print pn; assign a, a1, a2; procedure p; if ifs, ifs1; Select <a, p> pattern a(v, \"c\") and a1(v, _\"x\"_) and a2(v1, _) and ifs(v1, _, _) and ifs1(_, _, _)";
        QueryLexer ql1 = QueryLexer(test_string);
        QueryParser qp1 = QueryParser(&ql1);
        Query result = qp1.query();
        PKB pkb = PKB();
        QueryEvaluator qe = QueryEvaluator(&pkb);
        string expectedResult = "Current result lists are: \n"
                                " block0: p0 p1  \n"
                                " block1: p2 p3  \n"
                                " block2: p4  \n"
                                " ";
        REQUIRE(expectedResult == qe.PrintBuildSequence(&result));
    }
}

TEST_CASE("General Test for Order build") {

    string test_string1 = "procedure p; call c; assign a; while w; Select p such that Calls (\"Second\", p) and Parent (w, c) with c.procName = p.procName";
    string test_string2 = "procedure p; call c; assign a; while w; Select p such that Calls (\"Second\", p)";
    string test_string3 = "variable v; print pn; assign a; Select BOOLEAN such that Affects(a, _)";
    string test_string4 = "variable v, v1; print pn; assign a, a1, a2; procedure p; if ifs, ifs1; Select <a, p> pattern a(v, \"c\") and a1(v, _\"x\"_) and a2(v1, _) and ifs(v1, _, _) and ifs1(_, _, _)";
    string test_string5 = "variable v, v1; print pn; call c, c1; assign a, a1, a2; while w, w1; procedure p; if ifs, ifs1; Select <a, p> such that Calls (\"Second\", p) and Parent (w1, c) and Parent (w, c) and Calls (\"Second\", p) ";
    string test_string6 = "variable v, v1; print pn; call c, c1; assign a, a1, a2; while w, w1; procedure p; if ifs, ifs1; Select <a, p> such that Calls (\"Second\", p) and Parent (w1, c) and Parent (w, c) and Calls (\"Second\", p) pattern a(v, \"c\")";
    string test_string7 = "variable v, v1; print pn; call c, c1; assign a, a1, a2; while w, w1; procedure p; if ifs, ifs1; "
                          "Select <a, p.procName> such that Calls (\"Second\", p) and Parent (w1, c) "
                          "and Parent (w, c) and Calls (\"Second\", p) pattern a(v, \"c\")";
    string test_string8 = "variable v, v1; print pn; call c3, c1; assign a3, a1, a2; while w, w1; if ifs, ifs1; procedure p,a,b,c,d,e,f; "
                          "Select <a, p.procName> such that Calls (\"Second\", p) and Calls (\"Second\", p) and Calls (\"Second\", a) and Calls (b, c) and Calls (c, d) and Calls (d, e) and Calls (e, f) ";
    string test_string9 = "variable v, v1; print pn; call c, c1; assign a, a1, a2; while w, w1; stmt s; procedure p; if ifs, ifs1; Select BOOLEAN such that Uses(s, _) and Next(_, _)";
    string test_string12 = "stmt s; assign a; print pr; read re; if ifs; while w; constant cons; call c; variable v; procedure p, q;\n"
"Select s such that Follows(pr, re) and Next(126, a) and Parent(s, re) and Parent(s, a) and Uses(ifs, v) and Modifies(w, v) with p.procName = \"procedure\" and v.varName = \"b\" and c.procName = q.procName and re.stmt# = 126 pattern a(v, \"b + 6\") and ifs(v, _, _) and w(v, _)";
    string test_string11 = "variable v, v1; print pn; assign a, a1, a2; procedure p; if ifs, ifs1; Select <a, p> pattern a(v, \"c\") and a1(v, _\"x\"_) and a2(v1, _) and ifs(v1, _, _) and ifs1(_, _, _)";
    string test_string13 = "variable v, v1; print pn; assign a, a1, a2; procedure p; if ifs, ifs1; Select v such that Parent*(5, 20)";

    QueryLexer ql1 = QueryLexer(test_string13);
    QueryParser qp1 = QueryParser(&ql1);
    Query result = qp1.query();
    PKB pkb = PKB();
    QueryEvaluator qe = QueryEvaluator(&pkb);
    REQUIRE(1 == 1);
}