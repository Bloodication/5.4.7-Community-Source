-- [Spell/DB][Spellsteal] now dispells [Alter Time]
DELETE FROM spell_linked_spell where spell_trigger = 30449 and spell_effect = -110909;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (30449, -110909, 1, 'SpellSteal Dispells Alter Time');

-- [DB/Spell] Mage Spellsteal now Dispells Earth Shield
DELETE FROM spell_linked_spell where spell_trigger = 30449 and spell_effect = -974;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (30449, -974, 1, 'SpellSteal Dispells Earth Shield');