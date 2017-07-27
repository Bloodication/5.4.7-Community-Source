#include "ScriptPCH.h"
#include "Chat.h"
#include "WorldSession.h"
#include "AccountMgr.h"
#include "Config.h"

typedef std::set<uint32> DisabledPlrs;

class Rating_Vendor : public CreatureScript
{
private:
	DisabledPlrs m_plrList;
	DisabledPlrs m_charList;
public:
    Rating_Vendor() : CreatureScript("Rating_Vendor") 
	{
	
	}

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
		
        switch (action)
        {
            case 1:
				QueryResult result = WorldDatabase.Query("SELECT guid from gear_vendor_used");

				do {
					if (uint32 entry = result->Fetch()[0].GetUInt32())
						m_plrList.insert(entry);
				} while (result->NextRow());
				
				QueryResult result2 = WorldDatabase.Query("SELECT charid from gear_vendor_used2");
					do {
						if (uint32 entry = result2->Fetch()[0].GetUInt32())
							m_charList.insert(entry);
					} while (result2->NextRow());
				
				if (m_plrList.find(player->GetSession()->GetAccountId()) != m_plrList.end())
				{
					if (m_charList.find(player->GetGUID()) != m_charList.end())
					{
						player->PlayerTalkClass->ClearMenus();
						player->GetSession()->SendListInventory(creature->GetGUID());
					}
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("You have already used this vendor before and cannot use it again!");
						return false;
					}
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("You have now used the vendor and cannot use it on any other character.");
					player->PlayerTalkClass->ClearMenus();
					player->GetSession()->SendListInventory(creature->GetGUID());
					WorldDatabase.PExecute("INSERT INTO `gear_vendor_used` (`guid`) VALUES (%u)", player->GetSession()->GetAccountId());
					WorldDatabase.PExecute("INSERT INTO `gear_vendor_used2` (`charid`) VALUES (%u)", player->GetGUID());
				}
				break;
        }
    }

};

void AddSC_Rating_Vendor()
{
    new Rating_Vendor();
}