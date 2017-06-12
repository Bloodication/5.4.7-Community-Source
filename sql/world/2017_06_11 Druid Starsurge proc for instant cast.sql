-- Druid Fixed starsurge not removing proc on cast
Delete From spell_linked_spell where spell_trigger = 78674 and spell_effect = -93400;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (78674, -93400, 0, 'Starsurge now removes Instant Starsurge proc Instantly');