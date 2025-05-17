#include "employee.h"
#include <iostream>
#include <Windows.h>

int main() {
    HANDLE file = CreateFile("\\\\.\\pipe\\OSLab5", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, NULL, NULL, NULL);
    if (file == NULL) {
        std::cerr << "Error creating file: " << GetLastError() << '\n';
        exit(3);
    }
    
    while (true)
    {
        std::string whatToDo;
        std::cout << "modify, read or exit(m,r,e): ";
        DWORD readBytes;
        char command;
        int num;
        
        if (whatToDo == "m") {
            std::cout << "Enter number of employee: ";
            std::cin >> num;
            employee e;
            command = 'm';
            if (!WriteFile(file, &command, sizeof(command), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(command)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(command);
            }
            
            if (!WriteFile(file, &num, sizeof(num), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(num)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(num);
            }
            
            if (!ReadFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            std::cout << "num: " << e.num << " name: " << e.name << " hours: " << e.hours << '\n';
            std::cout << "enter new name: ";
            std::cin >> e.name;
            std::cout << "enter new hours: ";
            std::cin >> e.hours;
            
            if (!WriteFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }
        } else if (command == 'r') {
            std::cout << "Enter number of employee: ";
            std::cin >> num;
            employee e;
            command = 'r';
            if (!WriteFile(file, &command, sizeof(command), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(command)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(command);
            }
            
            if (!WriteFile(file, &num, sizeof(num), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(num)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(num);
            }
            
            if (!ReadFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            std::cout << "num: " << e.num << " name: " << e.name << " hours: " << e.hours << '\n';
        } else if (whatToDo == "e") {
            return 0;
        } else {
            std::cerr << "got unknown command: repeating" << command << '\n';
        }
    }
    
}