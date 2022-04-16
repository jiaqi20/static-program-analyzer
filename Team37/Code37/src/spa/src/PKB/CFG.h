#ifndef INC_21S2_CP_SPA_TEAM_37_CFG_H
#define INC_21S2_CP_SPA_TEAM_37_CFG_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "PKB/Attribute/StatementNumber.h"
#include "Attribute/ADT.h"
#include "Attribute/VarName.h"
#include "Attribute/Procedure.h"
#include "Attribute/LMT.h"
#include "Storage/Table.h"
#include "Storage/PKBStorage.h"

using namespace std;

class CFG {

private:

    PKBStorage* storage;

    const set<StatementNumber>* while_stmts = nullptr;

    set<StatementNumber>* outermost_while_stmts = nullptr;

    // Caches next star relations Next*(stmt, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>* next_star_table;

    // Caches affects relations Affects(stmt, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>* affects_table;

    unordered_map<ProcedureIndex, vector<vector<bool>>> affects_adjacent_matrices;

    unordered_set<ProcedureIndex> affects_record;

    // Caches affects star relations Affects*(stmt, stmt)
    Table<StatementNumber, StatementNumber, hashStatementNumber, hashStatementNumber>* affects_star_table;

    unordered_set<ProcedureIndex> affects_star_record;

    void GenerateNextStar(Procedure* proc);

    void GenerateOutermostWhiles();

    void UpdateNextStarOne(Procedure* proc);

    void UpdateNextStarAll();

    bool IsAssign(const StatementNumber& sn);

    bool IsWhile(const StatementNumber& sn);

    bool IsIf(const StatementNumber& sn);

    void GenerateAffects(Procedure* proc);

    void GenerateAffects(const StatementNumber& sn, LMT&);

    void GenerateAffectsForWhile(const StatementNumber& sn, LMT& before_lmt);

    void GenerateAffectsForIf(const StatementNumber& sn, LMT& before_lmt);

    void GenerateAffectsForAssign(const StatementNumber& cur, LMT& lmt);

    void GenerateAffectsForOthers(const StatementNumber& cur, LMT& lmt);

    void UpdateAffectsOne(Procedure* proc);

    void UpdateAffectsAll();

    void GenerateAffectsStar(Procedure* proc);

    void UpdateAffectsStarOne(Procedure* proc);

    void UpdateAffectsStarAll();

    void BuildAffectsAdjMatrix(Procedure* proc);


public:
    CFG(PKBStorage* storage);
    
    ~CFG();

    // APIs for Next*

    bool GetNextStar(const StatementNumber& s1, const StatementNumber& s2, Procedure* proc);
    const set<StatementNumber>* GetNextStarAfter(const StatementNumber& s, Procedure* proc);
    const set<StatementNumber>* GetNextStarBefore(const StatementNumber& s, Procedure* proc);
    const set<StatementNumber>* GetAllNextStarAfter();
    const set<StatementNumber>* GetAllNextStarBefore();
    vector<pair<StatementNumber, StatementNumber>>* GetAllNextStar();

    // API for affects

    // Gets if Affects(sn1, sn2) is true
    bool GetAffects(const StatementNumber& sn1, const StatementNumber& sn2, Procedure* proc);
    // Gets if Affects(sn, _) is true
    bool IsAffectsFirstExist(const StatementNumber& sn, Procedure* proc);
    // Gets if Affects(_, sn) is true
    bool IsAffectsSecondExist(const StatementNumber& sn, Procedure* proc);
    // Gets if Affects is empty
    bool IsAffectsEmpty();
    // Gets stmt such that Affects(sn, stmt) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt is affected by sn directly
    const set<StatementNumber>* GetStatementsAffectedBy(const StatementNumber& sn, Procedure* proc);
    // Gets stmt such that Affects(stmt, sn) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt affects sn
    const set<StatementNumber>* GetStatementsAffects(const StatementNumber& sn, Procedure* proc);
    // Gets all statements sn such that Affects(sn, _) holds
    const set<StatementNumber>* GetAllStatementsAffects();
    // Gets all statements sn such that Affects(_, sn) holds
    const set<StatementNumber>* GetAllStatementsAffected();
    // Gets all Affects(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>>* GetAllAffects();

    // API for affects star

    // Gets if Affects*(sn1, sn2) is true
    bool GetAffectsStar(const StatementNumber& sn1, const StatementNumber& sn2, Procedure* proc);
    // Gets stmt such that Affects*(sn, stmt) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt is affected by sn directly or indirectly
    const set<StatementNumber>* GetStatementsAffectedStarBy(const StatementNumber& sn, Procedure* proc);
    // Gets stmt such that Affects*(stmt, sn) holds, sn is input, sn and stmt are StatementNumber, i.e. stmt affects sn direcly or indirectly
    const set<StatementNumber>* GetStatementsAffectsStar(const StatementNumber& sn, Procedure* proc);
    // Gets all statements sn such that Affects*(sn, _) holds
    const set<StatementNumber>* GetAllStatementsAffectsStar();
    // Gets all statements sn such that Affects*(_, sn) holds
    const set<StatementNumber>* GetAllStatementsAffectedStar();
    // Gets all Affects*(stmt#, stmt#) in the form of a vector of pair<stmt#, stmt#>
    vector<pair<StatementNumber, StatementNumber>>* GetAllAffectsStar();

    // Clear caches in CFG
    void ClearCache();

};

#endif // INC_21S2_CP_SPA_TEAM_37_CFG_H