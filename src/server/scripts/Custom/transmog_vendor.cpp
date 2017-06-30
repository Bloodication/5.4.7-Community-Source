/*INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (555002, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24978, 0, 0, 0, 'Transmogrification Vendor', 'Dark-Portal', NULL, 0, 90, 90, 0, 0, 35, 35, 1, 0, 1, 1.14286, 1.14286, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 2048, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 12, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'npc_transmogvendor', 1);*/
#include "ScriptPCH.h"
#include "AccountMgr.h"
#include "Config.h"

enum Options
{
    MAIN_MENU,
    CHOOSE_ITEM,
    PURCHASE_ITEM,
    GOODBYE,
    MAIN_MENU_TEXT = 13371900 // Send a friendly gossip menu text
};

typedef std::set<uint32> DisabledItems;

class npc_transmogvendor : public CreatureScript
{
private:
    uint32 m_arenaCoins;
    DisabledItems m_disabledItems;

public:
    npc_transmogvendor() : CreatureScript("npc_transmogvendor")
    {
		QueryResult result = WorldDatabase.Query("SELECT * from restricted_tmog");
            do {
                if (uint32 entry = result->Fetch()[0].GetUInt32())
                    m_disabledItems.insert(entry);
            } while (result->NextRow());
    }

	bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();

        // Sanity checks - player in combat is not allowed to interact with the vendor
        if (player->isInCombat())
        {
            ChatHandler(player->GetSession()).SendSysMessage(LANG_YOU_IN_COMBAT);
            return true;
        }
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "I'd like to purchase an item for transmogrification.", 0, CHOOSE_ITEM, "", 0, true);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Goodbye.", 0, GOODBYE);
        player->PlayerTalkClass->SendGossipMenu(MAIN_MENU_TEXT, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();

        if (!uiAction)
            return OnGossipHello(player, creature);

        switch (uiAction)
        {
            case PURCHASE_ITEM:
            {
                AddItemToPlayer(player, uiSender);
                break;
            }
            case GOODBYE:
                return true;
            default:
                break;
        }

        return OnGossipHello(player, creature);
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, const char* code)
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();

        ChatHandler handler(player->GetSession());

        if(!uiAction)
            return OnGossipHello(player, creature);

        switch (uiAction)
        {
                       case CHOOSE_ITEM:
            {
                // Check if the input is a valid number
                uint32 entry = atoi(code);
                if (!entry)
                {
                    handler.SendSysMessage("You have entered an invalid item entry. Please try again with a valid entry.");
                    break;
                }

                // Check if the specified number corresponds to an item entry
                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(entry);
                if (!itemTemplate)
                {
                    handler.SendSysMessage("An item with the specified entry does not exist. Please try again with a valid entry.");
                    break;
                }

                // Check if the selected item meets the requirements
                if (!IsValidItemForTransmog(itemTemplate))
                {
                    handler.SendSysMessage("You may not purchase the specified item. Please try again with another item entry.");
                    break;
                }

                // If all the above checks pass send a confirmation to the user
                std::ostringstream os;
                os << "I'd like to purchase " << GetItemDesc(itemTemplate, false) << " please " << ".";

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, os.str(), entry, PURCHASE_ITEM);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Return to the main menu.", 0, MAIN_MENU);
                player->PlayerTalkClass->SendGossipMenu(MAIN_MENU_TEXT, creature->GetGUID());
                return true;
            }
            default:
                break;
        }

        return OnGossipHello(player, creature);
    }
	
    void AddItemToPlayer(Player* player, uint32 itemId)
    {
        ChatHandler handler(player->GetSession());

        // Sanity checks just to handle any unexpected surprises
        if (!itemId)
        {
            handler.SendSysMessage("An unexpected error has occurred. Please try again later.");
            return;
        }

        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);
        if (!itemTemplate)
        {
            handler.SendSysMessage("An unexpected error has occurred. Please try again later.");
            return;
        }
		
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1);
        if (msg == EQUIP_ERR_OK)
        {

            if (Item* item = player->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId)))
            {
                player->SendNewItem(item, 1, true, false);
                player->GetSession()->SendAreaTriggerMessage("You have successfully purchased\n%s.",
                                                             GetItemDesc(itemTemplate).c_str());
															 
                return;
            }
        }

        player->SendEquipError(msg, NULL, NULL);
        return;
    }

    std::string GetItemDesc(ItemTemplate const* itemTemplate, bool withQualityColor = true)
    {
        std::ostringstream os;
        os << "|Hitem:" << itemTemplate->ItemId << ":0:0:0:0:0:0:0:0:0|h";
        if (withQualityColor)
            os << "|c" << std::hex << ItemQualityColors[itemTemplate->Quality] << std::dec;
        else
            os  << "|cff363636";
        os << "[" << itemTemplate->Name1 << "]|r|h";

        return os.str();
    }

    bool IsValidItemForTransmog(ItemTemplate const* itemTemplate)
    {
        // Check disabled items first
        if (m_disabledItems.find(itemTemplate->ItemId) != m_disabledItems.end())
            return false;

        // Check item class/subclass
        switch (itemTemplate->Class)
        {
            case ITEM_CLASS_WEAPON:
            {
                switch (itemTemplate->SubClass)
                {
                    case ITEM_SUBCLASS_WEAPON_AXE:
                    case ITEM_SUBCLASS_WEAPON_AXE2:
                    case ITEM_SUBCLASS_WEAPON_BOW:
                    case ITEM_SUBCLASS_WEAPON_GUN:
                    case ITEM_SUBCLASS_WEAPON_MACE:
                    case ITEM_SUBCLASS_WEAPON_MACE2:
                    case ITEM_SUBCLASS_WEAPON_POLEARM:
                    case ITEM_SUBCLASS_WEAPON_SWORD:
                    case ITEM_SUBCLASS_WEAPON_SWORD2:
                    case ITEM_SUBCLASS_WEAPON_STAFF:
                    case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:
                    case ITEM_SUBCLASS_WEAPON_DAGGER:
                    case ITEM_SUBCLASS_WEAPON_THROWN:
                    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                        break;
                    default:
                        return false;
                }
                break;
            }
            case ITEM_CLASS_ARMOR:
            {
                switch (itemTemplate->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                    case ITEM_SUBCLASS_ARMOR_SHIELD:
                        break;
                    default:
                        return false;
                }
                break;
            }
            default:
                return false;
        }

        // Check inventory type
        switch (itemTemplate->InventoryType)
        {
            case INVTYPE_HEAD:
            case INVTYPE_SHOULDERS:
            case INVTYPE_BODY:
            case INVTYPE_CHEST:
            case INVTYPE_ROBE:
            case INVTYPE_WAIST:
            case INVTYPE_LEGS:
            case INVTYPE_FEET:
            case INVTYPE_WRISTS:
            case INVTYPE_HANDS:
            case INVTYPE_CLOAK:
            case INVTYPE_WEAPON:
            case INVTYPE_SHIELD:
            case INVTYPE_RANGED:
            case INVTYPE_2HWEAPON:
            case INVTYPE_WEAPONMAINHAND:
            case INVTYPE_WEAPONOFFHAND:
            case INVTYPE_THROWN:
            case INVTYPE_RANGEDRIGHT:
                break;
            default:
                return false;
        }

        // Item quality check
        switch (itemTemplate->Quality)
        {
            case ITEM_QUALITY_POOR:
            case ITEM_QUALITY_NORMAL:
            case ITEM_QUALITY_UNCOMMON:
            case ITEM_QUALITY_RARE:
            case ITEM_QUALITY_EPIC:
            case ITEM_QUALITY_HEIRLOOM:
                break;
            default:
                return false;
        }

        // Item level check
        if (itemTemplate->ItemLevel > 521)
            return false;

        return true;
    }

};

void AddSC_NPC_TransmogVendor()
{
    new npc_transmogvendor;
}