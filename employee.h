#ifndef EMPLOYEE_EE
#define EMPLOYEE_EE
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;

    bool operator==(const employee& e) const {
        return num == e.num && std::string(name) == std::string(e.name) && hours == e.hours;
    };
};

#endif