#include "thread.h"
#include "globals.h"
#include "employee.h"
#include "RFM.h"
#include <iostream>

VOID WINAPI cover(LPVOID arg)
{
    thread();
}

void thread(RFM rfm)
{
    if (!ConnectNamedPipe(*(namedPipe), NULL)) {
        std::cerr << "Error connecting to pipe: " << GetLastError() << '\n';
        exit(3);
    }
    
    while (true) {
        char command;
        int num;
        DWORD readBytes;
        if (!ReadFile(*(namedPipe), &command, sizeof(command), &readBytes, NULL)) {
            std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
            exit(3);
        }
        if (readBytes != sizeof(command)) {
            std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(command);
        }
        if (!ReadFile(*(namedPipe), &num, sizeof(num), &readBytes, NULL)) {
            std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
            exit(3);
        }
        if (readBytes != sizeof(num)) {
            std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(num);
        }
        
        if (command == 'm') {
            employee e;
            auto p = WaitForSingleObject(writesEvents[num], INFINITE);
            if (p == WAIT_FAILED) {
                std::cerr << "Wait failed: " << GetLastError() << '\n';
            }
            ResetEvent(readEvents[num]);
            e = rfm.read(num);
            SetEvent(readEvents[num]);
            if (!WriteFile(*(namedPipe), &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }
            
            if (!ReadFile(*(namedPipe), &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            
            auto p = WaitForSingleObject(readEvents[num], INFINITE);
            if (p == WAIT_FAILED) {
                std::cerr << "Wait failed: " << GetLastError() << '\n';
            }
            ResetEvent(writesEvents[num]);
            rfm.edit(num, e);
            SetEvent(writesEvents[num]);
        } else if (command == 'r') {
            employee e;
            auto p = WaitForSingleObject(writesEvents[num], INFINITE);
            if (p == WAIT_FAILED) {
                std::cerr << "Wait failed: " << GetLastError() << '\n';
            }
            ResetEvent(readEvents[num]);
            e = rfm.read(num);
            SetEvent(readEvents[num]);
            if (!WriteFile(*(namedPipe), &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                exit(3);
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }
        } else {
            std::cerr << "got unknown command: " << command << '\n';
        }
    }
}
