#include "globals.h"

std::map<int, HANDLE> writesEvents;
std::map<int, HANDLE> readEvents;
std::map<int, CRITICAL_SECTION> varCS;
std::map<int, int> counterReadEvent;