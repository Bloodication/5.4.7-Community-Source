#include "ScriptPCH.h"

class Vip_Access: public PlayerScript
{
    public:
        Vip_Access() : PlayerScript("Vip_Access") {}

    void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/)
    {
    	if (player->GetAreaId() == 5861 && player->GetSession()->GetSecurity() == SEC_PLAYER)
    	{
		player->TeleportTo(870, 3971.35f, 1862.69f, 904.65f, 0.59f);
		ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Information]:|r You are not allowed to be here, you aren't a VIP!|r!");
    	}
	}
};

void AddSC_Vip_Access()
{
    new Vip_Access();
}