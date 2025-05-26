#include "employee.h"
#include <iostream>
#include <limits>
#include <Windows.h>

int three() {
    std::cerr << "Error happend. Session terminated\n";
    int v;
    std::cin >> v;
    return 3;
}

int main() {
    if (!WaitNamedPipe("\\\\.\\pipe\\OSLab5" ,NMPWAIT_WAIT_FOREVER)) {
        std::cerr << "Error waiting for pipe: " << GetLastError() << '\n';
        exit(three());
    }
    std::cout << "Waited for connection\n";
    HANDLE file = CreateFile("\\\\.\\pipe\\OSLab5", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating file: " << GetLastError() << '\n';
        exit(three());
    }
    
    while (true)
    {
        std::string whatToDo;
        std::cout << "modify, read or exit(m,r,e): ";
        std::cin >> whatToDo;
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
                exit(three());
            }
            if (readBytes != sizeof(command)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(command);
            }
            
            if (!WriteFile(file, &num, sizeof(num), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(num)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(num);
            }
            
            if (!ReadFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            std::string name;
            std::cout << "num: " << e.num << " name: " << e.name << " hours: " << e.hours << '\n';
            while (true) {
                std::cout << "enter new name: ";
                std::cin >> name;
                if (name.size() < 10 || name.empty()) {
                    strcpy(e.name, name.c_str());
                    break;
                } else {
                    std::cout << "name size is more than 9 or empty. Repeating.\n";
                }
            }
            while (true) {
                std::cout << "enter new hours: ";
                std::string hours;
                std::cin >> hours;
                try {
                    e.hours = std::stod(hours);
                    break;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << std::endl;
                }
                std::cerr << "Enter number.\n";
            }
            
            if (!WriteFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }
        } else if (whatToDo == "r") {
            std::cout << "Enter number of employee: ";
            std::cin >> num;
            employee e;
            command = 'r';
            if (!WriteFile(file, &command, sizeof(command), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(command)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(command);
            }
            
            if (!WriteFile(file, &num, sizeof(num), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(num)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(num);
            }
            
            if (!ReadFile(file, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            std::cout << "num: " << e.num << " name: " << e.name << " hours: " << e.hours << '\n';
        } else if (whatToDo == "e") {
            return 0;
        } else {
            std::cerr << "got unknown command: repeating" << '\n';
        }
    }
    
}