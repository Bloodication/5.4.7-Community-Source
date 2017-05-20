-- Rogue [Burst of Speed] Spell now adds Aura and increases speed by 70%
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (108212, 137573, 0, '[Burst of Speed] Add Aura');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (-108212, -137573, 2, '[Burst of Speed] Remove Aura');

-- Warrior [Storm Bolt] Fix for 4sec Stun
Delete from spell_linked_spell where spell_trigger = 107570 and spell_effect = 132169;

-- Druid [Wrath] Removes [Soul of the Forest]
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (5176, -114108, 0, '[Wrath] Removes [Soul of the Forest]');
