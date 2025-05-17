#include "employee.h"
#include <iostream>
#include <fstream>
#include <string>
#include "RFM.h"
#include <Windows.h>
#include "globals.h"
#include "thread.h"

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

    RFM rfm(in, out);

    std::cout << "\tSetup beagn:\n";
    std::cout << "How many students to enter: ";
    size_t howMany;
    std::cin >> howMany;

    writesEvents = new HANDLE[howMany];
    readEvents = new HANDLE[howMany];

    for (size_t i = 0; i < howMany; i++)
    {
        writesEvents[i] = CreateEvent(NULL, TRUE, TRUE, NULL);
        if (writesEvents[i] == NULL) {
            std::cerr << "Cannot create write event: " << GetLastError() << '\n';
        }
        readEvents[i] = CreateEvent(NULL, TRUE, TRUE, NULL);
        if (readEvents[i] == NULL) {
            std::cerr << "Cannot create read event: " << GetLastError() << '\n';
        }

        std::string name;
        int num;
        double hours;
        std::cout << "Enter the number of employee: ";
        std::cin >> num;
        std::cout << "Enter name of employee: ";
        std::cin >> name;
        std::cout << "Enter worked hours for employee: ";
        std::cin >> hours;
        employee e{.num = num, .hours = hours};
        strcpy(e.name, name.c_str());
        
        rfm.add(e);
    }

    std::cout << "\tHere the file:\n";
    //reverse enginiring
    for (size_t i = 0; i < howMany; i++)
    {
        employee e;
        in.read(reinterpret_cast<char*>(&e), sizeof(employee));
        std::cout << "num: " << e.num << " name: " << e.name << " hours: " << e.hours << '\n';
    }

    char processName[] = "client.exe";

    std::cout << "How many clients to start: ";
    size_t amountOfClients;
    std::cin >> amountOfClients;

    PROCESS_INFORMATION* pis = new PROCESS_INFORMATION[amountOfClients];
    STARTUPINFO* sis = new STARTUPINFO[amountOfClients];
    HANDLE* threads = new HANDLE[amountOfClients];
    
    for (size_t i = 0; i < amountOfClients; i++)
    {
        threads[i] = CreateThread(NULL, NULL, cover, &rfm, NULL, NULL);
        if (threads[i] == NULL) {
            std::cerr << "Cannot create thread(i, error): " << i << ' ' << GetLastError() << '\n';
        }

        ZeroMemory(&sis[i], sizeof(STARTUPINFO));
        sis[i].cb = sizeof(STARTUPINFO);

        if (!CreateProcess(NULL, processName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &sis[i], &pis[i])) {
            std::cerr << "Error: cannot create process(i, error): " << i << ' ' << GetLastError() << '\n';
            exit(3);
        }
    }

    HANDLE* ppis = new HANDLE[amountOfClients];
    for (size_t i = 0; i < amountOfClients; i++)
    {
        ppis[i] = pis[i].hProcess;
    }
    
    
    WaitForMultipleObjects(amountOfClients, ppis, TRUE, INFINITE);
    
}