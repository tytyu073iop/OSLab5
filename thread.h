#ifndef THREAD_MINE
#define THREAD_MINE
#include <Windows.h>
#include "RFM.h"

DWORD WINAPI cover(LPVOID arg);

void thread(RFM* rfm);

#endif