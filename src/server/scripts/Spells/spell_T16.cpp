/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
 
// Divine Protection - 498
class spell_item_paladin_t16_protection_2p : public SpellScriptLoader
{
    public:
        spell_item_paladin_t16_protection_2p() : SpellScriptLoader("spell_item_paladin_t16_protection_2p") { }

        class spell_item_paladin_t16_protection_2p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_paladin_t16_protection_2p_AuraScript);

        public:
            spell_item_paladin_t16_protection_2p_AuraScript()
            {
                amount = 0;
            }

            bool CheckProc(ProcEventInfo& /*eventInfo*/)
            {
                return GetTarget()->HasAura(138244);
            }

            void HandleProc(const AuraEffect* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                amount += eventInfo.GetDamageInfo()->GetDamage();
            }

            void Register()
            {
                DoCheckProc += AuraCheckProcFn(spell_item_paladin_t16_protection_2p_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_item_paladin_t16_protection_2p_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
            }

        private:
            uint32 amount;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_paladin_t16_protection_2p_AuraScript();
        }
};

// Item - Druid T16 Restoration 2P Bonus - 144869
// Rejuvenation - 774
class spell_item_druid_t16_restoration_2p : public SpellScriptLoader
{
    public:
        spell_item_druid_t16_restoration_2p() : SpellScriptLoader("spell_item_druid_t16_restoration_2p") { }

        class spell_item_druid_t16_restoration_2p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_druid_t16_restoration_2p_AuraScript);

            void OnTick(const AuraEffect* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(144869))
                        if (roll_chance_i(12))
                            caster->CastSpell(caster, 144871, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_druid_t16_restoration_2p_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_druid_t16_restoration_2p_AuraScript();
        }
};

// Item - Druid T16 Guardian 4P Bonus - 144887
class spell_item_druid_t16_guardian_4p : public SpellScriptLoader
{
    public:
        spell_item_druid_t16_guardian_4p() : SpellScriptLoader("spell_item_druid_t16_guardian_4p") { }

        class spell_item_druid_t16_guardian_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_druid_t16_guardian_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144887))
                    {
                        int32 amount = _player->GetTotalAttackPowerValue(BASE_ATTACK) * 0.0375f;
                        _player->CastCustomSpell(144888, SPELLVALUE_BASE_POINT0, amount, _player, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_druid_t16_guardian_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_druid_t16_guardian_4p_SpellScript();
        }
};

// Sudden Doom - 81340 | Killing Machine - 51124
class spell_item_death_knight_t16_dps_2p : public SpellScriptLoader
{
    public:
        spell_item_death_knight_t16_dps_2p() : SpellScriptLoader("spell_item_death_knight_t16_dps_2p") { }

        class spell_item_death_knight_t16_dps_2p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_death_knight_t16_dps_2p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(144899))
                        _player->CastSpell(_player, 144901, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_death_knight_t16_dps_2p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_death_knight_t16_dps_2p_SpellScript();
        }
};

// Dancing Rune Weapon - 49028
class spell_item_death_knight_t16_blood_4p : public SpellScriptLoader
{
    public:
        spell_item_death_knight_t16_blood_4p() : SpellScriptLoader("spell_item_death_knight_t16_blood_4p") { }

        class spell_item_death_knight_t16_blood_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_death_knight_t16_blood_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144950))
                        _player->CastSpell(_player, 144953, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_death_knight_t16_blood_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_death_knight_t16_blood_4p_SpellScript();
        }
};

// Unleash Elements - 73680
// Item - Shaman T16 Enhancement 2P Bonus - 144962
class spell_item_shaman_t16_enhancement_2p : public SpellScriptLoader
{
    public:
        spell_item_shaman_t16_enhancement_2p() : SpellScriptLoader("spell_item_shaman_t16_enhancement_2p") { }

        class spell_item_death_knight_t16_blood_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_death_knight_t16_blood_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144962))
                        _player->CastSpell(_player, 144964, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_death_knight_t16_blood_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_death_knight_t16_blood_4p_SpellScript();
        }
};

// Flame Shock - 8050
class spell_item_shaman_t16_enhancement_4p : public SpellScriptLoader
{
    public:
        spell_item_shaman_t16_enhancement_4p() : SpellScriptLoader("spell_item_shaman_t16_enhancement_4p") { }

        class spell_item_shaman_t16_enhancement_2p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_shaman_t16_enhancement_2p_AuraScript);

            void OnTick(const AuraEffect* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(144966))
                    {
                        if (roll_chance_i(5))
                        {
                            if (Aura* aura = caster->AddAura(77661, caster))
                                aura->SetStackAmount(5);

                            if (Player* player = caster->ToPlayer())
                                player->RemoveSpellCooldown(60103, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_shaman_t16_enhancement_2p_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_shaman_t16_enhancement_2p_AuraScript();
        }
};

// Spiritwalker's Grace - 79206
class spell_item_shaman_t16_restoration_4p : public SpellScriptLoader
{
    public:
        spell_item_shaman_t16_restoration_4p() : SpellScriptLoader("spell_item_shaman_t16_restoration_4p") { }

        class spell_item_death_knight_t16_blood_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_death_knight_t16_blood_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(145380))
                        _player->CastSpell(_player, 145385, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_death_knight_t16_blood_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_death_knight_t16_blood_4p_SpellScript();
        }
};

// Item - Rogue T16 4P Bonus - 145210
// Backstab - 53
class spell_item_rogue_t16_4p : public SpellScriptLoader
{
    public:
        spell_item_rogue_t16_4p() : SpellScriptLoader("spell_item_rogue_t16_4p") { }

        class spell_item_rogue_t16_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_rogue_t16_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(145210))
                        if (roll_chance_i(4))
                            _player->CastSpell(_player, 145211, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_rogue_t16_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_rogue_t16_4p_SpellScript();
        }
};

// Item - Hunter T16 2P Bonus - 144637
class spell_item_hunter_t16_2p : public SpellScriptLoader
{
    public:
        spell_item_hunter_t16_2p() : SpellScriptLoader("spell_item_hunter_t16_2p") { }

        class spell_item_hunter_t16_2p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_hunter_t16_2p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144637))
                        _player->ReduceSpellCooldown(3045, 4000);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_hunter_t16_2p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_hunter_t16_2p_SpellScript();
        }
};

// Item - Hunter T16 4P Bonus - 144641
// Aimed Shot! - 82928
class spell_item_hunter_t16_4p : public SpellScriptLoader
{
    public:
        spell_item_hunter_t16_4p() : SpellScriptLoader("spell_item_hunter_t16_4p") { }

        class spell_item_hunter_t16_4p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_hunter_t16_4p_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144641))
                        _player->CastSpell(_player, 144659, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_hunter_t16_4p_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_hunter_t16_4p_SpellScript();
        }
};

// Item - Death Knight T16 Blood 2P Bonus - 144934
class spell_item_death_knight_t16_blood_2p : public SpellScriptLoader
{
    public:
        spell_item_death_knight_t16_blood_2p() : SpellScriptLoader("spell_item_death_knight_t16_blood_2p") { }

        class spell_item_death_knight_t16_blood_2p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_death_knight_t16_blood_2p_AuraScript);

            uint8 count;

        public:
            spell_item_death_knight_t16_blood_2p_AuraScript()
            {
                count = 0;
            }

            void HandleProc(const AuraEffect* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                if (Unit* target = GetTarget())
                {
                    if (Player* player = target->ToPlayer())
                    {
                        if (eventInfo.GetSpellInfo())
                        {
                            count++;

                            if (count >= 10)
                            {
                                target->CastSpell(target, 144948, true);
                                count = 0;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_item_death_knight_t16_blood_2p_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_death_knight_t16_blood_2p_AuraScript();
        }
};

void AddSC_t16_spell_scripts()
{
    new spell_item_paladin_t16_protection_2p();
    new spell_item_druid_t16_restoration_2p();
    new spell_item_druid_t16_guardian_4p();
    new spell_item_death_knight_t16_dps_2p();
    new spell_item_death_knight_t16_blood_2p();
    new spell_item_death_knight_t16_blood_4p();
    new spell_item_shaman_t16_enhancement_2p();
    new spell_item_shaman_t16_enhancement_4p();
    new spell_item_shaman_t16_restoration_4p();
    new spell_item_rogue_t16_4p();
    new spell_item_hunter_t16_2p();
    new spell_item_hunter_t16_4p();
}