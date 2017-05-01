#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "Language.h"

 
class vipcommands : public CommandScript
{
public:
    vipcommands() : CommandScript("vipcommands") { }
 
    ChatCommand* GetCommands() const
    {
        static ChatCommand vipCommandTable[] =
 
        {
            { "mall",       1,     true, &HandleVipMallCommand,         "", NULL },
			{ "bank",           1, false, &HandleVipBankCommand,               "", NULL },
            { "changerace",    1,  false, &HandleChangeRaceCommand,             "", NULL },
            { "changefaction",  1,  false, &HandleChangeFactionCommand,         "", NULL },
            { "customize",      1,  false, &HandleCustomizeCommand,             "", NULL },
			{ "activate",      0,  false, &HandleActivateCommand,        "", NULL },
 
            { NULL,             0,                     false, NULL,                                           "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "vip",        1,   true, NULL,      "",  vipCommandTable},
               { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

static bool HandleActivateCommand(ChatHandler * handler, const char * args)
{
        Player* player = handler->GetSession()->GetPlayer();
 
        if(player->GetSession()->GetSecurity() >= 1)
        {
                handler->PSendSysMessage("You already have VIP, what the hell are you doing?");
                handler->SetSentErrorMessage(true);
                return false;
        }
 
        if(player->HasItemCount(1010101, 1, false)) // Token ID, Count.
        {
                  PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_ACCESS);
                  stmt->setUInt32(0, player->GetSession()->GetAccountId());
                  stmt->setUInt8(1, 1);
                  stmt->setInt32(2, 1);
                 LoginDatabase.Execute(stmt);
                player->DestroyItemCount(1010101, 1, true, false); // Token ID, Count.
                handler->PSendSysMessage("Your VIP rank has been applied, restart your client to finalize.");
                return true;
        }
        return true;
}

static bool HandlevipCommand(ChatHandler* handler, const char* args)
    {
 
        Player* me = handler->GetSession()->GetPlayer();
 
            me->Say("vip command?", LANG_UNIVERSAL);
            return true;
    }
	
	static bool HandleVipBankCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
        return true;
    }
 
static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {
 
        Player* me = handler->GetSession()->GetPlayer();
                me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                handler->PSendSysMessage("Relog to change race of your character.");
        return true;
    }
 
static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {
 
        Player* me = handler->GetSession()->GetPlayer();
                me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                handler->PSendSysMessage("Relog to change faction of your character.");
        return true;
    }
 
 
static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {
 
        Player* me = handler->GetSession()->GetPlayer();
                me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                handler->PSendSysMessage("Relog to customize your character.");
        return true;
    }
 
static bool HandleVipMallCommand(ChatHandler* handler, const char* args)
    {
 
        Player* me = handler->GetSession()->GetPlayer();
 
        if (me->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            me->SaveRecallPosition();
 
                me->TeleportTo(974,       -4149.999023f,       6412.830078f,      15.622660f,       1.933143f); // MapId, X, Y, Z, O
                return true;
    }
 
       
       
};
 
void AddSC_vipcommands()
{
    new vipcommands();
}