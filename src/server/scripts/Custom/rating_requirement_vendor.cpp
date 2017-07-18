#include "ScriptPCH.h"
#include "Chat.h"
#include "WorldSession.h"

class Rating_Vendor : public CreatureScript
{
public:
    Rating_Vendor() : CreatureScript("Rating_Vendor") { }

	struct Rating_VendorAI : public ScriptedAI
	{
		Rating_VendorAI(Creature *c) : ScriptedAI(c)
		{
			
		}
	};
	
	CreatureAI* GetAI(Creature* _creature) const
	{
		return new Rating_VendorAI(_creature);
	}
	
    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Are you sure you want to buy this once?", GOSSIP_SENDER_MAIN, 1);
        player->PlayerTalkClass->SendGossipMenu(creature->GetEntry(), creature->GetGUID());

        return true;
    }

   
   void SendVendor(Player* player, uint64 creatureGuid, uint32 vendorid)
    {
        if (!vendorid)
            return;
		
		player->GetSession()->SendListInventory(creatureGuid);
    }
   

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
	//player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case 1: 
				if (player->GetArenaPersonalRating(ARENA_TYPE_2v2) >= 0)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("You have used the vendor and can now not use it on any other character.");
					player->PlayerTalkClass->ClearMenus();
					SendVendor(player, creature->GetGUID());
					
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("you don't have the required rating to open this vendor!");
					return false;
				}
				break;
            default: 
				return false;
        }
    }

};

void AddSC_Rating_Vendor()
{
    new Rating_Vendor();
}