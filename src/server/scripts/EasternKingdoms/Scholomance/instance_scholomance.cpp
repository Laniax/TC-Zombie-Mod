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

/* ScriptData
SDName: Instance_Scholomance
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

enum Scholomance
{
    GO_GATE_KIRTONOS   = 175570,
    GO_GATE_GANDLING   = 177374,
    GO_GATE_MALICIA    = 177375,
    GO_GATE_THEOLEN    = 177377,
    GO_GATE_POLKELT    = 177376,
    GO_GATE_RAVENIAN   = 177372,
    GO_GATE_BAROV      = 177373,
    GO_GATE_ILLUCIA    = 177371,
    MAX_ENCOUNTER      = 2,

    // Zombie Event
    GO_BARRICADE       = 293610,
    GO_ANVIL           = 293611,
    NPC_COUNTER        = 792131,
};

class instance_scholomance : public InstanceMapScript
{
    public:
        instance_scholomance() : InstanceMapScript("instance_scholomance", 289) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_scholomance_InstanceMapScript(map);
        }

        struct instance_scholomance_InstanceMapScript : public InstanceScript
        {
            instance_scholomance_InstanceMapScript(Map* map) : InstanceScript(map) { }

            // Lord Alexei Barov, Doctor Theolen Krastinov, The Ravenian, Lorekeeper Polkelt, Instructor Malicia and the Lady Illucia Barov.
            bool isBossDied[6];
            bool zombieEvent;

            uint32 m_auiEncounter[MAX_ENCOUNTER];

            uint64 GateKirtonosGUID;
            uint64 GateGandlingGUID;
            uint64 GateMiliciaGUID;
            uint64 GateTheolenGUID;
            uint64 GatePolkeltGUID;
            uint64 GateRavenianGUID;
            uint64 GateBarovGUID;
            uint64 GateIlluciaGUID;
            uint64 AnvilGUID;
            uint64 BarricadeGUID;

            void Initialize()
            {
                zombieEvent = false;

                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

                GateKirtonosGUID = 0;
                GateGandlingGUID = 0;
                GateMiliciaGUID = 0;
                GateTheolenGUID = 0;
                GatePolkeltGUID = 0;
                GateRavenianGUID = 0;
                GateBarovGUID = 0;
                GateIlluciaGUID = 0;
                AnvilGUID = 0;
                BarricadeGUID = 0;

                for (uint8 i = 0; i < 6; ++i)
                    isBossDied[i] = false;
            }

            void OnPlayerEnter(Player* player)
            {
                if (!player)
                    return;

                if (Creature* killCounterNPC = player->FindNearestCreature(NPC_COUNTER, 200.0f))
                    zombieEvent = true;
                else return;

                player->RemoveArenaAuras();
                player->RemoveArenaSpellCooldowns(true);
            }

            void OnCreatureCreate(Creature* creature)
            {
                if (zombieEvent)
                {

                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GO_GATE_KIRTONOS:
                        GateKirtonosGUID = go->GetGUID();
                        break;
                    case GO_GATE_GANDLING:
                        GateGandlingGUID = go->GetGUID();
                    case GO_GATE_MALICIA:
                        GateMiliciaGUID = go->GetGUID();
                    case GO_GATE_THEOLEN:
                        GateTheolenGUID = go->GetGUID();
                    case GO_GATE_POLKELT:
                        GatePolkeltGUID = go->GetGUID();
                    case GO_GATE_RAVENIAN:
                        GateRavenianGUID = go->GetGUID();
                    case GO_GATE_BAROV:
                        GateBarovGUID = go->GetGUID();
                    case GO_GATE_ILLUCIA:
                        GateIlluciaGUID = go->GetGUID();
                    case GO_BARRICADE:
                        BarricadeGUID = go->GetGUID();
                    case GO_ANVIL:
                        AnvilGUID = go->GetGUID();
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_LORDALEXEIBAROV_DEATH:
                        isBossDied[0] = true;
                        break;
                    case DATA_DOCTORTHEOLENKRASTINOV_DEATH:
                        isBossDied[1] = true;
                        break;
                    case DATA_THERAVENIAN_DEATH:
                        isBossDied[2] = true;
                        break;
                    case DATA_LOREKEEPERPOLKELT_DEATH:
                        isBossDied[3] = true;
                        break;
                    case DATA_INSTRUCTORMALICIA_DEATH:
                        isBossDied[4] = true;
                        break;
                    case DATA_LADYILLUCIABAROV_DEATH:
                        isBossDied[5] = true;
                        break;
                    case TYPE_GANDLING:
                        m_auiEncounter[0] = data;
                        break;
                    case TYPE_KIRTONOS:
                        m_auiEncounter[1] = data;
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                if (type == TYPE_GANDLING)
                {
                    if (isBossDied[0] && isBossDied[1] && isBossDied[2] && isBossDied[3] && isBossDied[4] && isBossDied[5])
                    {
                        m_auiEncounter[0] = IN_PROGRESS;
                        return IN_PROGRESS;
                    }
                }

                return 0;
            }
        };
};

void AddSC_instance_scholomance()
{
    new instance_scholomance();
}
