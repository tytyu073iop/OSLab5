#ifndef RFM_M
#define RFM_M
#include "employee.h"
#include <fstream>
#include <map>

struct RFM {
    std::ifstream& in;
    std::ofstream& out;
    std::map<int, long long> ids;

    RFM(std::ifstream& in, std::ofstream& out);
    RFM() = delete;

    void add(employee);
    void edit(int num, employee);
    employee read(int num);
};

#endif