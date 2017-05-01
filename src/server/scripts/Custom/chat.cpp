#include "ScriptPCH.h"
#include "Chat.h"
 
class chat : public CommandScript
{
        public:
                chat() : CommandScript("chat"){}
 
        ChatCommand * GetCommands() const
        {
                static ChatCommand ChatCommandTable[] =
                {
                        {"world",        SEC_PLAYER,             true,           &HandleChatCommand,     "", NULL},
                        {NULL,          0,                              false,          NULL,                                           "", NULL}
                };
 
                return ChatCommandTable;
        }
 
        static bool HandleChatCommand(ChatHandler * handler, const char * args)
        {
                if (!args)
                        return false;
 
                std::string msg = "";
                Player * player = handler->GetSession()->GetPlayer();
 
                switch(player->GetSession()->GetSecurity())
                {
                        case SEC_PLAYER:
                                if (player->GetTeam() == ALLIANCE)
                                {
										msg += "|cffff0000[World]";
                                        msg += "|cff4169e1[Player] |cffffffff[";
                                        msg += player->GetName();
                                        msg += "] |cFFA9A9A9";
                                }
                                else
                                {
										msg += "|cffff0000[World]";
                                        msg += "|cffff0000[Player] |cffffffff[";
                                        msg += player->GetName();
                                        msg += "] |cFFA9A9A9";
                                }
                                break;
                        case SEC_VIP:
								if (player->GetTeam() == ALLIANCE)
								{
										msg += "|cffff0000[World]";
										msg += "|TInterface/PvPRankBadges/PvPRank14:20:20|t";
										msg += "|cff4169e1[VIP] |cffffffff[";
										msg += player->GetName();
										msg += "] |cFFA9A9A9";
								}
								else
								{
										msg += "|cffff0000[World]";
										msg += "|TInterface/PvPRankBadges/PvPRank14:20:20|t";
										msg += "|cffff0000[VIP] |cffffffff[";
										msg += player->GetName();
										msg += "] |cFFA9A9A9";
								}
                                break;
                        case SEC_MODERATOR:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cff00ffff[Trial GM] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_GAMEMASTER:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cfffa9900[GM] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_CONFIRMED_GAMEMASTER:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cfffa9900[Head GM] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_REALM_LEADER:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cfffa9900[Dev] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_GM_LEADER:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cFF87CEEB[Executive] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_ADMINISTRATOR:
								msg += "|cffff0000[World]";
								msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";
                                msg += "|cFF87CEEB[Owner] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cFF87CEEB";
                                break;
                        case SEC_CONSOLE:
                                msg += "|cfffa9900[CONSOLE] |cffffffff[";
                                msg += player->GetName();
                                msg += "] |cffB400B4";
                                break;
 
                }
                       
                msg += args;
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);  
 
                return true;
        }
};
 
void AddSC_chat()
{
        new chat();
}