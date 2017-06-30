#include "ScriptMgr.h"
#include "MapManager.h"
#include "Language.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "DisableMgr.h"

class Skirmish_npc : public CreatureScript
{
public:
    Skirmish_npc() : CreatureScript("Skirmish_npc") {}

    bool OnGossipHello(Player *pPlayer, Creature *pCreature)
    {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Queue for 2v2 Skirmish", GOSSIP_SENDER_MAIN, ARENA_TYPE_2v2);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Queue for 3v3 Skirmish", GOSSIP_SENDER_MAIN, ARENA_TYPE_3v3);
        pPlayer->PlayerTalkClass->SendGossipMenu(1, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *pPlayer, Creature * /*pCreature*/, uint32 /*sender*/, uint32 action)
    {
		if (action != ARENA_TYPE_2v2 && action != ARENA_TYPE_3v3)
            return true;

        if (pPlayer->InBattlegroundQueue())
            return true;

        uint32 arenaRating = 0;
        uint32 matchmakerRating = 0;

		ArenaType arenatype = ArenaType(action);

        //check existance
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
        if (!bg)
        {
            //sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
            return false;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
        {
            ChatHandler(pPlayer->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
            return false;
        }

        BattlegroundTypeId bgTypeId = bg->GetTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), pPlayer->getLevel());
        if (!bracketEntry)
            return false;

        GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

        uint32 avgTime = 0;
        GroupQueueInfo* ginfo = bgQueue.AddGroup(pPlayer, NULL, bgTypeId, bracketEntry, arenatype, false, false, 0, 0);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        // m_bgData.bgQueuesJoinedTime[currentBg->GetTypeID()] = time(NULL);
        
        // add to queue
        uint32 queueSlot = pPlayer->AddBattlegroundQueueId(bgQueueTypeId);
        // add joined time data
        // pPlayer->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);
        
        //WorldPacket data; // send status packet (in queue) TEMP DISABLED TILL 1 ERRO GETS FIXED
       // sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, pPlayer, queueSlot, STATUS_WAIT_QUEUE, avgTime, arenatype);
        //pPlayer->GetSession()->SendPacket(&data);

        sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
        
        pPlayer->PlayerTalkClass->ClearMenus();
        pPlayer->CLOSE_GOSSIP_MENU();
        return true;
    }
};

void AddSC_Skirmish_npc()
{
    new Skirmish_npc;
}