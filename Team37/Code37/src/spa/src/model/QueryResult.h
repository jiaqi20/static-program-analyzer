
#ifndef INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULT_H
#define INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULT_H

#include "string"
#include "vector"
#include <list>

using namespace std;

class QueryResult {
    vector<string> resultsList;
public:
    QueryResult();
    QueryResult(vector<int> results);
    QueryResult(vector<string> results);
    void add(int newResult);
    void add(string newResult);
    int GetResultListSize();
    string GetResultAt(int i);
    list<string> ToList();
};


#endif //INC_21S2_CP_SPA_TEAM_37_1_QUERYRESULT_H
