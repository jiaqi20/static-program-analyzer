//
// This is the data type for storing PQL query order
//

#include "Query.h"

Query::Query(Ref *ref, bool semantically_valid, string& message) {
    selected_ent = *ref;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(bool valid, string& message) {
    syntactically_valid_query = valid;
    s = message;
}

Query::Query(Ref *ref, bool syntactically_valid, bool semantically_valid, string &message) {
    selected_ent = *ref;
    syntactically_valid_query = syntactically_valid;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, Relation *relation, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_such_that_cl.push_back(*relation);
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, Pattern *pattern, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_pattern_cl.push_back(*pattern);
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, Relation *relation, Pattern *pattern, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_such_that_cl.push_back(*relation);
    list_of_pattern_cl.push_back(*pattern);
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Relation> *relation, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_such_that_cl = *relation;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Pattern> *pattern, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_pattern_cl = *pattern;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Relation> *relation, vector<Pattern> *pattern, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_such_that_cl = *relation;
    list_of_pattern_cl = *pattern;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<With> *with, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_with_cl = *with;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Pattern> *pattern, vector<With> *with, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_with_cl = *with;
    list_of_pattern_cl = *pattern;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Relation> *relation, vector<With> *with, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_with_cl = *with;
    list_of_such_that_cl = *relation;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(Ref *ref, vector<Relation> *relation, vector<Pattern> *pattern, vector<With> *with, bool semantically_valid, string& message) {
    selected_ent = *ref;
    list_of_with_cl = *with;
    list_of_such_that_cl = *relation;
    list_of_pattern_cl = *pattern;
    semantically_valid_query = semantically_valid;
    s = message;
}

Query::Query(bool valid) {
    valid_query = valid;
}

bool Query::IsValid() {
    return semantically_valid_query;
}

Ref* Query::GetSelectedEnt() {
    return &selected_ent;
}

vector<Relation>* Query::GetRelationList() {
    return &list_of_such_that_cl;
}

vector<Pattern>* Query::GetPatternList() {
    return &list_of_pattern_cl;
}

vector<With>* Query::GetWithList() {
    return &list_of_with_cl;
}

string Query::ToString() {
    if (IsSyntacticallyValid() && IsSemanticallyValid()) {
        string resultString = "Select " + selected_ent.SelectToString() + " ";
        if (!list_of_such_that_cl.empty()) {
            resultString += "such that ";
            for (auto & iter : list_of_such_that_cl) {
                resultString += iter.ToString() + " ";
            }
        }
        if (!list_of_pattern_cl.empty()) {
            for (auto& iter : list_of_pattern_cl) {
                resultString += iter.ToString() + " ";
            }
        }
        if (!list_of_with_cl.empty()) {
            for (auto& iter : list_of_with_cl) {
                resultString += iter.ToString() + " ";
            }
        }
        return resultString;
    } else {
        return GetErrorMessage();
    }
}

string Query::GetErrorMessage() {
    return this->s;
}

bool Query::conditionContainsSynonym() {
    for (auto & iter : list_of_such_that_cl) {
        if (iter.GetArgument1()->getName()->GetType() == kSynonym || iter.GetArgument2()->getName()->GetType() == kSynonym) {
            return true;
        }
    }
    for (auto & iter : list_of_pattern_cl) {
        if (iter.GetArg1()->getName()->GetType() == kSynonym || iter.GetArg2()->getName()->GetType() == kSynonym) {
            return true;
        }
    }
    for (auto & iter : list_of_with_cl) {
        if (iter.GetArgument1()->getName()->GetType() == kSynonym || iter.GetArgument2()->getName()->GetType() == kSynonym) {
            return true;
        }
    }
    return false;
}

bool Query::IsSemanticallyValid() {
    return semantically_valid_query;
}

bool Query::IsSyntacticallyValid() {
    return syntactically_valid_query;
}
