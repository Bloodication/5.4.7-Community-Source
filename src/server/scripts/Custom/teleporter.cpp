// Courtesy of Dogmar.

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
 
class tele : public CreatureScript
{
    public:
 
        tele()
            : CreatureScript("tele")
        {
        }
bool OnGossipHello(Player* Plr, Creature* pCrea)
{
        //Plr->ADD_GOSSIP_ITEM( 12, "Welcome to Project Xuen", GOSSIP_SENDER_MAIN, 8888);
	
        // Main Menu for Alliance
        if (Plr->GetTeam() == ALLIANCE)
        {
			if(Plr->getClass() != CLASS_HUNTER) {
				Plr->ADD_GOSSIP_ITEM( 7, "Alliance Mall", GOSSIP_SENDER_MAIN, 1001);
				Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
				Plr->ADD_GOSSIP_ITEM( 9, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
			}
			else
			{
				Plr->ADD_GOSSIP_ITEM( 7, "Alliance Mall", GOSSIP_SENDER_MAIN, 1001);
				Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
				Plr->ADD_GOSSIP_ITEM( 9, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
				//Plr->ADD_GOSSIP_ITEM(7, "Hunter Mall", GOSSIP_SENDER_MAIN, 2005);
			}
        }
        else // Main Menu for Horde
        {
			if(Plr->getClass() != CLASS_HUNTER) {
				Plr->ADD_GOSSIP_ITEM( 7, "Horde Mall", GOSSIP_SENDER_MAIN, 2001);
				Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
				Plr->ADD_GOSSIP_ITEM( 9, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
			}
			else
			{
				Plr->ADD_GOSSIP_ITEM( 7, "Horde Mall", GOSSIP_SENDER_MAIN, 2001);
				Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
				Plr->ADD_GOSSIP_ITEM( 9, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
				//Plr->ADD_GOSSIP_ITEM(7, "Hunter Mall", GOSSIP_SENDER_MAIN, 2005);
			}
			
        }
 
        Plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCrea->GetGUID());
return true;
}
 
 bool OnGossipSelect(Player* Plr, Creature* pCrea, uint32 /*uiSender*/, uint32 action)
{
 
switch(action)
 
{

case 8888:
        Plr->PlayerTalkClass->ClearMenus();
        OnGossipHello(Plr, pCrea);
        break;
	
 
 
case 5200:
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(0, -388.145996f, 1543.670044f, 16.853933f, 3.1017100f);
        break;
 
        case 3500:
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(0, -13245.379883f, 194.428299f, 30.992887f, 1.098357f);
        break;
 
       
case 1000: //Alliance Town
        Plr->PlayerTalkClass->ClearMenus();
        Plr->ADD_GOSSIP_ITEM( 5, "Alliance Mall", GOSSIP_SENDER_MAIN, 1001);
        Plr->ADD_GOSSIP_ITEM( 7, "<- Main Menu", GOSSIP_SENDER_MAIN, 5005);
        Plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCrea->GetGUID());
break;
 
 
case 5005: //Back To Main Menu
        Plr->PlayerTalkClass->ClearMenus();
        // Main Menu for Alliance
	//Plr->ADD_GOSSIP_ITEM( 12, "Welcome to Project Hellbane", GOSSIP_SENDER_MAIN, 8888);
        // Main Menu for Alliance
        if (Plr->GetTeam() == ALLIANCE)
        {
        Plr->ADD_GOSSIP_ITEM( 7, "Alliance Mall", GOSSIP_SENDER_MAIN, 1001);
        Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
        Plr->ADD_GOSSIP_ITEM( 7, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
        }
        else // Main Menu for Horde
        {
        Plr->ADD_GOSSIP_ITEM( 7, "Horde Mall", GOSSIP_SENDER_MAIN, 2001);
        Plr->ADD_GOSSIP_ITEM( 7, "Duel Zone", GOSSIP_SENDER_MAIN, 3001);
        Plr->ADD_GOSSIP_ITEM( 9, "Gurubashi", GOSSIP_SENDER_MAIN, 3005);
        }
        //Plr->ADD_GOSSIP_ITEM( 7, "Transmog Mall Placeholder", GOSSIP_SENDER_MAIN, 3010);
        Plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCrea->GetGUID());
break;
 
//////////////////////////////////////////////////ALLIANCE///////////////////////////////////////////////////////////////
 
case 1001: // Alliance Mall
        Plr->CLOSE_GOSSIP_MENU();
	//MAP, x, y , z, o
        Plr->TeleportTo(870, 858.329407f, 2001.977661f, 316.074554f, 3.114956f);
       
break;

//////////////////////////////////////////////////HORDE///////////////////////////////////////////////////////////////
 
case 2001: // Horde Mall
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(870, 858.329407f, 2001.977661f, 316.074554f, 3.114956f);
       
break;
 
case 2005: // Hunter Mall
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(730, 852.109009f, 1038.969971f, -5.400400f, 4.845880f);
       
break;

case 3001:// Duel Zone
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(870, 3971.35f, 1862.69f, 904.65f, 0.59f);
       
break;
 
case 3005:// Gurubashi
        Plr->CLOSE_GOSSIP_MENU();
        Plr->TeleportTo(0, -13230.45f, 223.70f, 32.55f, 1.10f);
       
break;
 

       
        }
		return true;
}
 
 
};
void AddSC_tele()
{
        new tele();
}