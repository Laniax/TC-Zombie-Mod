/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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

/* Explanation
<TODO>
*/

#include "ScriptPCH.h"

enum ZombieSpells
{
    // Zombie
    SPELL_ATTACK_1H         = 42880,
    SPELL_ATTACK_2H         = 42904,
    SPELL_EMERGE_VISUAL     = 50142,
    SPELL_ZOMBIE_DIES       = 61454,

    // Kill counter
    SPELL_COUNTER           = 18282,

    // Boom bot, Mine
    SPELL_EXPLODE           =  7670, // Used by Bomb Bot
    SPELL_EXPLODE_2         = 71495, // Used by Proximity Mine - triggers knockback (40191)

    // Spawnpoint
    SPELL_KILL_SPAWNPOINT   = 60861,

    // Zombie Turret
    SPELL_RAPID_FIRE        = 71250,

    // Unused
    SPELL_FLAME_BREATH      = 49161,
};

enum ZombieGameobjects
{
    GAMEOBJECT_BARRICADE    = 293610,
    GAMEOBJECT_ANVIL        = 293611,
    GAMEOBJECT_SKELETON     = 185437,
    GAMEOBJECT_POWER_UP     = 0,
};

enum ZombieCreatures
{
    NPC_ZOMBIE              = 792130,
    NPC_COUNTER             = 792131,
    NPC_SPAWNER             = 792132,
    NPC_SPAWNPOINT          = 792133,
    NPC_BARRICADE_TRIGGER   = 792134,
    NPC_TURRET_P1           = 792135,
    NPC_TURRET_P2           = 792136,
    NPC_BOMB_BOT            = 792137,
    NPC_COCKROACH           = 792138,
};

enum ZombieEvents
{
    // Zombie Turret
    EVENT_FIRE_CANNON      = 1,
};

uint32 const MAX_SPAWNPOINTS = 8;
Position const ZombieSpawnPoints[MAX_SPAWNPOINTS] =
{
    {136.054184f, 201.700638f, 95.039246f, 4.680336f},
    {156.154419f, 201.463287f, 98.452942f, 4.246449f},
    {121.992653f, 200.666763f, 98.161942f, 4.664613f},
    {107.410492f, 199.340973f, 96.184494f, 4.821693f},
    {107.863098f, 133.579697f, 97.949394f, 1.482956f},
    {123.680313f, 132.347824f, 97.240753f, 1.570144f},
    {138.114655f, 132.511612f, 97.275185f, 1.542667f},
    {158.301880f, 134.723984f, 99.867416f, 1.750798f},
};

class npc_zombie_barricade_trigger : public CreatureScript
{
    public:
        npc_zombie_barricade_trigger() : CreatureScript("npc_zombie_barricade_trigger") { }

        struct npc_zombie_barricade_triggerAI : public ScriptedAI
        {
            npc_zombie_barricade_triggerAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            uint8 hitCounter;

            void Reset()
            {
                hitCounter = 0;
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (caster->GetTypeId() == TYPEID_UNIT && (spell->Id == SPELL_ATTACK_1H) || (spell->Id == SPELL_ATTACK_2H))
                {
                    ++hitCounter;

                    if (hitCounter == 12)
                    {
                        if (GameObject* barricade = me->FindNearestGameObject(GAMEOBJECT_BARRICADE, 5.0f))
                        {
                            barricade->Delete();
                            me->ForcedDespawn();
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_barricade_triggerAI(creature);
        }
};

class npc_zombie_kill_counter : public CreatureScript
{
    public:
        npc_zombie_kill_counter() : CreatureScript("npc_zombie_kill_counter") { }

        struct npc_zombie_kill_counterAI : public ScriptedAI
        {
            npc_zombie_kill_counterAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            uint8 killCounter;

            void Reset()
            {
                killCounter = 0;
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (caster->GetTypeId() == TYPEID_UNIT && spell->Id == SPELL_COUNTER)
                {
                    ++killCounter;

                    // Phase 1 - We summon a few turrets which shoot on randomly picked out zombies
                    // These turrets are not eligible for blowing up spawnpoints, those will be spawned in P2
                    if (killCounter == 50)
                    {
                        //me->SummonCreature(NPC_TURRET_P1, x, y, z, o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                        //me->SummonCreature(NPC_TURRET_P1, x, y, z, o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                        //me->SummonCreature(NPC_TURRET_P1, x, y, z, o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    }

                    // Phase 2
                    if (killCounter == 100)
                    {
                        // Despawn the turrets from P1
                        std::list<Creature*> TurretList1;
                        GetCreatureListWithEntryInGrid(TurretList1, me, NPC_TURRET_P1, 100.0f);
                        for (std::list<Creature*>::iterator itr = TurretList1.begin(); itr != TurretList1.end(); ++itr)
                            (*itr)->ForcedDespawn();

                        //me->SummonCreature(NPC_TURRET_P2, x, y, z, o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    }
                    
                    // Phase 3
                    if (killCounter == 150)
                    {
                        // Despawn the turret from P2
                        if (Creature* turretP2 = me->FindNearestCreature(NPC_TURRET_P2, 50.0f, true))
                            turretP2->ForcedDespawn();
                    }

                    // Phase 4
                    if (killCounter == 200)
                    {

                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_kill_counterAI(creature);
        }
};

class npc_zombie : public CreatureScript
{
    public:
        npc_zombie() : CreatureScript("npc_zombie") { }

        struct npc_zombieAI : public ScriptedAI
        {
            npc_zombieAI(Creature* creature) : ScriptedAI(creature)
            {
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
            }

            uint32 attackBarricadeTimer;

            bool atBarricade;

            void Reset()
            {
                attackBarricadeTimer = 3500;

                me->SetReactState(REACT_PASSIVE); // Not putting this up there so we can cancel it when a bracket is down
            }

            void JustDied(Unit* /*killer*/)
            {
                if (Creature* counter = me->FindNearestCreature(NPC_COUNTER, 50.0f, true))
                    DoCast(counter, SPELL_COUNTER);

                DoCast(me, SPELL_ZOMBIE_DIES, true);
                me->SummonGameObject(GAMEOBJECT_SKELETON, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 60000); // One minute

                if (urand(0, 9) == 0) // 10% chance
                    DoSpawnPowerUp();

                me->ForcedDespawn(2000); // We want all bodies to be removed when we die, but we also want the visual to be there
            }

            void DoSpawnPowerUp()
            {
                switch (urand(0, 10))
                {
                    case 0:
                        me->SummonGameObject(GAMEOBJECT_POWER_UP, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 120000);
                    case 1:
                        // Kill all zombies which are currently spawned
                        std::list<Creature*> ZombieList;
                        GetCreatureListWithEntryInGrid(ZombieList, me, me->GetEntry(), 100.0f);
                        for (std::list<Creature*>::iterator itr = ZombieList.begin(); itr != ZombieList.end(); ++itr)
                            (*itr)->Kill(*itr);
                }
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (me->FindNearestGameObject(GAMEOBJECT_BARRICADE, 50.0f))
                    if (Creature* barricadeTrigger = me->FindNearestCreature(NPC_BARRICADE_TRIGGER, 100.0f, true))
                        me->GetMotionMaster()->MovePoint(1, barricadeTrigger->GetPositionX()+3, barricadeTrigger->GetPositionY(), barricadeTrigger->GetPositionZ());
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (caster->GetTypeId() != TYPEID_UNIT)
                    return;

                // Die when a Bomb Bot or Proximity Mine explodes near us
                if (spell->Id == SPELL_EXPLODE || spell->Id == SPELL_EXPLODE_2)
                    me->Kill(me);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE && id == 1)
                    atBarricade = true;
            }

            void UpdateAI(uint32 const diff)
            {
                if (atBarricade)
                {
                    if (attackBarricadeTimer <= diff)
                        DoCast(RAND(SPELL_ATTACK_1H, SPELL_ATTACK_2H));
                    else
                        attackBarricadeTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombieAI(creature);
        }
};

class npc_zombie_spawner : public CreatureScript
{
    public:
        npc_zombie_spawner() : CreatureScript("npc_zombie_spawner") { }
 
        struct npc_zombie_spawnerAI : public ScriptedAI
        {
            npc_zombie_spawnerAI(Creature* creature) : ScriptedAI(creature)
            {
                for (uint8 i = 0; i < MAX_SPAWNPOINTS; i++)
                    canSpawn[i] = true;
            }
 
            uint32 summonTimer;
            bool canSpawn[MAX_SPAWNPOINTS];
 
            void Reset()
            {
                summonTimer = 15000;
            }
 
            void StopSpawning(uint8 spawnPoint)
            {
                canSpawn[spawnPoint] = false;
            }
 
            void DoSpawnZombies()
            {
                for (uint8 i = 0; i < MAX_SPAWNPOINTS; i++)
                {
                    if (!canSpawn[i])
                        continue;
 
                    float x = ZombieSpawnPoints[i].GetPositionX() + irand(-4, 4);
                    float y = ZombieSpawnPoints[i].GetPositionY() + irand(-4, 4);
                    float z = ZombieSpawnPoints[i].GetPositionZ();
                    float ground_Z = me->GetMap()->GetHeight(x, y, z, true, MAX_FALL_DISTANCE);
                    if (fabs(ground_Z - z) < 0.1f)
                        z = ground_Z;
 
                    if (Creature* zombie = me->SummonCreature(NPC_ZOMBIE, x, y, z, ZombieSpawnPoints[i].GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                        zombie->CastSpell(zombie, SPELL_EMERGE_VISUAL, false);
                }
            }
 
            void UpdateAI(uint32 const diff)
            {
                if (summonTimer <= diff)
                {
                    DoSpawnZombies();
                    summonTimer = 15000;
                }
                else
                    summonTimer -= diff;
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_spawnerAI(creature);
        }
};

typedef npc_zombie_spawner::npc_zombie_spawnerAI SpawnerAI;

class npc_zombie_spawnpoint : public CreatureScript
{
    public:
        npc_zombie_spawnpoint() : CreatureScript("npc_zombie_spawnpoint") { }

        struct npc_zombie_spawnpointAI : public ScriptedAI
        {
            npc_zombie_spawnpointAI(Creature* creature) : ScriptedAI(creature) { }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (caster->GetTypeId() != TYPEID_UNIT)
                    return;

                if (spell->Id == SPELL_KILL_SPAWNPOINT)
                {
                    for (uint8 i = 0; i < MAX_SPAWNPOINTS; i++)
                    {
                        if (floor(ZombieSpawnPoints[i].GetPositionX()) == floor(me->GetPositionX())
                            && floor(ZombieSpawnPoints[i].GetPositionY()) == floor(me->GetPositionY()))
                        {
                            if (Creature* spawner = me->FindNearestCreature(NPC_SPAWNER, 200.0f, true))
                                if (SpawnerAI* spawnerAI = CAST_AI(SpawnerAI, spawner->AI()))
                                    spawnerAI->StopSpawning(i);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_spawnpointAI(creature);
        }
};

class npc_proximity_mine : public CreatureScript
{
    public:
        npc_proximity_mine() : CreatureScript("npc_proximity_mine") { }

        struct npc_proximity_mineAI : public ScriptedAI
        {
            npc_proximity_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!who || who->GetEntry() != NPC_ZOMBIE)
                    return;

                if (me->GetMapId() == 289 && me->IsWithinDist(who, 1.5f))
                {
                    DoCast(who, SPELL_EXPLODE_2, true);
                    me->ForcedDespawn();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_proximity_mineAI(creature);
        }
};

class npc_bomb_bot : public CreatureScript
{
    public:
        npc_bomb_bot() : CreatureScript("npc_bomb_bot") { }

        struct npc_bomb_botAI : public ScriptedAI
        {
            npc_bomb_botAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 explodeTimer;

            bool isFollowing;

            void Reset()
            {
                explodeTimer = 10000;
                isFollowing = false;

                // Or do we want to make it move random and then explode?
                if (Creature* zombie = me->FindNearestCreature(NPC_ZOMBIE, 30.0f, true))
                {
                    me->GetMotionMaster()->MoveFollow(zombie, 1.0f, 1.0f);
                    isFollowing = true;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (isFollowing)
                {
                    if (explodeTimer <= diff)
                    {
                        DoCast(SPELL_EXPLODE);
                        me->ForcedDespawn();
                    }
                    else
                        explodeTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bomb_botAI(creature);
        }
};

class npc_zombie_turret : public CreatureScript
{
    public:
        npc_zombie_turret() : CreatureScript("npc_zombie_turret") { }

        struct npc_zombie_turretAI : public Scripted_NoMovementAI
        {
            npc_zombie_turretAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                _events.Reset();
                _events.ScheduleEvent(EVENT_FIRE_CANNON, 1000);
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_FIRE_CANNON)
                {
                    me->CastSpell((Unit*)NULL, SPELL_RAPID_FIRE, false);
                    _events.ScheduleEvent(EVENT_FIRE_CANNON, 5000);
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_turretAI(creature);
        }
};

class spell_repair_channel : public SpellScriptLoader
{
    public:
        spell_repair_channel() : SpellScriptLoader("spell_repair_channel") { }

        class spell_repair_channel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_repair_channel_AuraScript);

            void HandleOnEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* player = GetCaster()->ToPlayer();

                if (!player)
                    return;

                if (GameObject* barricade = player->FindNearestGameObject(GAMEOBJECT_BARRICADE, 100.0f))
                    barricade->SetRespawnTime(0);

                if (GameObject* anvil = player->FindNearestGameObject(GAMEOBJECT_ANVIL, 100.0f))
                    anvil->DeleteFromDB();

                if (Creature* barricadeTrigger = player->FindNearestCreature(NPC_BARRICADE_TRIGGER, 100.0f, false))
                    barricadeTrigger->Respawn(true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_repair_channel_AuraScript::HandleOnEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_repair_channel_AuraScript();
        }
};

class ZombieEntryCheck
{
    public:
        ZombieEntryCheck() {}

        bool operator()(Unit* unit) const
        {
            if (unit->GetTypeId() == TYPEID_PLAYER)
                return true;

            if (!unit->isAlive())
                return true;

            return unit->GetEntry() != NPC_ZOMBIE;
        }
};

class spell_zombie_rapid_fire : public SpellScriptLoader
{
    public:
        spell_zombie_rapid_fire() : SpellScriptLoader("spell_zombie_rapid_fire") { }

        class spell_zombie_rapid_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_zombie_rapid_fire_SpellScript);

            void SelectTarget(std::list<Unit*>& unitList)
            {
                if (unitList.empty())
                    return;

                unitList.remove_if(ZombieEntryCheck());
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_zombie_rapid_fire_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_CONE_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_zombie_rapid_fire_SpellScript();
        }
};

/*
enum Actions
{
    ACTION_TELEPORT_GROUP = 1;
};

class custom_mr_asshat : public CreatureScript
{
    public:
        custom_mr_asshat() : CreatureScript("custom_mr_asshat") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            Group* group = player->GetGroup();

            if (!group)
                return false;

            if (group->IsLeader(player->GetGUID()) && !group->isLFGGroup())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, GOSSIP_ITEM_PARTICIPATE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();

            Group* group = player->GetGroup();

            // If the player disbandon/leaves group during OnGossipHello
            if (!group || HasRecentlyReadyCheck)
                return false;

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                {
                    if (Player* member = itr->getSource())
                    {
                        WorldPacket data(MSG_RAID_READY_CHECK, 8);
                        data << player->GetGUID();
                        member->GetSession()->SendPacket(&data);
                    }
                }
                HasRecentlyReadyCheck = true;
                ReadyCheckTimer = 30000;
            }

            return true;
        }

        struct custom_mr_asshatAI : public ScriptedAI // to be able to access DoTeleportAll
        {
            custom_mr_asshatAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                ReadyCheckTimer = 0;
                HasRecentlyReadyCheck = false;
            }

            void DoAction(int32 const action)
            {
                if (action != ACTION_TELEPORT_GROUP || !HasRecentlyReadyCheck)
                    return;

                DoTeleportAll(x, y, z, o);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!HasRecentlyReadyCheck)
                    return;

                // Reset the boolean if the 30 sec timer passed
                if (ReadyCheckTimer <= diff)
                    HasRecentlyReadyCheck = false;
                else
                    ReadyCheckTimer -= diff;
            }
        };

        bool HasRecentlyReadyCheck;
        uint32 ReadyCheckTimer;

        CreatureAI* GetAI(Creature* creature) const
        {
            return new custom_mr_asshatAI(creature);
        }
};

class custom_serverscript : public ServerScript
{
    public:
        custom_serverscript() : ServerScript("woho") { }

        void OnPacketReceive(WorldSocket* socket, WorldPacket& packet)
        {
            if (GetOpcode() == MSG_RAID_READY_CHECK_FINISHED)
            {
                Player* player = socket->GetSession()->GetPlayer();

                // Check to make it faster
                if (!player || player->GetMapId() != 289)
                    return;

                InstanceScript* instance = player->GetInstanceScript();

                if (!instance)
                    return;

                if (Creature* creature = sObjectAccessor->GetCreature(*player, instance->GetData64(DATA_MR_ASSHAT))) // DATA_MR_ASSHAT returns the guid of mr asshat
                    creature->GetAI()->DoAction(ACTION_TELEPORT_GROUP);
            }
        }
};

void AddSC_()
{
    new custom_mr_asshat();
}
*/

void AddSC_Zombie_event()
{
    new npc_zombie_barricade_trigger();
    new npc_zombie();
    new npc_bomb_bot();
    new npc_zombie_kill_counter();
    new npc_zombie_spawner();
    new npc_zombie_spawnpoint();
    new npc_proximity_mine();
    new npc_zombie_turret();
    new spell_repair_channel();
    new spell_zombie_rapid_fire();
}
