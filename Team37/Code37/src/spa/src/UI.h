#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include "./SourceProcessor/SourceParser.h"
#include "./QueryProcessing/QueryProcessor.h"
#include "./PKB/PKB.h"

using namespace std;

class UI {
private:
    SourceParser* sp;
    PKB* p;
    QueryProcessor* qp;

public:
    UI();
    ~UI();
    void parseSource(std::string& filename);
    void evaluateQuery(std::string query, std::list<std::string>& results);
    void free();
};