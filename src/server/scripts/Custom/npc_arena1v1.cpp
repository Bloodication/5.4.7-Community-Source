/*
*
* Copyright (C) 2013 Emu-Devstore <http://emu-devstore.com/>
* Written by Teiby <http://www.teiby.de/>
*
*/

#include "ScriptMgr.h"
#include "Common.h"
#include "DisableMgr.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "Language.h"


class npc_1v1arena : public CreatureScript
{
public:
	npc_1v1arena() : CreatureScript("npc_1v1arena")
	{
	}

	bool JoinQueueArena(Player* player, Creature* me, bool isRated)
	{
		if (!player || !me)
			return false;

		uint64 guid = player->GetGUID();
		uint8 arenatype = ARENA_TYPE_5v5;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;

		// ignore if we already in BG or BG queue
		if (player->InBattleground())
			return false;

		//check existance
		Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if (!bg)
		{
			//TC_LOG_ERROR("network", "Battleground: template bg (all arenas) not found");
			return false;
		}

		if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
			return false;
		}

		BattlegroundTypeId bgTypeId = bg->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
		if (!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

		// check if already in queue
		if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
			//player is already in this queue
			return false;
		// check if has free queue slots
		if (!player->HasFreeBattlegroundQueueId())
			return false;

		uint32 ateamId = 0;

		BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

		GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, arenatype, isRated, false, arenaRating, matchmakerRating);
		//GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, arenatype, false, false, 0,0);
		//uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
		uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
		uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

		WorldPacket data;
		// send status packet (in queue)
		sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, player, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, arenatype);
		player->GetSession()->SendPacket(&data);

		sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

		return true;
	}

	bool OnGossipHello(Player* player, Creature* me)
	{
		if (!player || !me)
			return true;

		if (player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_5v5))
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Leave queue 1v1 Arena", GOSSIP_SENDER_MAIN, 3, "Are you sure?", 0, false);
		else
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Join 1v1 Queue", GOSSIP_SENDER_MAIN, 20);


		switch (player->ToPlayer()->GetRoleForGroup(player->ToPlayer()->GetSpecializationId(player->ToPlayer()->GetActiveSpec())))
		{
		case ROLES_DPS:
			player->SEND_GOSSIP_MENU(68, me->GetGUID());
			break;
		case ROLES_HEALER:
		case ROLES_TANK:
			ChatHandler(player->GetSession()).PSendSysMessage("You can't join 1v1 Queue with Tank or Healer spec!");
			player->CLOSE_GOSSIP_MENU();
			break;
		default:
			ChatHandler(player->GetSession()).PSendSysMessage("You need to choose a specilization!");
		}
		return true;
	}



	bool OnGossipSelect(Player* player, Creature* me, uint32 /*uiSender*/, uint32 uiAction)
	{
		if (!player || !me)
			return true;

		player->PlayerTalkClass->ClearMenus();

		switch (uiAction)
		{

		case 20: // Join Queue Arena (unrated)
		{
			if (JoinQueueArena(player, me, false) == false)
				ChatHandler(player->GetSession()).SendSysMessage("Something went wrong while join queue.");

			player->CLOSE_GOSSIP_MENU();
			return true;
		}
		break;

		case 3: // Leave Queue
		{
			WorldPacket Data;
			Data << (uint8)0x1 << (uint8)0x0 << (uint32)BATTLEGROUND_AA << (uint16)0x0 << (uint8)0x0;
			player->GetSession()->HandleBattleFieldPortOpcode(Data);
			player->CLOSE_GOSSIP_MENU();
			return true;
		}
		break;
		}

		OnGossipHello(player, me);
		return true;
	}
};


void AddSC_npc_1v1arena()
{
	new npc_1v1arena();
}