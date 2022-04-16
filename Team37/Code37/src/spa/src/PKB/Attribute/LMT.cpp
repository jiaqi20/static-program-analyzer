#include "LMT.h"
#include "LMT.h"

LMT::LMT()
{
}

LMT::~LMT()
{
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::iterator it = table.begin(); 
        it != table.end();) {
        delete it->second;
        it->second = NULL;
        table.erase(it++);
    }
}

LMT::LMT(const LMT& obj)
{
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator it = obj.table.begin();
        it != obj.table.end(); ++it) {
        this->table[it->first] = new unordered_set<StatementNumber, hashStatementNumber>(it->second->begin(), it->second->end());
    }
}

bool LMT::operator==(const LMT& obj) const
{
    if (this == &obj) {
        return true;
    }
    if (this->table.size() != obj.table.size()) {
        return false;
    }
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator it = obj.table.begin();
        it != obj.table.end(); ++it) {
        if (IsKeyExist(it->first)) {
            unordered_set<StatementNumber, hashStatementNumber>* temp = table.at(it->first);
            if (!(*(temp) == *(it->second))) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

bool LMT::Contains(const LMT& obj) const
{
    if (this == &obj) {
        return true;
    }
    if (this->table.size() != obj.table.size()) {
        return false;
    }
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator it = table.begin();
        it != table.end(); ++it) {
        if (!obj.IsKeyExist(it->first)) {
            return false;
        }
        unordered_set<StatementNumber, hashStatementNumber>* my_temp = table.at(it->first);
        unordered_set<StatementNumber, hashStatementNumber>* obj_temp = obj.table.at(it->first);
        for (typename unordered_set<StatementNumber, hashStatementNumber>::const_iterator it2 = obj_temp->begin();
            it2 != obj_temp->end(); ++it2) {
            if (my_temp->find(*it2) == my_temp->end()) {
                return false;
            }
        }
    }
    return true;
}

void LMT::Add(const VarIndex& vi, const StatementNumber& sn)
{
    if (table.find(vi) == table.end()) {
        table[vi] = new unordered_set<StatementNumber, hashStatementNumber>();
        table[vi]->insert(sn);
    }
    else {
        table[vi]->clear();
        table[vi]->insert(sn);
    }
}

bool LMT::IsKeyExist(const VarIndex& vi) const
{
    return this->table.find(vi) != this->table.end();
}

void LMT::MergeInto(const LMT& lmt)
{
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator
        it = lmt.table.begin(); it != lmt.table.end(); ++it) {
        if (IsKeyExist(it->first)) {
            table[it->first]->insert(it->second->begin(), it->second->end());
        }
        else {
            table[it->first] = new unordered_set<StatementNumber, hashStatementNumber>(it->second->begin(), it->second->end());
        }
    }
}

void LMT::CopyFrom(const LMT& lmt)
{
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::iterator it = table.begin();
        it != table.end();) {
        delete it->second;
        it->second = NULL;
        table.erase(it++);
    }

    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator it = lmt.table.begin();
        it != lmt.table.end(); ++it)
    {
        this->table[it->first] = new unordered_set<StatementNumber, hashStatementNumber>(it->second->begin(), it->second->end());
    }
}

unordered_set<StatementNumber, hashStatementNumber>* LMT::GetLastModifiesStatements(const VarIndex& vi)
{
    return table[vi];
}

void MergeTwoLMTs(LMT& lmt1, LMT& lmt2, LMT& result)
{
    result = lmt1;
    for (typename unordered_map<VarIndex, unordered_set<StatementNumber, hashStatementNumber>*, hash<VarIndex>>::const_iterator
        it = lmt2.table.begin(); it != lmt2.table.end(); ++it) {
        if (result.IsKeyExist(it->first)) {
            result.table[it->first]->insert(it->second->begin(), it->second->end());
        }
        else {
            result.table[it->first] = new unordered_set(*(it->second));
        }
    }
}