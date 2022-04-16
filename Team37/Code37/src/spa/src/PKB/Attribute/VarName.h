#ifndef INC_21S2_CP_SPA_TEAM_37_VARNAME_H
#define INC_21S2_CP_SPA_TEAM_37_VARNAME_H

#include <string>

using namespace std;

class VarName {
private:
	string name="";

public:
    VarName();
	VarName(string s);
    VarName(const VarName& obj);
    ~VarName();
    static const VarName INVALID_NAME();
    string GetName() const;
    bool IsValid() const;
    void ToString();
    bool operator==(const VarName& other) const;
    bool operator<(const VarName& other) const;
};

struct hashVarName {
public:
    size_t operator() (const VarName& vn) const {
        return hash<string>()(vn.GetName());
    }
};

#endif //INC_21S2_CP_SPA_TEAM_37_VARNAME_H
