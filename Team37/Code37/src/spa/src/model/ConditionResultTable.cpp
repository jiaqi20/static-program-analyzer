#include "ConditionResultTable.h"


using namespace std;

ConditionResultTable::ConditionResultTable() {

}

ConditionResultTable::~ConditionResultTable() {

}

bool ConditionResultTable::DeleteColByRef(Ref &col, list<list<Ref>> &input_table) {
    for (list<list<Ref>>::iterator it = input_table.begin(); it != input_table.end(); it++) {
        list<Ref>::iterator it1 = it->begin();
        Ref first_element = *it1;
        if (first_element == col) {
            input_table.erase(it);
            return true;
        }
    }
    return false;
}

bool ConditionResultTable::DeleteColByIndex(int index, list<list<Ref>> &input_table) {
    if (input_table.size() <= index) {
        return false;
    }

    list<list<Ref>>::iterator it = input_table.begin();
    advance(it, index);
    input_table.erase(it);
    return true;
}

bool ConditionResultTable::DeleteRow(int row, list<list<Ref>> &input_table) {
    for (list<list<Ref>>::iterator it = input_table.begin(); it != input_table.end(); it++) {
        int size_of_list = it->size();
        if (size_of_list <= row) {
            return false;
        }
        list<Ref>::iterator it1 = it->begin();
        advance(it1, row);
        it->erase(it1);
    }
    return true;
}

void ConditionResultTable::RepeatRow(int k) {
    if (k == 1) {
        return;
    }
    for (list<list<Ref>>::iterator it = table.begin(); it != table.end(); it++) {
        list<Ref>::iterator start = it->begin();
        start++;
        list<Ref>::iterator end = it->end();
        int number_of_row = it->size() - 1;
        for (int i = 1; i < k; i = i + 1) {
            for (int j = 0; j < number_of_row; j = j + 1) {
                it->push_back(*start);
                start++;
            }
            start = it->begin();
            start++;
        }
    }
}

bool ConditionResultTable::RepeatCol(list<Ref> &col, int k) {
    list<Ref> col_in;
    list<Ref>::iterator it = col.begin();
    col_in.push_back(*it);
    it++;
    for (it; it != col.end(); it++) {
        for (int j = 0; j < k; j = j + 1) {
            col_in.push_back(*it);
        }
    }
    if (col_in.size() <= 1) {
        return false;
    }
    table.push_back(col_in);
    return true;
}

void ConditionResultTable::AddColDirectToTable(list<list<Ref>> &input_table, list<Ref> &col) {
    input_table.push_back(col);
}

bool ConditionResultTable::AddCol(list<Ref> &col) {
    int number_of_repeat_in_row = col.size() - 1;
    int number_of_repeat_in_new_col;
    bool table_size = table.size();
    if (table_size == 0) {
        number_of_repeat_in_new_col = 0;
    } else {
        number_of_repeat_in_new_col = table.front().size() - 1;
    }
    if (table_size == 0) {
        AddColDirectToTable(table, col);
    } else if (number_of_repeat_in_row == 0 || number_of_repeat_in_new_col == 0) {
        return false;
    } else {
        RepeatRow(number_of_repeat_in_row);
        RepeatCol(col, number_of_repeat_in_new_col);
    }
}

bool ConditionResultTable::ContainsRef(Ref &col, list<list<Ref>> &list_row) {
    for (list<list<Ref>>::iterator it = list_row.begin(); it != list_row.end(); it++) {
        Ref current_ref = *it->begin();
        if (current_ref == col) {
            return true;
        }
    }
    return false;
}

bool ConditionResultTable::ValidInputRow(list<list<Ref>> &list_row) {
    if (list_row.size() != table.size()) {
        return false;
    } else {
        for (list<list<Ref>>::iterator it = table.begin(); it != table.end(); it++) {
            Ref current_ref = *it->begin();
            if (!ContainsRef(current_ref, list_row)) {
                return false;
            }
        }
        return true;
    }
}

list<Ref> ConditionResultTable::ExtractRefFromInputRow(Ref &col, list<list<Ref>> &list_row) {
    for (list<list<Ref>>::iterator it = list_row.begin(); it != list_row.end(); it++) {
        list<Ref>::iterator it1 = it->begin();
        if (*it1 == col) {
            list<Ref> result;
            it1++;
            for (it1; it1 != it->end(); it1++) {
                result.push_back(*it1);
            }
            return result;
        }
    }
    list<Ref> result1;
    return result1;
}

bool ConditionResultTable::AddRow(list<list<Ref>> &list_row) {
    if (ValidInputRow(list_row)) {
        for (list<list<Ref>>::iterator it = table.begin(); it != table.end(); it++) {
            list<Ref>::iterator it1 = it->begin();
            list<Ref> current_content = ExtractRefFromInputRow(*it1, list_row);
            if (current_content.size() == 0) {
                return false;
            }
            for (list<Ref>::iterator it2 = current_content.begin(); it2 != current_content.end(); it2++) {
                it->push_back(*it2);
            }
        }
        return true;
    } else {
        return false;
    }
}

list<Ref> ConditionResultTable::ReadRow(int row, list<list< Ref>> &input_table) {
    list<Ref> result;
    for (list<list<Ref>>::iterator it = input_table.begin(); it != input_table.end(); it++) {
        int size_of_list = it->size();
        if (size_of_list <= row) {
            list<Ref> result1;
            return result1;
        }
        list<Ref>::iterator it1 = it->begin();
        advance(it1, row);
        result.push_back(*it1);
    }
    return result;
}

list<Ref> ConditionResultTable::ReadCol(Ref &col) {
    for (auto & iter : table) {
        if (iter.front().equals(&col)) {
            return iter;
        }
    }
    list<Ref> result;
    return result;
}

Ref ConditionResultTable::ExtractElement(int row, Ref &col) {
    Ref result1;
    for (list<list<Ref>>::iterator it = table.begin(); it != table.end(); it++) {
        list<Ref>::iterator it1 = it->begin();
        Ref first_element = *it1;
        if (first_element == col) {
            int size_of_list = it->size();
            if (size_of_list <= row) {
                return result1;
            }
            list<Ref>::iterator it1 = it->begin();
            advance(it1, row);
            return *it1;
        }
    }
    return result1;
}

string ConditionResultTable::PrintCustomTable(list<list<Ref>> &input_table) {
    string result;
    for (auto & col : input_table) {
        result += "[";
        for (auto & ele: col) {
            result += ele.ToString() + ", ";
        }
        result += "]\n";
    }
    return result;
}

bool ConditionResultTable::HasCommonAttribute(list<list<Ref>> &data) {
    if (table.size() <= 0 ) {
        return false;
    }
    for (list<list<Ref>>::iterator it = data.begin(); it != data.end(); it++) {
        Ref dataRef = it->front();
        for (list<list<Ref>>::iterator it1 = table.begin(); it1 != table.end(); it1++) {
            Ref tableRef = it1->front();
            if (dataRef == tableRef) {
                return true;
            }
        }
    }
     return false;
}

bool ConditionResultTable::PermutateCol(list<Ref> &col, int &number_of_repeat_in_new_col) {
    int number_of_repeat_in_row = col.size() - 1;
    bool table_size = table.size();
    if (table_size == 0) {
        AddColDirectToTable(table, col);
    } else if (number_of_repeat_in_row == 0 || number_of_repeat_in_new_col == 0) {
        return false;
    } else {
        RepeatCol(col, number_of_repeat_in_new_col);
    }
    return true;
}

bool ConditionResultTable::VerifyInputTable(list<list<Ref>> &input_table) {
    int counter = 0;
    int size = -1;
    for (list<list<Ref>>::iterator it = input_table.begin(); it != input_table.end(); it++) {
        int current_size = it->size();
        if (counter == 0) {
            counter = counter + 1;
            size = current_size;
            continue;
        }

        if (size != current_size) {
            return false;
        }
    }

    return true;
}

bool ConditionResultTable::AddCrossProductTable(list<list<Ref>> &input_table) {
    if (VerifyInputTable(input_table)) {
        bool table_size = table.size();
        int number_of_repeat_in_new_col;
        if (table_size == 0) {
            number_of_repeat_in_new_col = 0;
        } else {
            number_of_repeat_in_new_col = table.front().size() - 1;
        }
        int number_of_repeat_in_row = input_table.front().size() - 1;
        RepeatRow(number_of_repeat_in_row);
        for (auto & it : input_table) {
            bool correct_output = PermutateCol(it, number_of_repeat_in_new_col);
            if (!correct_output) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

void ConditionResultTable::ExtractCommonAttributesPosition(list<list<Ref>> &input_table, unordered_map<int, int> &hashmap,
                                                           list<int> &input_column_to_be_remove) {
    int table_counter = 0;
    for (list<list<Ref>>::iterator it = table.begin(); it != table.end(); it++) {
        Ref dataRef = it->front();
        int input_counter = 0;
        for (list<list<Ref>>::iterator it1 = input_table.begin(); it1 != input_table.end(); it1++) {
            Ref tableRef = it1->front();
            if (dataRef == tableRef) {
                input_column_to_be_remove.push_back(input_counter);
                hashmap.insert(pair<int, int>(table_counter, input_counter));
                break;
            }
            input_counter = input_counter  + 1;
        }
        table_counter = table_counter + 1;
    }
}

bool ConditionResultTable::MatchRow(list<Ref> &table_row, list<Ref> &input_row, unordered_map<int, int> &map) {
    for (unordered_map<int, int>::iterator it = map.begin(); it != map.end(); it++) {
        int table_pos = it->first;
        int input_pos = it->second;
        list<Ref>::iterator table_it = table_row.begin();
        list<Ref>::iterator input_it = input_row.begin();
        advance(table_it, table_pos);
        advance(input_it, input_pos);
        if (table_it->ToString() == input_it->ToString()) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

void ConditionResultTable::ReverseSortList(list<int> &input_column_index_removed) {
    input_column_index_removed.sort();
    input_column_index_removed.reverse();
}

void ConditionResultTable::AddRowDirectToTable(list<list<Ref>> &input_table, list<Ref> &list_row) {
    int number_of_element = list_row.size();
    for (int i = 0; i < number_of_element; i = i + 1) {
        list<list<Ref>>::iterator table_index = input_table.begin();
        list<Ref>::iterator row_index = list_row.begin();
        advance(table_index, i);
        advance(row_index, i);
        table_index->push_back(*row_index);
    }
}

void ConditionResultTable::DeleteListOfRowOrColFromTable(list<list<Ref>> &origin_table, list<int> &row_or_column_removed, bool is_row) {
    for (list<int>::iterator it1 = row_or_column_removed.begin(); it1 != row_or_column_removed.end(); it1++) {
        if (is_row) {
            DeleteRow(*it1, origin_table);
        } else {
            DeleteColByIndex(*it1, origin_table);
        }
    }
}

void ConditionResultTable::InitializeTableWithHeader(list<list<Ref>> &origin_table, list<Ref> header) {
    for (list<Ref>::iterator it1 = header.begin(); it1 != header.end(); it1++) {
        Ref current_ref = *it1;
        list<Ref> first_row;
        first_row.push_back(current_ref);
        origin_table.push_back(first_row);
    }
}

void ConditionResultTable::JoinTwoTable(list<list<Ref>> &input_table, unordered_map<int, list<int>> &map) {
    list<list<Ref>> origin_table;
    list<list<Ref>> another_table;

    list<Ref> first_ref = ReadRow(0, table);
    list<Ref> first_ref_input = ReadRow(0, input_table);
    InitializeTableWithHeader(origin_table, first_ref);
    InitializeTableWithHeader(another_table, first_ref_input);
    for (unordered_map<int, list<int>>::iterator it1 = map.begin(); it1 != map.end(); it1++) {
        int origin_table_row = it1->first;
        list<int> current_list = it1->second;
        list<Ref> row_to_repeat = ReadRow(origin_table_row, table);
        for (list<int>::iterator it2 = current_list.begin(); it2 != current_list.end(); it2++) {
            list<Ref> input_row_to_repeat = ReadRow(*it2, input_table);
            AddRowDirectToTable(origin_table, row_to_repeat);
            AddRowDirectToTable(another_table, input_row_to_repeat);
        }
    }

    for (list<list<Ref>>::iterator it1 = another_table.begin(); it1 != another_table.end(); it1++) {
        AddColDirectToTable(origin_table, *it1);
    }
    table = origin_table;

}
bool ConditionResultTable::AddCommonAttributesTable(list<list<Ref>> &input_table) {
    unordered_map<int, int> hm;
    unordered_map<int, list<int>> hm1;
    list<int> input_column_to_be_remove;
    ExtractCommonAttributesPosition(input_table, hm, input_column_to_be_remove);
    ReverseSortList(input_column_to_be_remove);
    int number_of_row_table = table.front().size();
    int number_of_row_input = input_table.front().size();
    for (int i = 1; i < number_of_row_table; i = i + 1) {
        list<Ref> table_ref = ReadRow(i, table);
        list<int> current_list;
        for (int j = 1; j < number_of_row_input; j = j + 1) {
            list<Ref> input_ref = ReadRow(j, input_table);
            if (MatchRow(table_ref, input_ref, hm)) {
                current_list.push_back(j);
            }
        }
        if (current_list.size() != 0) {
            hm1[i] = current_list;
        }
    }
    DeleteListOfRowOrColFromTable(input_table, input_column_to_be_remove, false);
    JoinTwoTable(input_table, hm1);
    return true;
}

bool ConditionResultTable::combine(list<list<Ref>>& data) {
    bool table_initially_empty = false;
    
    if (data.size() == 0) {
        return false;
    }

    if (table.size() == 0) {
        table_initially_empty = true;
        table = data;
    } else {

        if (HasCommonAttribute(data)) {
            bool add_common_attribute_success = AddCommonAttributesTable(data);
            if (!add_common_attribute_success) {
                return false;
            }
        } else {
            bool add_cross_product_success = AddCrossProductTable(data);
            if (!add_cross_product_success) {
                return false;
            }
        }
    }
    if (table.size() == 0 || table.front().size() == 0) {
        return false;
    }
    return true;
}

list<list<Ref>>& ConditionResultTable::GetTable() {
    return this -> table;
}

size_t ConditionResultTable::recordSize() {
    if (table.empty()) {
        return 0;
    } else {
        return table.front().size() - 1;
    }
}

size_t ConditionResultTable::columnSize() {
    return table.size();
}

bool ConditionResultTable::hasColumn(Ref *checking) {
    for (auto & iter : table) {
        if (iter.begin()->equals(checking)) {
            return true;
        }
    }
    return false;
}

void ConditionResultTable::CreateDataSet(Ref &ref, string s) {
    EntType et1 = kStmt;
    ValueType type1 = kSynonym;
    string name1 = s;
    RefValue rf1 = RefValue(type1, name1);
    ref = Ref(et1, rf1);
}

bool ConditionResultTable::VerifyTwoRowTheSame(list<Ref> &pre_row, list<Ref> &current_row) {
    int number_of_row = pre_row.size();
   for (int i = 0; i < number_of_row; i = i + 1) {
       list<Ref>::iterator it_pre_row = pre_row.begin();
       list<Ref>::iterator it_current_row = current_row.begin();
        int pre_row_size = pre_row.size();
        int cur_row_size = current_row.size();
        if (pre_row_size <= i || cur_row_size <= i) {
            return false;
        }
        advance(it_pre_row, i);
        advance(it_current_row, i);
        if (it_pre_row == pre_row.end() || it_current_row == current_row.end()) {
            return false;
        }
        Ref pre_ref = *it_pre_row;
        Ref current_ref = *it_current_row;

        if (pre_ref.getName()->ToString() == current_ref.getName()->ToString()) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool ConditionResultTable::VerifyRefInSet(list<Ref> &col, Ref &ref) {
    for (list<Ref>::iterator it = col.begin(); it != col.end(); it++) {
        Ref current_ref = *it;
        EntType et_cur = current_ref.GetType();
        string name_cur = current_ref.getName()->GetName();

        EntType et_ref = ref.GetType();
        string name_ref = ref.getName()->GetName();

        if (et_cur == et_ref && name_cur == name_ref) {
            return true;
        }
    }
    return false;
}

bool ConditionResultTable::VerifyRowIsUnique(list<Ref> &row, list<list<Ref>> &input_table,  int current_index, list<int> &to_removed) {
    int number_of_row = input_table.front().size();
    bool result = true;
    for (int i = 1; i < number_of_row; i = i + 1) {
        if (i == current_index) {
            continue;
        }
        list<Ref> current_list = ReadRow(i, input_table);
        if (VerifyTwoRowTheSame(row, current_list)) {
            result = false;
            to_removed.push_back(i);
        }
    }
    return result;
}

void ConditionResultTable::RemoveDuplicatesInTable(list<list<Ref>> &input_table) {
    int number_of_row = input_table.front().size();
    int i = 1;
    while (i < number_of_row) {
        list<int> to_removed;
        list<Ref> current_list = ReadRow(i, input_table);
        if (!VerifyRowIsUnique(current_list, input_table, i, to_removed)) {
            ReverseSortList(to_removed);
            DeleteListOfRowOrColFromTable(input_table, to_removed, true);
            number_of_row = input_table.front().size();
        }
        i = i + 1;
    }
    return;
}

list<list<Ref>> ConditionResultTable::SelectDistinctRef(list<Ref> &input_ref, list<list<Ref>> &input_table) {
    list<list<Ref>> result;

    for (list<list<Ref>>::iterator it = input_table.begin(); it != input_table.end(); it++) {
        list<Ref> current_list = *it;
        Ref first_element = current_list.front();
        if (VerifyRefInSet(input_ref, first_element)) {
            result.push_back(current_list);
        }
    }

    if (!result.empty() && result.front().size() - 1 > 0) {
        RemoveDuplicatesInTable(result);
    }
    return result;
}