// General shit
#include "Utilities.h"

// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "MiscHacks.h"
#include "Dumping.h"
#include "AntiAntiAim.h"
#include "hitmarker.h"
#include "lagcomp.h"
#include "CBulletListener.h"


// Used as part of the reflective DLL injection

extern HINSTANCE hAppInstance;
Vector AimPoint;
float intervalPerTick;
bool lbyupdate1;

// Our DLL Instance
float autowalldmgtest[65];
HINSTANCE HThisModule;
bool DoUnload;
bool pEntityLBYUpdate[65];
float pEntityLastUpdateTime[65];
float enemyLBYDelta[65];
int ResolverStage[65];
bool islbyupdate;
bool toggleSideSwitch;
float ProxyLBYtime;
float lineLBY;
float lineRealAngle;
float lineFakeAngle; int LBYBreakerTimer;
bool rWeInFakeWalk;
float fsnLBY;
bool switchInverse;
float testFloat1;
float enemyLBYTimer[65];
float testFloat2;
int shotsfired[65];
float testFloat4;
int historyIdx = 10;
int hittedLogHits[65];
int missedLogHits[65];
float consoleProxyLbyLASTUpdateTime; // This is in ProxyLBY AntiAim.cpp
float enemysLastProxyTimer[65];

#define me (-1173689068)
#define alex (- 1776957446)
#define orten (16681856)
#define global (316407911)
#define aron (1634000399)
#define dragy (- 833291718)
#define slow (1213810804)

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

int InitialThread()
{

	//Utilities::OpenConsole("");

	// Intro banner with info
	PrintMetaHeader();
	//---------------------------------------------------------
	// Initialise all our shit

	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	Interfaces::Initialise(); // Get pointers to the valve classes
	NetVar.RetrieveClasses(); // Setup our NetVar manager 
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	hitmarker::singleton()->initialize();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyNetVarsHooks(); // Errors while Injected donno where
	lagComp = new LagCompensation;
	lagComp->initLagRecord();
	CBulletListener::singleton()->init();

	//Dumping
	//Dump::DumpClassIds();

	//---------------------------------------------------------

	// While our cheat is running

	while (DoUnload == false)
	{
		Sleep(500);
	}

	RemoveNetVarsHooks();
	Hooks::UndoHooks();
	Sleep(500); // Make sure none of our hooks are running
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;

}


// DllMain

// Entry point for our module and also the HWID CHECK for the non autoriseted people lol

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		if (dwSerial == me ||
			dwSerial == alex ||
			dwSerial == dragy ||
			dwSerial == aron ||
			dwSerial == slow ||
			dwSerial == global ||
			dwSerial == orten)
		{
			Sleep(100);
		}
		else
		{
			// when HWID rejected
			MessageBox(NULL, "You don't have acces to the Cheat", "ASTRIX BETA", MB_OK);
			exit(0);
			return TRUE;
		}

		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}