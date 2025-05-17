#include "RFM.h"
#include <iostream>

RFM::RFM(std::ifstream& in2, std::ofstream& out2) : in(in2), out(out2)
{
}

// error handling lets make inside exit

void RFM::add(employee e) {
    out.seekp(0, std::ios::end);
    if (ids.contains(e.num)) {
        std::cout << "error: id exists\n";
        exit(2);
    }
    ids[e.num] = out.tellp();
    out.write(reinterpret_cast<const char*>(&e), sizeof(employee));
}

void RFM::edit(int num, employee e) {
    if (!ids.contains(num)) {
        std::cout << "error: num does not exist\n";
        exit(2);
    }
    out.seekp(ids[num]);
    out.write(reinterpret_cast<const char*>(&e), sizeof(employee));
}

employee RFM::read(int num) {
    if (!ids.contains(num)) {
        std::cout << "error: num do not exist for reading\n";
        exit(2);
    }
    in.seekg(ids[num]);
    employee e;
    in.read(reinterpret_cast<char*>(&e), sizeof(employee));
    return e;
}