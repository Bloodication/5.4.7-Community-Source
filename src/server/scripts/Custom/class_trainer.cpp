#include "ScriptPCH.h"

class npc_globalclasstrainer : public CreatureScript
{
public: npc_globalclasstrainer() : CreatureScript("npc_globalclasstrainer") { }
        void MainMenu(Player* player, Creature* creature)
        {
            //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "[Train glyphs]", GOSSIP_SENDER_MAIN, 10000);
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Reset your specialization", GOSSIP_SENDER_MAIN, 10002, "Do you really want to reset your specializations?", 0, false);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Reset your talents", GOSSIP_SENDER_MAIN, 10003);
            if (player->GetSpecsCount() < 2)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Learn Dual Talent Specialization", GOSSIP_SENDER_MAIN, 10004);

            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        }

        
        bool OnGossipHello(Player* player, Creature* creature)
        {
            MainMenu(player, creature);
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
			player->PlayerTalkClass->ClearMenus();
            switch(action)
            {
                case 10002:
                    //player->SetClassTrainerEntry(vendor);
                    //player->GetSession()->SendClassTrainerList(creature->GetGUID(), vendor);
                    player->ResetSpec();
                    break;
                case 10003:
                {
                    player->CLOSE_GOSSIP_MENU();
                    uint8 spec = 0;
                    player->GetSpecializationId(spec);
                    player->SendTalentWipeConfirm(creature->GetGUID(), spec);
                    break;
                }
                case 10004:
                    if (player->GetSpecsCount() == 1 && !(player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)))
                    {
                        // Cast spells that teach dual spec
                        // Both are also ImplicitTarget self and must be cast by player
                        player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
                        player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());

                        // Should show another Gossip text with "Congratulations..."
                        player->CLOSE_GOSSIP_MENU();
                    }
                    break;
            }
            return true;
        }
};

void AddSC_npc_globalclasstrainer()
{
    new npc_globalclasstrainer();
}