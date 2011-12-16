enum ZombieInvasion
{
    SPELL_ATTACK_1H         = 42880,
    SPELL_ATTACK_2H         = 42904,
    SPELL_EMERGE_VISUAL     = 50142,
    SPELL_ZOMBIE_DIES       = 61454,
    SPELL_COUNTER           = 18282,
    SPELL_EXPLODE           =  7670, // Used by Bomb Bot
    SPELL_EXPLODE_2         = 71495, // Used by Proximity Mine - triggers knockback (40191)

    GAMEOBJECT_BARRICADE    = 293610,
    GAMEOBJECT_ANVIL        = 293611,
    GAMEOBJECT_SKELETON     = 185437,
    GAMEOBJECT_POWER_UP     = 0,

    NPC_ZOMBIE              = 792130,
    NPC_COUNTER             = 792131,
    NPC_SPAWNER             = 792132,
    NPC_BARRICADE_TRIGGER   = 792133,
    NPC_TURRET_P1           = 792134,
    NPC_TURRET_P2           = 792135,
    NPC_BOMB_BOT            = 792136,
    NPC_PROXIMITY_MINE      = 792137,
    NPC_COCKROACH           = 792138,
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

                DoCast(SPELL_EMERGE_VISUAL);
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

                me->ForcedDespawn(1000); // We want all bodies to be removed when we die, but we also want the visual to be there
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
                    if (attackBarricadeTimer <= diff)
                        DoCast(RAND(SPELL_ATTACK_1H, SPELL_ATTACK_2H));
                    else
                        attackBarricadeTimer -= diff;
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
            npc_zombie_spawnerAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 summonTimer;

            bool pointOne;
            bool pointTwo;
            bool pointThree;
            bool pointFour;
            bool pointFive;

            void Reset()
            {
                summonTimer = 15000;
            }

            void UpdateAI(uint32 const diff)
            {
                if (summonTimer <= diff)
                    me->SummonCreature(NPC_ZOMBIE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0);
                else
                    summonTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zombie_spawnerAI(creature);
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
                //if (GameObject* barricade = ObjectAccessor::GetGameobject(*GetCaster(), InstanceScript->GetData64(DATA_BARRIER)))
                {
                    sLog->outString("first");
                    if (GameObject* anvil = player->FindNearestGameObject(GAMEOBJECT_ANVIL, 100.0f))
                    //if (GameObject* anvil = ObjectAccessor::GetGameobject(*GetCaster(), InstanceScript->GetData64(DATA_ANVIL)))
                    {
                        sLog->outString("second");
                        if (Creature* barricadeTrigger = player->FindNearestCreature(NPC_BARRICADE_TRIGGER, 100.0f, false))
                        {
                            sLog->outString("third");
                            barricade->SetRespawnTime(0);
                            barricadeTrigger->Respawn(true);
                            anvil->DeleteFromDB();
                        }
                    }
                }
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
    new npc_zombie_barricade_trigger;
    new npc_zombie;
    new npc_bomb_bot;
    new npc_zombie_kill_counter;
    new npc_zombie_spawner;
    new npc_proximity_mine;
    new spell_repair_channel;
}