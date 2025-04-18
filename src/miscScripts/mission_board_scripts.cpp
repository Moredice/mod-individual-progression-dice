#include "IndividualProgression.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "GameObject.h"
#include "QuestDef.h"

enum
{
    QUEST_HC_THE_DARKENED_PORTAL_A      = 28708,
    QUEST_WC_THE_DARKENED_PORTAL_H      = 28705,
    QUEST_HC_THE_FROZEN_NORTH_A         = 28709,
    QUEST_WC_THE_FROZEN_NORTH_H         = 28711
};

class go_missionboard : public GameObjectScript
{
public:
    go_missionboard() : GameObjectScript("go_missionboard") {}

    bool OnGossipHello(Player* player, GameObject* go) override
    {

        // Prepare a gossip menu for the player
        player->PrepareQuestMenu(go->GetGUID());

        // Retrieve all quests related to the GameObject
        QuestRelationBounds questRelations = sObjectMgr->GetGOQuestRelationBounds(go->GetEntry());

        QuestMenu& qm = player->PlayerTalkClass->GetQuestMenu();
        qm.ClearMenu();

        // Iterate through the quests and filter based on custom conditions
        for (QuestRelations::const_iterator itr = questRelations.first; itr != questRelations.second; ++itr)
        {
            Quest const* quest = sObjectMgr->GetQuestTemplate(itr->second);
            if (!quest)
                continue;

            uint32 questId = quest->GetQuestId();
            if (questId == 0)
                continue;

            QuestStatus qStatus = player->GetQuestStatus(questId);
            uint32 icon = 0;
            switch (qStatus)
            {
                case QUEST_STATUS_REWARDED:
                    continue;
                    break;
                case QUEST_STATUS_NONE:
                    icon = 2;
                    break;
                case QUEST_STATUS_COMPLETE:
                case QUEST_STATUS_INCOMPLETE:
                case QUEST_STATUS_FAILED:
                    icon = 4;
                    break;
            }

            if (!player->IsGameMaster())
            {
                switch (questId)
                {
                    case QUEST_HC_THE_DARKENED_PORTAL_A:
                    case QUEST_WC_THE_DARKENED_PORTAL_H:
                        if (sIndividualProgression->hasPassedProgression(player, PROGRESSION_NAXX40) && sIndividualProgression->isBeforeProgression(player, PROGRESSION_TBC_LAUNCH))
                        {
                            continue;
                        }
                        break;
                    case QUEST_HC_THE_FROZEN_NORTH_A:
                    case QUEST_WC_THE_FROZEN_NORTH_H:
                        if (sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && sIndividualProgression->isBeforeProgression(player, PROGRESSION_WOTLK_LAUNCH))
                        {
                            continue;
                        }
                        break;
                }
            }

            // If the quest passed the condition, add it to the player's available quests
            qm.AddMenuItem(questId, icon);
        }

        // Send the quest menu to the player
        player->SendPreparedQuest(go->GetGUID());
        return true;
    }
};

// Register the script with AzerothCore
void AddSC_missionboard_scripts()
{
    new go_missionboard();
}

