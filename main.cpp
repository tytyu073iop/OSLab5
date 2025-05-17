#include "employee.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string filename;
    std::cout << "Enter name of file: ";
    std::cin >> filename;
    if (filename.empty()) {
        std::cout << "Filename is empty!\n";
        return 1;
    }
    filename += ".bin";

    std::ofstream out(filename);
    if (!out) {
        std::cout << "Something wrong with file creation\n";
        return 1;
    }
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Something wrong with file reading\n";
        return 1;
    }

    std::cout << "\tSetup beagn:\n";
    std::cout << "How many students to enter: ";
    size_t howMany;
    std::cin >> howMany;

    for (size_t i = 0; i < howMany; i++)
    {
        /* code */
    }
    
}