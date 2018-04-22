
#include "MetaInfo.h"
#include "Utilities.h"

void PrintMetaHeader()
{
	printf("                                  ASTRIX");
	Utilities::SetConsoleColor(FOREGROUND_INTENSE_RED);
	printf(".vip\n");
	Utilities::SetConsoleColor(FOREGROUND_WHITE);
	Utilities::Log("Build %s", __DATE__);
	Utilities::Log("Setting Up astrix.cc for %s", CORRUPTION_META_GAME);
}