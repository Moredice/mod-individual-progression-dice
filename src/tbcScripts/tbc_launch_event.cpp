#include "IndividualProgression.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

enum TbcLaunchEvent
{
    NPC_INVADING_FELGUARD           = 19284,
    NPC_INVADING_INFERNAL           = 19285,
    NPC_INVADING_FEL_STALKER        = 19286,
    NPC_INVADING_VOIDWALKER         = 19287,
    NPC_INVADING_ANGUISHER          = 19290,

    NPC_LORD_KAZZAK                 = 12397,
    NPC_HIGHLORD_KRUUL              = 18338,

    STAGE_INVASION_PROGRESS_START   = 0,
    STAGE_INVASION_PROGRESS_1       = 1,
    STAGE_INVASION_PROGRESS_2       = 2,
    STAGE_INVASION_PROGRESS_3       = 3,
    STAGE_INVASION_PROGRESS_END     = 4,
    STAGE_INVASION_PROGRESS_RESET   = 5
};

class npc_agent_proudwell : public CreatureScript
{
public:
    npc_agent_proudwell() : CreatureScript("npc_agent_proudwell") { }
    
    struct npc_agent_proudwellAI : public ScriptedAI
    {
        npc_agent_proudwellAI(Creature* creature) : ScriptedAI(creature)
        {}

        bool CanBeSeen(Player const* player) override
        {
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());

            if (target->IsGameMaster())
            {
                return true;
            }

            return (
                sIndividualProgression->hasPassedProgression(target, PROGRESSION_NAXX40) &&
                sIndividualProgression->isBeforeProgression(target, PROGRESSION_TBC_LAUNCH)
            );
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_agent_proudwellAI(creature);
    }
};

void AddSC_tbc_launch_event_scripts()
{
    new npc_agent_proudwell();
}
