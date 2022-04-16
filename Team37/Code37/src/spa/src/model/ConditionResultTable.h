#ifndef SPA_CONDITIONRESULTTABLE_H
#define SPA_CONDITIONRESULTTABLE_H
#include <list>
#include "QType.h"
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <set>
#include <map>

class ConditionResultTable {

private:
    list<list<Ref>> table;
    void AddRowDirectToTable(list<list<Ref>> &input_table, list<Ref> &list_row); // no check
    bool ValidInputRow(list<list<Ref>> &list_row);
    list<Ref> ExtractRefFromInputRow(Ref &col, list<list<Ref>> &list_row);
    void RepeatRow(int k);
    bool RepeatCol(list<Ref> &col, int k);
    bool ContainsRef(Ref &col, list<list<Ref>> &list_row);
    bool AddCommonAttributesTable(list<list<Ref>> &input_table);
    bool PermutateCol(list<Ref> &col, int &number_of_repeat_in_new_col);
    bool HasCommonAttribute(list<list<Ref>>& data);
    bool VerifyInputTable(list<list<Ref>> &input_table);
    void ExtractCommonAttributesPosition(list<list<Ref>> &input_table, unordered_map<int, int> &map, list<int> &input_column_index_removed);
    void JoinTwoTable(list<list<Ref>> &input_table, unordered_map<int, list<int>> &map);
    bool MatchRow(list<Ref> &table_row, list<Ref> &input_row, unordered_map<int, int> &map);
    void ReverseSortList(list<int> &input_column_index_removed);
    void DeleteListOfRowOrColFromTable(list<list< Ref>> &origin_table, list<int> &row_removed, bool is_row);
    void InitializeTableWithHeader(list<list< Ref>> &origin_table, list< Ref> header);
    bool VerifyTwoRowTheSame(list<Ref> &pre_row, list<Ref> &current_row);
    bool VerifyRefInSet(list<Ref> &col, Ref &ref);
    bool VerifyRowIsUnique(list<Ref> &row, list<list<Ref>> &input_table, int current_index, list<int> &to_removed);
public:
    ConditionResultTable();
    ~ConditionResultTable();
    bool combine(list<list<Ref>>& data);
    size_t recordSize();
    size_t columnSize();
    bool hasColumn(Ref *checking);
    list<Ref> ReadCol (Ref &col);
    list<list<Ref>>& GetTable();
    void RemoveDuplicatesInTable(list<list<Ref>> &input_table);
    string PrintCustomTable(list<list<Ref>> &input_table);
    bool AddCol(list<Ref> &col); // n^2 cross product
    void AddColDirectToTable(list<list<Ref>> &input_table, list<Ref> &col);
    list<Ref> ReadRow(int row, list<list< Ref>> &input_table);
    bool DeleteRow(int row, list<list<Ref>> &input_table);
    bool DeleteColByIndex(int index, list<list<Ref>> &input_table);
    bool DeleteColByRef(Ref &col, list<list<Ref>> &input_table);
    void CreateDataSet(Ref &ref, string s);
    list<list<Ref>> SelectDistinctRef(list<Ref> &col, list<list<Ref>> &input_table);
    bool AddRow(list<list<Ref>> &list_row); // check then add 2 * n^2
    bool AddCrossProductTable(list<list<Ref>> &input_table);
    Ref ExtractElement(int row, Ref &col);
};
#endif //SPA_CONDITIONRESULTTABLE_H
