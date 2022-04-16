
#include "QueryProcessor.h"

QueryProcessor::QueryProcessor() {

}

QueryProcessor::QueryProcessor(PKB *pkbIn)
{
    this->pkb = pkbIn;
}

list<string> QueryProcessor::ProcessQuery(string queryString){

        QueryLexer lexer = QueryLexer(queryString);
        
        QueryParser parser = QueryParser(&lexer);

        Query query = parser.query();

        if (query.IsSemanticallyValid() && query.IsSyntacticallyValid()) {

            QueryEvaluator queryResultDecider = QueryEvaluator(this->pkb);

            QueryResult queryResult = queryResultDecider.Evaluate(&query);

            pkb->ClearCache();

            return queryResult.ToList();
        } else {
            list<string> finalResultList;
            if (query.GetSelectedEnt()->IsBoolean()) {
                if (query.IsSyntacticallyValid() && !query.IsSemanticallyValid()) {
                    finalResultList.emplace_back("FALSE");
                }
            }
            return finalResultList;
        }
}
