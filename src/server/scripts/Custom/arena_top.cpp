#include "ScriptPCH.h"

using namespace std;

class npc_arena_teamTop : public CreatureScript
{
public:
	npc_arena_teamTop() : CreatureScript("npc_arena_teamTop") { }

	void MainMenu(Player*player, Creature*creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[View top 2v2 teams]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF - 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[View top 3v3 teams]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF - 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[View top 5v5 teams]", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF - 3);
		///player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Nevermind", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
		player->SEND_GOSSIP_MENU(creature->GetEntry(), creature->GetGUID());
	}

	bool OnGossipHello(Player * player, Creature * creature)
	{
		MainMenu(player, creature);
		return true;
	}

	bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (actions)
			{
			case GOSSIP_ACTION_INFO_DEF - 1:
			{
				QueryResult result = CharacterDatabase.Query("SELECT rating0, guid FROM character_arena_data ORDER BY rating0 DESC LIMIT 15");
				if (!result)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("There are no top arena teams in the selected bracket.");
					player->CLOSE_GOSSIP_MENU();
					return false;
				}

				Field * fields = NULL;
				//player->MonsterWhisper("|cff4169E1Here are the top 10 2v2 arena teams:|r", player->GetGUID());
				player->PlayerTalkClass->ClearMenus();
				do
				{
					fields = result->Fetch();
					uint32 playerrating = fields[0].GetUInt32();
					uint32 playerguid = fields[1].GetUInt32();
					char msg[250];
					snprintf(msg, 250, "[Name: POTATO, Rating: %u]", playerrating);
					player->ADD_GOSSIP_ITEM(7, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + playerguid);
					player->SEND_GOSSIP_MENU(creature->GetEntry(), creature->GetGUID());
					
				} while (result->NextRow());

			}break;

			case GOSSIP_ACTION_INFO_DEF - 2:
			{
				QueryResult result = CharacterDatabase.Query("SELECT rating1, guid FROM character_arena_data ORDER BY rating1 DESC LIMIT 15");
				if (!result)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("There are no top arena teams in the selected bracket.");
					player->CLOSE_GOSSIP_MENU();
					return false;
				}

				Field * fields = NULL;
				//player->MonsterWhisper("|cff4169E1Here are the top 10 2v2 arena teams:|r", player->GetGUID());
				player->PlayerTalkClass->ClearMenus();
				do
				{
					fields = result->Fetch();
					uint32 playerrating = fields[0].GetUInt32();
					uint32 playerguid = fields[1].GetUInt32();
					char msg[250];
					snprintf(msg, 250, "[Name: POTATO, Rating: %u]", playerrating);
					player->ADD_GOSSIP_ITEM(7, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + playerguid);
					player->SEND_GOSSIP_MENU(creature->GetEntry(), creature->GetGUID());
				} while (result->NextRow());

			}break;

			case GOSSIP_ACTION_INFO_DEF - 3:
			{
				QueryResult result = CharacterDatabase.Query("SELECT rating2, guid FROM character_arena_data ORDER BY rating2 DESC LIMIT 15");
				if (!result)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("There are no top arena teams in the selected bracket.");
					player->CLOSE_GOSSIP_MENU();
					return false;
				}

				Field * fields = NULL;
				//player->MonsterWhisper("|cff4169E1Here are the top 10 2v2 arena teams:|r", player->GetGUID());
				player->PlayerTalkClass->ClearMenus();
				do
				{
					fields = result->Fetch();
					uint32 playerrating = fields[0].GetUInt32();
					uint32 playerguid = fields[1].GetUInt32();
					char msg[250];
					snprintf(msg, 250, "[Name: POTATO, Rating: %u]", playerrating);
					player->ADD_GOSSIP_ITEM(7, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + playerguid);
					player->SEND_GOSSIP_MENU(creature->GetEntry(), creature->GetGUID());
				} while (result->NextRow());
			}break;
			case GOSSIP_ACTION_INFO_DEF - 4:
				MainMenu(player, creature);
				return true;
				break;

			}
			//if (actions > 1000)
			//{
			//        uint32 teamid = actions - GOSSIP_ACTION_INFO_DEF;
			//        ArenaTeam*team = sArenaTeamMgr->GetArenaTeamById(teamid);
			//        char msg[1024];
			//        string capname;
			//        sObjectMgr->GetPlayerNameByGUID(team->GetCaptain(), capname);
			//        char rank[128];
			//        if (team->getRank() == 0)
			//            snprintf(rank, 128, "No rank");
			//        else
			//            snprintf(rank, 128, "Rank: %u", team->getRank());
			//        player->PlayerTalkClass->ClearMenus();
			//        snprintf(msg, 1024, "[%s, Captain: %s]\n[Season games: %u, Wins: %u, Losses: %u]\n[Week games: %u, Wins: %u, Losses: %u]",
			//            rank, capname.c_str(),
			//            team->GetSeasonGames(), team->GetSeasonWins(), (team->GetSeasonGames() - team->GetSeasonWins()), 
			//            team->GetWeekGames(), team->GetWeekWins(), (team->GetWeekGames() - team->GetWeekWins())
			//            );
			//        player->ADD_GOSSIP_ITEM(9, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF-4);
			//        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF-4);
			//        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF-4);
			//        player->SEND_GOSSIP_MENU(creature->GetEntry(), creature->GetGUID());
			//        return true;
			//}
		}
		return true;
	}
};

void AddSC_npc_arena_setup()
{
	new npc_arena_teamTop;
}