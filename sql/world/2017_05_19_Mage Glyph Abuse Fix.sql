-- Remove Molten Armor Aura to get rid of Glyph Of Armor Buff
Delete From spell_linked_spell where spell_trigger = 56546 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 63864 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 56547 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 56600 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 56602 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 56592 AND spell_effect = -30482;
Delete From spell_linked_spell where spell_trigger = 62127 AND spell_effect = -30482;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (56546, -30482, 0, '[Glyph Of Blink] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (63864, -30482, 0, '[Glyph Of Deep Freeze] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (56547, -30482, 0, '[Glyph Of Evocation] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (56600, -30482, 0, '[Glyph Of Polymorph] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (56602, -30482, 0, '[Glyph Of Spellsteal] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (56592, -30482, 0, '[Glyph Of Ice BLock] Dispell [Molten Armor]');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (62127, -30482, 0, '[Glyph Of Remove Curse] Dispell [Molten Armor]');

-- Condtions to FIX MAGE Glyph Abuse
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 56546;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 63864;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 56547;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 56600;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 56602;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 56592;
Delete From conditions where SourceTypeOrReferenceId = 17 AND SourceEntry = 62127;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 56546, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 56547, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 56592, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 56600, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 62127, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 63864, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (17, 0, 56602, 0, 0, 1, 0, 110909, 0, 0, 1, 0, '', 'Can\'t use Glyph While Alter Time is casted');