#ifndef GLOBALS_MINE
#define GLOBALS_MINE
#include <Windows.h>
#include <map>

extern std::map<int, HANDLE> writesEvents;
extern std::map<int, HANDLE> readEvents;
extern std::map<int, CRITICAL_SECTION> varCS;
extern std::map<int, int> counterReadEvent;

#endif