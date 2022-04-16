#include <string>
#include <set>

#include "../src/PKB/Storage/Table.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("PKB Table: test insertion") {
    Table<string, int> testTable;

    // insert new tuples, expect success
    REQUIRE(testTable.InsertRelation("relation1", 1));
    REQUIRE(testTable.InsertRelation("relation2", 2));
    REQUIRE(testTable.InsertRelation("relation3", 3));
    REQUIRE(testTable.InsertRelation("relation3", 10));
    REQUIRE(testTable.InsertRelation("relation10", 10));
    REQUIRE(testTable.InsertRelation("relation10", 100));
    REQUIRE(testTable.InsertRelation("relation100", 100));
    // Insert duplicate tuples, expect failure
    REQUIRE(!testTable.InsertRelation("relation1", 1));
    REQUIRE(!testTable.InsertRelation("relation10", 10));
    REQUIRE(!testTable.InsertRelation("relation3", 10));

}

TEST_CASE("PKB Table: test getter") {
    Table<string, int> testTable;
    testTable.InsertRelation("relation1", 1);
    testTable.InsertRelation("relation2", 2);
    testTable.InsertRelation("relation3", 3);
    testTable.InsertRelation("relation3", 10);
    testTable.InsertRelation("relation10", 10);
    testTable.InsertRelation("relation10", 100);
    testTable.InsertRelation("relation100", 100);

    SECTION("Test GetRelation") {
        REQUIRE(testTable.GetRelation("relation1", 1));
        REQUIRE(testTable.GetRelation("relation10", 100));
        REQUIRE(testTable.GetRelation("relation3", 10));
        REQUIRE(testTable.GetRelation("relation100", 100));
        REQUIRE(!testTable.GetRelation("relation2", 20));
        REQUIRE(!testTable.GetRelation("relation3", 100));
        REQUIRE(!testTable.GetRelation("relation0", 0));
    }

    SECTION("Test GetByAttribute1") {
        set<int> expectedSet1{ 1 };
        REQUIRE(*testTable.GetByAttribute1("relation1") == expectedSet1);
        set<int> expectedSet3{ 3, 10 };
        REQUIRE(*testTable.GetByAttribute1("relation3") == expectedSet3);
        set<int> expectedSet100{ 100 };
        REQUIRE(*testTable.GetByAttribute1("relation100") == expectedSet100);

        REQUIRE(testTable.GetByAttribute1("relationNoExist")->empty());
    }

    SECTION("Test GetByAttribute2") {
        set<string> expectedSet1{ "relation1" };
        REQUIRE(*testTable.GetByAttribute2(1) == expectedSet1);
        set<string> expectedSet10{ "relation3", "relation10" };
        REQUIRE(*testTable.GetByAttribute2(10) == expectedSet10);
        set<string> expectedSet3{ "relation3" };
        REQUIRE(*testTable.GetByAttribute2(3) == expectedSet3);

        REQUIRE(testTable.GetByAttribute2(0)->empty());
    }

    SECTION("Test GetAllAttribute1") {
        set<string> expectedSet{ "relation1", "relation2", "relation3", "relation10", "relation100" };
        REQUIRE(*testTable.GetAllAttribute1() == expectedSet);
    }

    SECTION("Test GetAllAttribute2") {
        set<int> expectedSet{ 1, 2, 3, 10, 100 };
        REQUIRE(*testTable.GetAllAttribute2() == expectedSet);
    }

    SECTION("Test getAllRelation") {
        set<pair<string, int>> expectedSet{ {"relation10", 10}, {"relation10", 100},{"relation100", 100},
            {"relation1", 1}, {"relation2", 2}, {"relation3", 3}, {"relation3", 10}};
        vector<pair<string, int>> actual = *testTable.GetAllRelations();
        set<pair<string, int>> actualSet = set(actual.begin(), actual.end());
        REQUIRE(expectedSet == actualSet);
    }

    SECTION("Test AddAllRelations") {
        Table<int, int> test2;
        set<int> s{ 1, 2, 3, 4, 5 };
        test2.InsertRelation(0, 0);
        test2.InsertRelation(10, 10);
        test2.AddAllRelations(0, s);
        set<int> expectedSet0{ 0, 1, 2, 3, 4, 5 };
        set<int> expectedSet10{ 10 };
        REQUIRE(*test2.GetByAttribute1(0) == expectedSet0);
        REQUIRE(*test2.GetByAttribute1(10) == expectedSet10);
        set<int> expectedSet1{ 0 };
        REQUIRE(*test2.GetByAttribute2(1) == expectedSet1);
        REQUIRE(*test2.GetByAttribute2(2) == expectedSet1);
        REQUIRE(*test2.GetByAttribute2(3) == expectedSet1);
        REQUIRE(*test2.GetByAttribute2(4) == expectedSet1);
        REQUIRE(*test2.GetByAttribute2(5) == expectedSet1);
        set<int> expectedSet2{ 10 };
        REQUIRE(*test2.GetByAttribute2(10) == expectedSet2);
    }

    SECTION("Test ComputeTransitiveClosureForDAG") {
        Table<int, int> test3;
        test3.InsertRelation(1, 2);
        test3.InsertRelation(2, 3);
        test3.InsertRelation(3, 4);
        test3.InsertRelation(5, 6);
        test3.InsertRelation(6, 7);
        test3.InsertRelation(7, 8);
        test3.InsertRelation(9, 10);
        Table<int, int> closure;
        ComputeTransitiveClosureForDAG(test3, closure);
        vector<pair<int, int>> v = *closure.GetAllRelations();
        set<pair<int, int>> expectedSet{ {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}, {5, 8},
            {5, 6}, {5, 7}, {6, 7}, {6, 8}, {7, 8}, {9, 10} };
        set<pair<int, int>> actualSet = set(v.begin(), v.end());

        REQUIRE(actualSet == expectedSet);
    }
}

TEST_CASE("PKB Table: test DFS 1") {
    Table<int, int> test;
    test.InsertRelation(1, 2);
    test.InsertRelation(2, 3);
    test.InsertRelation(2, 6);
    test.InsertRelation(3, 4);
    test.InsertRelation(3, 5);
    test.InsertRelation(6, 7);
    test.InsertRelation(6, 8);
    test.InsertRelation(4, 1);
    test.InsertRelation(5, 1);
    test.InsertRelation(7, 1);
    test.InsertRelation(8, 1);
    test.InsertRelation(1, 9);
    test.InsertRelation(9, 10);
    test.InsertRelation(10, 11);
    test.InsertRelation(11, 12);
    test.InsertRelation(12, 13);
    test.InsertRelation(13, 14);
    test.InsertRelation(14, 12);
    test.InsertRelation(12, 11);
    test.InsertRelation(11, 15);
    test.InsertRelation(15, 16);

    Table<int, int> testdfs;
    DFS(&test, &testdfs, 1);
    set<int> out1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    REQUIRE(*testdfs.GetByAttribute1(1) == out1);
    
}

TEST_CASE("PKB Table: test DFS 2") {
    Table<int, int> test;
    test.InsertRelation(1, 2);
    test.InsertRelation(1, 4);
    test.InsertRelation(2, 3);
    test.InsertRelation(3, 2);
    test.InsertRelation(4, 5);
    test.InsertRelation(5, 4);
    test.InsertRelation(5, 6);
    test.InsertRelation(6, 7);
    test.InsertRelation(2, 7);
    Table<int, int> testdfs;
    DFS(&test, &testdfs, 1);
    set<int> out1 = { 2, 3, 4, 5, 6, 7 };
    REQUIRE(*testdfs.GetByAttribute1(1) == out1);
    
}

TEST_CASE("PKB Table: test DFS 3") {
    Table<int, int> test;
    test.InsertRelation(0, 1);
    test.InsertRelation(0, 20);
    test.InsertRelation(1, 2);
    test.InsertRelation(1, 11);
    test.InsertRelation(2, 3);
    test.InsertRelation(2, 8);
    test.InsertRelation(3, 4);
    test.InsertRelation(3, 7);
    test.InsertRelation(4, 5);
    test.InsertRelation(5, 6);
    test.InsertRelation(6, 4);
    test.InsertRelation(4, 0);
    test.InsertRelation(7, 0);
    test.InsertRelation(8, 9);
    test.InsertRelation(8, 10);
    test.InsertRelation(9, 0);
    test.InsertRelation(10, 0);
    test.InsertRelation(11, 12);
    test.InsertRelation(11, 15);
    test.InsertRelation(12, 13);
    test.InsertRelation(12, 14);
    test.InsertRelation(13, 0);
    test.InsertRelation(14, 0);
    test.InsertRelation(15, 16);
    test.InsertRelation(16, 15);
    test.InsertRelation(15, 17);
    test.InsertRelation(17, 18);
    test.InsertRelation(17, 19);
    test.InsertRelation(18, 0);
    test.InsertRelation(19, 0);
    test.InsertRelation(0, 20);
    test.InsertRelation(20, 21);
    Table<int, int> testdfs;
    DFS(&test, &testdfs, 0);
    set<int> out1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21 };
    REQUIRE(*testdfs.GetByAttribute1(0) == out1);
    
}