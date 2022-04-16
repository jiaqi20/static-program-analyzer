#include <string>
#include <fstream>
#include <list>
#include <iostream>

#include "../src/SourceProcessor/DesignExtractor.h"
#include "../src/SourceProcessor/DesignExtractor.cpp"
#include "catch.hpp"

using namespace std;

TEST_CASE("Expression Tree") {

    // variable with single letter
    // check variable (first position)
    SECTION( "Test 1" ) {
        string a = "x + y + z";
        string b = "x";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // variable with single letter
    // check variable (second position)
    SECTION( "Test 2" ) {
        std::string a = "x + y + z";
        std::string b = "y";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // variable with single letter and constants
    // check constant (second position)
    SECTION( "Test 3" ) {
        std::string a = "x + 1 + 2";
        std::string b = "1";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // variable with single letter
    // check variable not in tree
    SECTION( "Test 4" ) {
        std::string a = "x + 1 + 2";
        std::string b = "y";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // variable with single letter
    // check constant (second position)
    SECTION( "Test 5" ) {
        std::string a = "x + 10 + z";
        std::string b = "10";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr only contains digits
    // check constant
    SECTION( "Test 6" ) {
        std::string a = "100 + 10 + (200) + 300 ";
        std::string b = "200";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with multiply operation
    // check variable not in tree
    SECTION( "Test 7" ) {
        std::string a = "x * x * y";
        std::string b = "z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with multiply operation
    // check variable in tree
    SECTION( "Test 8" ) {
        std::string a = "x * x * y";
        std::string b = "y";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        int sAlen = sA.length() + 1;
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operations
    // check variable in tree
    SECTION( "Test 9" ) {
        std::string a = "x + y * z + c";
        std::string b = "z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operation with brackets (precedence)
    // check variable in tree
    SECTION( "Test 10" ) {
        std::string a = "x + (y * z) + c";
        std::string b = "z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operation with brackets (precedence) and spaces
    // check variable not in tree
    SECTION( "Test 11" ) {
        std::string a = "x + (                y * z) + c";
        std::string b = "a";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operation with brackets (precedence) and spaces
    // check variable in tree
    SECTION( "Test 12" ) {
        std::string a = "x + (                y * z) + c";
        std::string b = "c";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operation with brackets (precedence)
    // check variable in tree (inside the bracket)
    SECTION( "Test 13" ) {
        std::string a = "x + (                y * z) + c";
        std::string b = "z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // check two exactly equal trees
    SECTION( "Test 14" ) {
        std::string a = "x + y + z";
        std::string b = "x + y + z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }
    
    // variables made up with multiple letters
    // check variable in tree
    SECTION( "Test 15" ) {
        std::string a = "abc + dce";
        std::string b = "dce";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed variables with mixed letters and constants larger than 9 (multiple digits)
    // check constant in tree
    SECTION( "Test 16" ) {
        std::string a = "abc + 100 + 2000 + 30";
        std::string b = "100";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed variables with mixed letters and constants larger than 9 (multiple digits)
    // check variable in tree
    SECTION( "Test 17" ) {
        std::string a = "300 + 100 + dfg + 30";
        std::string b = "dfg";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // variables made up with multiple letters and digits
    // check variable in tree
    SECTION("Test 18") {
        std::string a = "200 + 89 + dfg123";
        std::string b = "dfg123";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // variables made up with multiple letters and digits
    // check constant in tree
    SECTION("Test 19") {
        std::string a = "200 + 89 + abc";
        std::string b = "89";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with division
    // variables made up with multiple letters and digits
    // check variable in tree
    SECTION("Test 20") {
        std::string a = "200 / abc";
        std::string b = "abc";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with subtraction
    // variables made up with multiple letters and digits
    // check trees that are exactly same
    SECTION("Test 20") {
        std::string a = "20 - abc";
        std::string b = "20 - abc";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters and digits
    // check variable not in the tree
    SECTION("Test 21") {
        std::string a = "20 + (10 * abc)";
        std::string b = "ac";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with letters
    // check subpattern not in the tree
    SECTION("Test 22") {
        std::string a = "x + (y * z) + c";
        std::string b = "x + y";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with letters
    // check variable in tree
    SECTION("Test 23") {
        std::string a = "x + (y * z) + c";
        std::string b = "z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters and digits
    // check variable not in the tree
    SECTION("Test 24") {
        std::string a = "20 + (10 * aBc)";
        std::string b = "2";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with subtraction
    // variables made up with multiple letters and digits
    // check inequal trees
    SECTION("Test 25") {
        std::string a = "20 - abc";
        std::string b = "20 - ab";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    // check variable in the tree
    SECTION("Test 24") {
        std::string a = "20 + (10 * aBc)";
        std::string b = "aBc";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    // check variable not in the tree
    SECTION("Test 24") {
        std::string a = "20 + (10 * aBc)";
        std::string b = "abc";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    SECTION("Test 25") {
        std::string a = "20 + 10 * (aBc + z) * t1";
        std::string b = "(aBc + z) * t1";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    SECTION("Test 26") {
        std::string a = "20 + 10 * (aBc + z) * t1";
        std::string b = "(aBc + z)";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    SECTION("Test 27") {
        std::string a = "20 + 10 * (aBc + z) * t1";
        std::string b = "10 * (aBc + z) * t1";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    SECTION("Test 28") {
        std::string a = "20 + 10 * (aBc % z) * t1";
        std::string b = "(aBc % z)";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    //invalid subtree
    SECTION("Test 29") {
        std::string a = "20 + 10 * (aBc % z) * t1";
        std::string b = "% z";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }

    // expr with mixed operators
    // variables made up with multiple letters (caps and non caps)
    //invalid subtree
    SECTION("Test 30") {
        std::string a = "20 + 10 * (aBc % z) * t1";
        std::string b = "20 + 10";
        DesignExtractor dex = DesignExtractor();
        DesignExtractor dexA = DesignExtractor();
        DesignExtractor dexB = DesignExtractor();
        string sA = dexA.Rpn(a);
        string sB = dexB.Rpn(b);
        REQUIRE(!dex.KmpSearch(sB, sA));
    }
}

