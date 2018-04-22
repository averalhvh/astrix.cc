
#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Namestealer
		ChatSpamName();
		break;
	case 2:
		// Regular
		ChatSpamRegular();
		break;
	case 3:
		// Interwebz
		ChatSpamInterwebz();
		break;
	case 4:
		// Astrix Spam
		Namespam();
		break;
	case 5:
		// ASTRIX Spam
		AstrixName();
		break;
	}
}

void CMiscHacks::RotateMovement(CUserCmd * pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);
	float cosr, sinr;
	cosr = cos(rotation);
	sinr = sin(rotation);
	float forwardmove, sidemove;
	forwardmove = (cosr * pCmd->forwardmove) - (sinr * pCmd->sidemove);
	sidemove = (sinr * pCmd->forwardmove) - (cosr * pCmd->sidemove);
	pCmd->forwardmove = forwardmove;
	pCmd->sidemove = sidemove;
}

void CMiscHacks::doCircularStrafe(CUserCmd * pCmd, Vector & Originalview)
{
	int CircleKey = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();
	IClientEntity * pLocal = hackManager.pLocal();
	Vector View(pCmd->viewangles);
	float niglet = 0;
	if (GetAsyncKeyState(CircleKey))
	{
		pCmd->forwardmove = 450.f;
		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = View.y;
		if (pLocal->GetVelocity().Length()> 50.f)
		{
			niglet += 0.00007;
			current_y += 3 - niglet;
		}
		else
		{
			niglet = 0;
		}
		View.y = current_y;
		if (random == random2)
			View.y += random;
		// Clamp (View);
		RotateMovement(pCmd, current_y);
	}
	else
	{
		float niglet = 0;
	}
}


void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff


	if (Menu::Window.MiscTab.Logs.GetState())
	{

		ConVar* Developer = Interfaces::CVar->FindVar("developer");
		*(float*)((DWORD)&Developer->fnChangeCallback + 0xC) = NULL;
		Developer->SetValue("1");

		ConVar* con_filter_enable = Interfaces::CVar->FindVar("con_filter_enable");
		*(float*)((DWORD)&con_filter_enable->fnChangeCallback + 0xC) = NULL;
		con_filter_enable->SetValue("2");

		ConVar* con_filter_text = Interfaces::CVar->FindVar("con_filter_text");
		*(float*)((DWORD)&con_filter_text->fnChangeCallback + 0xC) = NULL;
		con_filter_text->SetValue("[astrix.cc]");
	}

	if (Menu::Window.MiscTab.LogsBuy.GetState())
	{

		ConVar* Developer = Interfaces::CVar->FindVar("developer");
		*(float*)((DWORD)&Developer->fnChangeCallback + 0xC) = NULL;
		Developer->SetValue("1");

		ConVar* con_filter_enable = Interfaces::CVar->FindVar("con_filter_enable");
		*(float*)((DWORD)&con_filter_enable->fnChangeCallback + 0xC) = NULL;
		con_filter_enable->SetValue("2");

		ConVar* con_filter_text = Interfaces::CVar->FindVar("con_filter_text");
		*(float*)((DWORD)&con_filter_text->fnChangeCallback + 0xC) = NULL;
		con_filter_text->SetValue("[astrix.cc]");
	}
	
	// Bhop
	switch (Menu::Window.MiscTab.OtherAutoJump.GetState())
	{
	case 0:
		break;
	case 1:
		AutoJump(pCmd);
		break;
	}

	if (Menu::Window.MiscTab.OtherCircleStrafe.GetState() && Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey() > 0)
	{
		int CircleKey = Menu::Window.MiscTab.OtherCircleStrafeKey.GetKey();
		if (GetAsyncKeyState(CircleKey))
		{
			doCircularStrafe(pCmd, AutoStrafeView);
		}
	}

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	if (Menu::Window.MiscTab.OtherSlowMotion.GetKey())
		SlowMo(pCmd);

	if (Menu::Window.MiscTab.OtherFakeWalk.GetKey())
		FakeWalk(pCmd, bSendPacket);

	if (Menu::Window.VisualsTab.DisablePostProcess.GetState())
		PostProcces();

	if (Menu::Window.VisualsTab.OptionsGray.GetState())
		DrawGray();
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::DrawGray()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_drawgray");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	if (Menu::Window.VisualsTab.OptionsGray.GetState())
		meme_spoofed->SetInt(1);
	else
		meme_spoofed->SetInt(0);
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	static auto bJumped = false;
	static auto bFake = false;
	if(!bJumped && bFake)
	{ 
		bFake = false;
		pCmd->buttons |= IN_JUMP;
	}
	else if(pCmd->buttons & IN_JUMP)
	{
		if(hackManager.pLocal()->GetFlags() & FL_ONGROUND)
		{
			bJumped = true;
			bFake   = true;
		}
		else
		{
			pCmd->buttons &= ~IN_JUMP;
			bJumped = false;
		}
	}
	else
	{
		bJumped = false;
		bFake = false;
	}
}

void CMiscHacks::SlowMo(CUserCmd *pCmd)
{
	int SlowMotionKey = Menu::Window.MiscTab.OtherSlowMotion.GetKey();
	if (SlowMotionKey > 0 && GetAsyncKeyState(SlowMotionKey))
	{
		static bool slowmo;
		slowmo = !slowmo;
		if (slowmo)
		{
			pCmd->tick_count = INT_MAX;
		}
	}
}

void CMiscHacks::FakeWalk(CUserCmd* pCmd, bool &bSendPacket) // less autistic fakewalk?
{
	IClientEntity* pLocal = hackManager.pLocal();
	int key1 = Menu::Window.MiscTab.OtherFakeWalk.GetKey();
	if (key1 >0 && GetAsyncKeyState(key1))
	{
		static int iChoked = -1;
		iChoked++;

		if (iChoked < 3)
		{
			bSendPacket = false;
			pCmd->tick_count += 10;
			pCmd += 7 + pCmd->tick_count % 2 ? 0 : 1;
			pCmd->buttons |= pLocal->GetMoveType() == IN_BACK;
			pCmd->forwardmove = pCmd->sidemove = 0.f;
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
			Interfaces::Globals->frametime *= (pLocal->GetVelocity().Length2D()) / 1.f;
			pCmd->buttons |= pLocal->GetMoveType() == IN_FORWARD;
		}

	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{

	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static bool bDirection = true;

	static float move = 450.f;
	float s_move = move * 0.5065f;
	static float strafe = pCmd->viewangles.y;
	float rt = pCmd->viewangles.y, rotation;

	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{

		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		rotation = strafe - rt;

		strafe = rt;

		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;

		bool bKeysPressed = true;

		if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x57) || GetAsyncKeyState(0x53) || GetAsyncKeyState(0x44))
			bKeysPressed = false;
		if (pCmd->buttons & IN_ATTACK)
			bKeysPressed = false;

		float flYawBhop = 0.f;

		float sdmw = pCmd->sidemove;
		float fdmw = pCmd->forwardmove;

		static float move = 420.f;
		float s_move = move * 0.5276f;
		static float strafe = pCmd->viewangles.y;

		if (!GetAsyncKeyState(VK_RBUTTON))
		{
			if (pLocal->GetVelocity().Length() > 45.f)
			{
				float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

				z = x / y;
				z = fabsf(z);

				a = x * z;

				flYawBhop = a;
			}

			if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
			{

				if (bDirection)
				{
					AutoStrafeView -= flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = -410;
					bDirection = false;
				}
				else
				{
					AutoStrafeView += flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = 430;
					bDirection = true;
				}

				if (pCmd->mousedx < 0)
				{
					pCmd->forwardmove = 30;
					pCmd->sidemove = -420;
				}

				if (pCmd->mousedx > 0)
				{
					pCmd->forwardmove = +30;
					pCmd->sidemove = 420;
				}
			}
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

// …e Õ½Ê¿

void CMiscHacks::ChatSpamInterwebz()
{
	change_name("CYA@MEDIA ™C™Y™A™™@™™M™E™D™I™A™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™C™Y™A™™@™™M™E™D™I™A™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™C™Y™A™™@™™M™E™D™I™A™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™™™™C™Y™A™™@™™M™E™D™I™A™™™™™C™Y™A™™™™A™Y™Y™W™A™R™E™");
}

void CMiscHacks::Namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("™ASTRIX");
			useSpace = !useSpace;
		}
		else
		{
			change_name("™ ASTRIX");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::AstrixName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("-ASTRIX");
			useSpace = !useSpace;
		}
		else
		{
			change_name(" ASTRIX-");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ.CC SPAM";

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name ("Astrix.cc");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("ASTRIX, THE BEST AYY PASTE");
	}
	else
	{
		SayInChat("ASTRIX, THE BEST AYY PASTE");
	}

	start_t = clock();
}
