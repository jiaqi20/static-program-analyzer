#include <iostream>
#include <string>
#include <list>
#include "UI.h"
#include "SourceProcessor/SourceParser.h"


using namespace std;

UI::UI() {
}

UI::~UI() {

}

void UI::free()
{

    delete p;
    delete sp;
    delete qp;
}

void UI::parseSource(std::string& filename) 
{
 
    try {

        std::ifstream file;
        file.open(filename);
        std::istream& fileStream = file;

        p = new PKB();
        sp = new SourceParser(p);
        qp = new QueryProcessor(p);

        sp->Parse(fileStream);

    }
    catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
        exit(0);
    }

}

void UI::evaluateQuery(std::string query, std::list<std::string>& results)
{
    try {
        // call query processor to parse query and return results
        results = qp->ProcessQuery(query);
    }
    catch (std::logic_error& e) {
        exit(0);
    }

}