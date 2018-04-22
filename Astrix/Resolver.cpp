#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"

void LowerBodyYawFix(IClientEntity* pEntity)
{
	if (Menu::Window.RageBotTab.LBYCorrection.GetState())
	{
		if (!pEntity) return;
		if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return;
		if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
		if (Interfaces::Engine->GetLocalPlayer()) return;

		auto EyeAngles = pEntity->GetEyeAnglesXY();
		if (pEntity->GetVelocity().Length() > 1 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
			EyeAngles->y = pEntity->GetLowerBodyYaw();
	}
}

void ResolverSetup::Resolve(IClientEntity* pEntity)
{
	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	bool IsMoving;
	if (pEntity->GetVelocity().Length2D() >= 0.5)
		IsMoving = true;
	else
		IsMoving = false;

	ResolverSetup::NewANgles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::newlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::newsimtime = pEntity->GetSimulationTime();
	ResolverSetup::newdelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::newlbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::finaldelta[pEntity->GetIndex()] = ResolverSetup::newdelta[pEntity->GetIndex()] - ResolverSetup::storeddelta[pEntity->GetIndex()];
	ResolverSetup::finallbydelta[pEntity->GetIndex()] = ResolverSetup::newlbydelta[pEntity->GetIndex()] - ResolverSetup::storedlbydelta[pEntity->GetIndex()];
	if (newlby == storedlby)
		ResolverSetup::lbyupdated = false;
	else
		ResolverSetup::lbyupdated = true;

	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 0)
	{
	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 1)//level 3
	{
		resolvokek::resolvemode = 2;
		if (Globals::missedshots > 3 && Globals::missedshots < 21)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			}
			else if (!MeetsLBYReq && lbyupdated)
			{
				switch (Globals::Shots % 4)
				{
				case 1:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 2:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
					break;
				case 3:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
					break;
				case 4:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
					break;
				}
			}
			else
				pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 35;
		}

		else if (Globals::missedshots >= 2 && Globals::missedshots <= 3)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()];
			}
			else
				pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()];
		}
		else
		{
			if (MeetsLBYReq && lbyupdated)
			{
				bool timer = 0;
				if (timer)
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] + rand() % 35;
				else
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] - rand() % 35;
				timer = !timer;
			}
			else
			{
				bool timer = 0;
				if (timer)
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] + rand() % 35;
				else
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] - rand() % 35;
				timer = !timer;
			}
		}
	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 2) //level 2 
	{
		resolvokek::resolvemode = 1;
		if (Resolver::didhitHS)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			}
			switch (Globals::Shots % 30)
			{
			case 1:
				*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
				break;
			case 2:
				*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
				break;
			case 3:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 4:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 40;
				break;
			case 5:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 15;
				break;
			case 6:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 40;
				break;
			case 7:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 50;
				break;
			case 8:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 60;
				break;
			case 9:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 70;
				break;
			case 10:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 80;
				break;
			case 11:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 90;
				break;
			case 12:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 100;
				break;
			case 13:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 110;
				break;
			case 14:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 120;
				break;
			case 15:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 140;
				break;
			case 16:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 150;
				break;
			case 17:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 160;
				break;
			case 18:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 180;
				break;
			case 19:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 200;
				break;
			case 20:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 220;
				break;
			case 21:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 240;
				break;
			case 22:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 260;
				break;
			case 23:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 280;
				break;
			case 24:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 300;
				break;
			case 25:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 320;
				break;
			case 26:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 330;
				break;
			case 27:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 340;
				break;
			case 28:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 345;
				break;
			case 29:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 350;
				break;
			case 30:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 360;
				break;



			}
		}
		else if (MeetsLBYReq && lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
		}

		else if (GetAsyncKeyState(Menu::Window.RageBotTab.SomeShit.GetKey()) && Menu::Window.RageBotTab.SomeShit.GetKey() > 0)
		{
			auto local_player = Interfaces::EntList->GetClientEntity(
				Interfaces::Engine->GetLocalPlayer());
			if (!local_player)
				return;

			Vector viewangles;
			Interfaces::Engine->GetViewAngles(viewangles);
			auto delta = NormalizeYaw(viewangles.y - pEntity->GetEyeAnglesXY()->y);

			if (delta > 0)
				pEntity->GetEyeAnglesXY()->y + 180;
			else
				pEntity->GetEyeAnglesXY()->y - 180;
		}

		else if (!MeetsLBYReq || !lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 35;
		}
		else
			pEntity->GetEyeAnglesXY()->y = rand() % 180;
	}
	LowerBodyYawFix(pEntity);
}

void ResolverSetup::StoreFGE(IClientEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}
void ResolverSetup::StoreThings(IClientEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{

		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::OverrideResolver(IClientEntity* pEntity)
{

	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	int OverrideKey = Menu::Window.RageBotTab.SomeShit.GetKey();

	if (Menu::Window.RageBotTab.SomeShit.GetKey());
	{
		if (GetAsyncKeyState(Menu::Window.RageBotTab.SomeShit.GetKey()))
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 180.f;

			{
				resolvokek::resolvemode = 2;
				if (Globals::missedshots > 4 && Globals::missedshots < 5)
				{
					if (MeetsLBYReq && lbyupdated)
					{
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
					}
					else if (!MeetsLBYReq && lbyupdated)
					{
						switch (Globals::Shots % 4)
						{
						case 1:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 45;
							break;
						case 2:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
							break;
						case 3:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 135;
							break;
						case 4:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
							break;
						}
					}
					else
						pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 35;
				}

		}
	}
	}
}

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
		{

			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity
				|| pEntity == hackManager.pLocal()
				|| pEntity->IsDormant()
				|| !pEntity->IsAlive())
				continue;

			ResolverSetup::Resolve(pEntity);
			ResolverSetup::OverrideResolver(pEntity);
		}
	}
}


