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
            
            bool exitb = false;
            while (true) {
                std::string state;
                std::cout << "send data to server or not(y/n): ";
                std::cin >> state;
                if (state == "y") {
                    break;
                } else if (state == "n") {
                    exitb = true;
                    break;
                } else {
                    std::cout << "wrong command. repeating\n";
                }
            }
            char continuec;
            if (exitb) {
                continuec = 'e';
            } else {
                continuec = 'c';
            }
            if (!WriteFile(file, &continuec, sizeof(continuec), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(continuec)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(continuec);
            }
            
            if (!exitb) {
                if (!WriteFile(file, &e, sizeof(e), &readBytes, NULL)) {
                    std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                    exit(three());
                }
                if (readBytes != sizeof(e)) {
                    std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
                }
                
            }
            
            std::cout << "Press any key to end modification";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::getchar();
            char endc = 'e';
            
            if (!WriteFile(file, &endc, sizeof(endc), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(endc)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(endc);
            }
            
            // end
            
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
            
            std::cout << "Press any key to end reading";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::getchar();
            char endc = 'e';
            
            if (!WriteFile(file, &endc, sizeof(endc), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(three());
            }
            if (readBytes != sizeof(endc)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(endc);
            }
        } else if (whatToDo == "e") {
            return 0;
        } else {
            std::cerr << "got unknown command: repeating" << '\n';
        }
    }
    
}