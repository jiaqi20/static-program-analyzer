#include "QueryResult.h"
#include <list>

QueryResult::QueryResult() {

}
QueryResult::QueryResult(vector<int> results) {
    for (int i = 0; i < results.size(); ++i) {
        resultsList.push_back(to_string(results.at(i)));
    }
}

QueryResult::QueryResult(vector<string> results) {
    resultsList.assign(results.begin(), results.end());
}

void QueryResult::add(int newResult) {
    resultsList.push_back(to_string(newResult));
}

void QueryResult::add(string newResult) {
    resultsList.push_back(newResult);
}

int QueryResult::GetResultListSize() {
    return resultsList.size();
}

string QueryResult::GetResultAt(int i) {
    return resultsList[i];
}

list<string> QueryResult::ToList() {
    string result = "";
    list<string> finalResultList;
    if (resultsList.size() == 0) {
        return finalResultList;
    }
    for (int i = 0; i < resultsList.size(); ++i) {
        finalResultList.push_back(resultsList[i]);
        if (i == resultsList.size() - 1) {
            result += resultsList[i];
        } else {
            result += resultsList[i] + " ";
        }
    }
    return finalResultList;
}