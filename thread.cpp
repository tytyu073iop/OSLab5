#include "thread.h"
#include "globals.h"
#include "employee.h"
#include "RFM.h"
#include <iostream>

DWORD WINAPI cover(LPVOID arg)
{
    thread(reinterpret_cast<RFM*>(arg));
    return 0;
}

void thread(RFM* rfm)
{
    HANDLE namedPipe = CreateNamedPipe("\\\\.\\pipe\\OSLab5", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 4096*sizeof(TCHAR), 4096*sizeof(TCHAR), 5000, NULL);
    if (!ConnectNamedPipe(namedPipe, NULL)) {
        std::cerr << "Error connecting to pipe: " << GetLastError() << '\n';
        return;
    }
    std::cout << "Connection completed\n";
    
    while (true) {
        char command;
        int num;
        DWORD readBytes;
        if (!ReadFile(namedPipe, &command, sizeof(command), &readBytes, NULL)) {
            std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
            return;
        }
        if (readBytes != sizeof(command)) {
            std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(command);
        }
        if (!ReadFile(namedPipe, &num, sizeof(num), &readBytes, NULL)) {
            std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
            return;
        }
        if (readBytes != sizeof(num)) {
            std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(num);
        }
        
        if (command == 'm') {
            employee e;
            DWORD p;
            const HANDLE arr[]{readEvents[num], writesEvents[num]};
            p = WaitForMultipleObjects(2, arr, TRUE, INFINITE);
            ResetEvent(writesEvents[num]);
            
            e = rfm->read(num);
            
            if (!WriteFile(namedPipe, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                return;
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }
            
            if (!ReadFile(namedPipe, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                return;
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(e);
            }
            
            rfm->edit(num, e);

            char endc;
            if (!ReadFile(namedPipe, &endc, sizeof(endc), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                return;
            }
            if (readBytes != sizeof(endc)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(endc);
            }
            if (endc != 'e') {
                std::cerr << "wrong end symbol. still terminating\n";
            }
            SetEvent(writesEvents[num]);
        } else if (command == 'r') {
            employee e;
            auto p = WaitForSingleObject(writesEvents[num], INFINITE);
            if (p == WAIT_FAILED) {
                std::cerr << "Wait failed: " << GetLastError() << '\n';
            }
            EnterCriticalSection(&varCS[num]);
            counterReadEvent[num]++;
            ResetEvent(readEvents[num]);
            LeaveCriticalSection(&varCS[num]);
            
            e = rfm->read(num);
            
            if (!WriteFile(namedPipe, &e, sizeof(e), &readBytes, NULL)) {
                std::cerr << "Cannot write from pipe: " << GetLastError() << '\n';
                return;
            }
            if (readBytes != sizeof(e)) {
                std::cerr << "Caution: not full data write(read, should): " << readBytes << ' ' << sizeof(e);
            }

            char endc;
            if (!ReadFile(namedPipe, &endc, sizeof(endc), &readBytes, NULL)) {
                std::cerr << "Cannot read from pipe: " << GetLastError() << '\n';
                return;
            }
            if (readBytes != sizeof(endc)) {
                std::cerr << "Caution: not full data read(read, should): " << readBytes << ' ' << sizeof(endc);
            }
            if (endc != 'e') {
                std::cerr << "wrong end symbol. still terminating\n";
            }
            
            EnterCriticalSection(&varCS[num]);
            counterReadEvent[num]--;
            if (counterReadEvent[num] <= 0) {
                SetEvent(readEvents[num]);
                counterReadEvent[num] = 0;
            }
            LeaveCriticalSection(&varCS[num]);
        } else {
            std::cerr << "got unknown command: " << command << '\n';
        }
    }
}
