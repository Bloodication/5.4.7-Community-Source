#ifndef ARENA_1V1_H
#define ARENA_1V1_H

// TalentTab.dbc -> TalentTabID
const uint32 FORBIDDEN_TALENTS_IN_1V1_ARENA[] =
{
    // Healer
	270,
	264,
	256,
	257,
	105,
	 65,


    // Tanks
    //383, // PaladinProtection
    //163, // WarriorProtection

    0 // End
};


// Return false, if player have invested more than 35 talentpoints in a forbidden talenttree.
static bool Arena1v1CheckTalents(Player* player)
{
    if(!player)
        return false;

	for (uint32 i = 0; i < 7; ++i)
	{
		if (player->GetActiveSpec() == FORBIDDEN_TALENTS_IN_1V1_ARENA[i])
		{
			player->GetSession()->SendAreaTriggerMessage("Healers cannot join for 1v1");
		}

	}
}

#endif