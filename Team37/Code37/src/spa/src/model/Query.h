//
// This is the data type for storing PQL query order
//

#ifndef INC_21S2_CP_SPA_TEAM_37_1_QUERY_H
#define INC_21S2_CP_SPA_TEAM_37_1_QUERY_H

#include "iostream"
#include "string"
#include "QType.h"
#include "ENUM.h"

using namespace std;

class Query {
    Ref selected_ent;
    vector<Relation> list_of_such_that_cl;
    vector<Pattern> list_of_pattern_cl;
    vector<With> list_of_with_cl;
    bool valid_query;
    bool syntactically_valid_query = true;
    bool semantically_valid_query = true;
    string s;
    bool has_list_of_such_that = false;
    bool has_list_of_pattern = false;
    bool has_list_of_with = false;

public:
    explicit Query(Ref *ref,  bool semantically_valid, string& message);
    Query(Ref *ref, bool syntactically_valid, bool semantically_valid, string& message);
    Query(Ref *ref, Relation *relation,  bool semantically_valid, string& message);
    Query(Ref *ref, Pattern *pattern,  bool semantically_valid, string& message);
    Query(Ref *ref, Relation *relation, Pattern *pattern,  bool semantically_valid, string& message);
    Query(Ref *ref, vector<Relation> *relation,  bool semantically_valid, string& message);
    Query(Ref *ref, vector<Pattern> *pattern,  bool semantically_valid, string& message);
    Query(Ref *ref, vector<Relation> *relation, vector<Pattern> *pattern, bool semantically_valid, string& message);
    Query(Ref *ref, vector<Relation> *relation, vector<Pattern> *pattern, vector<With> *with, bool semantically_valid, string& message);
    Query(Ref *ref, vector<Pattern> *pattern, vector<With> *with, bool semantically_valid, string& message);
    Query(Ref *ref, vector<Relation> *relation, vector<With> *with, bool semantically_valid, string& message);
    Query(Ref *ref, vector<With> *with, bool semantically_valid, string& message);
    explicit Query(bool valid);
    Query(bool valid, string &message);
    bool IsValid();
    bool IsSemanticallyValid();
    bool IsSyntacticallyValid();
    Ref* GetSelectedEnt();
    vector<Relation>* GetRelationList();
    vector<Pattern>* GetPatternList();
    vector<With>* GetWithList();
    bool conditionContainsSynonym();
    string ToString();
    string GetErrorMessage();
};


#endif //INC_21S2_CP_SPA_TEAM_37_1_QUERY_H
