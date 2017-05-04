#include "ScriptPCH.h"
#include "Language.h"

class proff_npc : public CreatureScript

{
public:

	proff_npc() : CreatureScript("proff_npc") {}

	struct proff_npcAI : public ScriptedAI
	{
		proff_npcAI(Creature *c) : ScriptedAI(c)
		{
		}
	};

	CreatureAI* GetAI(Creature* _creature) const
	{
		return new proff_npcAI(_creature);
	}

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Alchemy]", GOSSIP_SENDER_MAIN, 1, "Do you really want to learn [Alchemy]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Blacksmithing]", GOSSIP_SENDER_MAIN, 2, "Do you really want to learn [Blacksmithing]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Leatherworking]", GOSSIP_SENDER_MAIN, 3, "Do you really want to learn [Leatherworking]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Tailoring]", GOSSIP_SENDER_MAIN, 4, "Do you really want to learn [Tailoring]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Engineering]", GOSSIP_SENDER_MAIN, 5, "Do you really want to learn [Engineering]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Enchanting]", GOSSIP_SENDER_MAIN, 6, "Do you really want to learn [Enchanting]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Jewelcrafting]", GOSSIP_SENDER_MAIN, 7, "Do you really want to learn [Jewelcrafting]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Inscription]", GOSSIP_SENDER_MAIN, 8, "Do you really want to learn [Inscription]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Mining]", GOSSIP_SENDER_MAIN, 11, "Do you really want to learn [Mining]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Herbalism]", GOSSIP_SENDER_MAIN, 12, "Do you really want to learn [Herbalism]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, "[Skinning]", GOSSIP_SENDER_MAIN, 13, "Do you really want to learn [Skinning]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(1, "[Cooking]", GOSSIP_SENDER_MAIN, 9, "Do you really want to learn [Cooking]?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(1, "[First Aid]", GOSSIP_SENDER_MAIN, 10, "Do you really want to learn [First Aid]?", 0, false);
		pPlayer->PlayerTalkClass->SendGossipMenu(pCreature->GetEntry(), pCreature->GetGUID());
		return true;
	}

	bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
	{
		uint32 skillCount = 0;

		if (pPlayer->HasSkill(SKILL_MINING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_SKINNING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_HERBALISM))
			skillCount++;

		if (skillCount >= 2)
			return true;

		for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
		{
			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
			if (!SkillInfo)
				continue;

			if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
				continue;

			if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
				continue;

			const uint32 skillID = SkillInfo->id;
			if (pPlayer->HasSkill(skillID))
				skillCount++;

			if (skillCount >= 2)
				return true;
		}
		return false;
	}

	bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
	{
		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
		std::string skill_name = SkillInfo->name;
		if (!SkillInfo)
			return false;

		LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
		pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 600, 600);
		if (skill_name.empty())
			return false;

		ChatHandler(pPlayer).PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name.c_str());
		return true;
	}

	void LearnSkillRecipesHelper(Player *pPlayer, uint32 skill_id)
	{
		uint32 classmask = pPlayer->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
		{
			SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
			if (!skillLine)
				continue;

			// wrong skill
			if (skillLine->SkillLine != skill_id)
				continue;
			if (skillLine->Spell == 110955) // skip this spell.. idk why its added
				continue;
			// skip racial skills
			if (skillLine->RaceMask != 0)
				continue;
			// skip wrong class skills
			if (skillLine->ClassMask && (skillLine->ClassMask & classmask) == 0)
				continue;

			SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->Spell);
			pPlayer->learnSpell(skillLine->Spell, false);
		}
	}

	bool IsSecondarySkill(SkillType skill) const
	{
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
	}

	void CompleteLearnProfession(Player *pPlayer, SkillType skill)
	{
		if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
			pPlayer->GetSession()->SendNotification("You already know two primary professions!");
		else
		{
			if (!LearnAllRecipesInProfession(pPlayer, skill))
				pPlayer->GetSession()->SendNotification("Error #1: Internal code failiure");
		}
	}

	bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus();

		switch (uiAction)
		{
		case 1: CompleteLearnProfession(pPlayer, SKILL_ALCHEMY); break;
		case 2: CompleteLearnProfession(pPlayer, SKILL_BLACKSMITHING); break;
		case 3: CompleteLearnProfession(pPlayer, SKILL_LEATHERWORKING); break;
		case 4: CompleteLearnProfession(pPlayer, SKILL_TAILORING); break;
		case 5: CompleteLearnProfession(pPlayer, SKILL_ENGINEERING); break;
		case 6: CompleteLearnProfession(pPlayer, SKILL_ENCHANTING); break;
		case 7: CompleteLearnProfession(pPlayer, SKILL_JEWELCRAFTING); break;
		case 8: CompleteLearnProfession(pPlayer, SKILL_INSCRIPTION); break;
		case 9: CompleteLearnProfession(pPlayer, SKILL_COOKING); break;
		case 10: CompleteLearnProfession(pPlayer, SKILL_FIRST_AID); break;
		case 11: CompleteLearnProfession(pPlayer, SKILL_MINING); break;
		case 12: CompleteLearnProfession(pPlayer, SKILL_HERBALISM); break;
		case 13: CompleteLearnProfession(pPlayer, SKILL_SKINNING); break;
		}

		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
	}
};

void AddSC_proff_npc()
{
	new proff_npc();
}