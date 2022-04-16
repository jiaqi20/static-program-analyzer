

#ifndef INC_21S2_CP_SPA_TEAM_37_1_QUERYPROCESSOR_H
#define INC_21S2_CP_SPA_TEAM_37_1_QUERYPROCESSOR_H

#include "string"
#include "PKB/PKB.h"
#include "QueryParser.h"
#include "model/Query.h"
#include "QueryEvaluator.h"
#include "QueryLexer.h"
using namespace std;

class QueryProcessor {
PKB *pkb;
public:
    QueryProcessor();
    QueryProcessor(PKB *pkbIn);
    list<string> ProcessQuery(string queryString);
};


#endif //INC_21S2_CP_SPA_TEAM_37_1_QUERYPROCESSOR_H
