    #include "ScriptPCH.h"
     
    class Morph_npc : public CreatureScript
    {
    public:
        Morph_npc() : CreatureScript("Morph_npc") { }
     
        bool OnGossipHello(Player* player, Creature* creature)
        {
                    if (player->IsInCombat())
                    {
                        player->GetSession()->SendNotification("You are in combat!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

					if (player->IsMounted())
					{
						player->GetSession()->SendNotification("You are mounted!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
					}

					if (player->IsInFeralForm())
					{
						player->GetSession()->SendNotification("You are in Feral Form! Shapeshift back to normal to Morph!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
		            }

					// Swift flight form, flight form, Travel form, Bear form, 
					if (player->HasAura(40120) || player->HasAura(33943) || player->HasAura(783) || player->HasAura(5487) || player->HasAura(24858) || player->HasAura(33891))
                   {
						player->GetSession()->SendNotification("You are shapeshifted! Shapeshift back to normal to Morph!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
		            }
                                    player->ADD_GOSSIP_ITEM(6, "Human Male", GOSSIP_SENDER_MAIN, 1);
                                    player->ADD_GOSSIP_ITEM(6, "Human Female", GOSSIP_SENDER_MAIN, 2);
                                    player->ADD_GOSSIP_ITEM(6, "Gnome Male", GOSSIP_SENDER_MAIN, 3);
                                    player->ADD_GOSSIP_ITEM(6, "Gnome Female", GOSSIP_SENDER_MAIN, 4);
                                    player->ADD_GOSSIP_ITEM(6, "Blood Elf Male", GOSSIP_SENDER_MAIN, 6);
									player->ADD_GOSSIP_ITEM(6, "Blood Elf Female", GOSSIP_SENDER_MAIN, 7);
									player->ADD_GOSSIP_ITEM(6, "Tauren Male", GOSSIP_SENDER_MAIN, 8);
									player->ADD_GOSSIP_ITEM(6, "Tauren Female", GOSSIP_SENDER_MAIN, 9);
									player->ADD_GOSSIP_ITEM(6, "Dwarf Male", GOSSIP_SENDER_MAIN, 10);
									player->ADD_GOSSIP_ITEM(6, "Dwarf Female", GOSSIP_SENDER_MAIN, 11);
									player->ADD_GOSSIP_ITEM(6, "Night Elf Male", GOSSIP_SENDER_MAIN, 12);
									player->ADD_GOSSIP_ITEM(6, "Night Elf Female", GOSSIP_SENDER_MAIN, 13);
									player->ADD_GOSSIP_ITEM(6, "Troll Male", GOSSIP_SENDER_MAIN, 14);
									player->ADD_GOSSIP_ITEM(6, "Troll Female", GOSSIP_SENDER_MAIN, 15);
									player->ADD_GOSSIP_ITEM(6, "Goblin Male", GOSSIP_SENDER_MAIN, 16);
									player->ADD_GOSSIP_ITEM(6, "Goblin Female", GOSSIP_SENDER_MAIN, 17);
									player->ADD_GOSSIP_ITEM(6, "Undead Male", GOSSIP_SENDER_MAIN, 18);
									player->ADD_GOSSIP_ITEM(6, "Undead Female", GOSSIP_SENDER_MAIN, 19);
									player->ADD_GOSSIP_ITEM(6, "Orc Male", GOSSIP_SENDER_MAIN, 20);
									player->ADD_GOSSIP_ITEM(6, "Orc Female", GOSSIP_SENDER_MAIN, 21);
									player->ADD_GOSSIP_ITEM(6, "Draenei Male", GOSSIP_SENDER_MAIN, 22);
									player->ADD_GOSSIP_ITEM(6, "Draenei Female", GOSSIP_SENDER_MAIN, 23);

                                    player->ADD_GOSSIP_ITEM(6, "Demorph My Character", GOSSIP_SENDER_MAIN, 24);

                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                    return true;
            }
           
            bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
            {
				if (player->IsMounted())
					{
						player->GetSession()->SendNotification("You are mounted!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
					}

					if (player->IsInFeralForm())
					{
						player->GetSession()->SendNotification("You are in Feral Form! Shapeshift back to normal to Morph!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
		            }

					// Swift flight form, flight form, Travel form, Bear form, 
					if (player->HasAura(40120) || player->HasAura(33943) || player->HasAura(783) || player->HasAura(5487) || player->HasAura(24858) || player->HasAura(33891))
                   {
						player->GetSession()->SendNotification("You are shapeshifted! Shapeshift back to normal to Morph!");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
		            }
     
                    switch (action)
            {
                case 1:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37925);
                    break;
                case 2:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37926);
                    break;
                case 3:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37913);
                    break;
                case 4:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20581);
                    break;

			    case 6:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20578);
                    break;
			    case 7:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20579);
                    break;
			    case 8:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20585);
                    break;
				case 9:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20584);
                    break;
				case 10:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20317);
                    break;
				case 11:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37918);
                    break;
				case 12:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20318);
                    break;
			    case 13:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37919);
                    break;
				case 14:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20321);
                    break;
				case 15:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37922);
                    break;
				case 16:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20582);
                    break;
			    case 17:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20583);
                    break;
				case 18:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37923);
                    break;
				case 19:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37924);
                    break;
				case 20:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(37920);
                    break;
				case 21:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20316);
                    break;
				case 22:
					player->CLOSE_GOSSIP_MENU();
					player->SetDisplayId(37916);
					break;
				case 23:
                    player->CLOSE_GOSSIP_MENU();
                    player->SetDisplayId(20323);
                    break;
                case 24:
                    player->CLOSE_GOSSIP_MENU();
                    player->DeMorph();
                    break;
                    }
                    return true;
            }
    };
     
    void AddSC_Morph_npc()
    {
            new Morph_npc();
    }