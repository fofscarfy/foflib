#if defined _foflib_teams_included
    #endinput
#endif
#define _foflib_teams_included

#include "playerutils.inc"

#define FOFLIB_TEAM_FFA 0
#define FOFLIB_TEAM_SPECTATOR 1
#define FOFLIB_TEAM_VIGILANTES 2
#define FOFLIB_TEAM_DESPERADOS 3
#define FOFLIB_TEAM_BANDIDOS 4
#define FOFLIB_TEAM_RANGERS 5
#define FOFLIB_TEAM_ZOMBIES 6

static const char models[8][32] = {
    "",
    "",
    "models/playermodels/player1.mdl",
    "models/playermodels/player2.mdl",
    "models/playermodels/bandito.mdl",
    "models/playermodels/frank.mdl",
    "models/zombies/fof_zombie.mdl",
    ""
}

static int modelIndices[8];

// static int teamSwitch[32];

stock void PrecacheAllFofTeamModels()
{
    for (int i = 0; i < sizeof(models); i++)
    {
        if (strlen(models[i])) modelIndices[i] = PrecacheModel(models[i]);
    }
}

stock bool ChangePlayerTeam(int client, int team)
{
    if (!PlayerUtils_IsValidPlayer(client)) return false;

    int modTeam = team % 8;

    SetEntProp(client, Prop_Send, "m_iTeamNum", team);
    if (modTeam > 1 && modTeam < 7)
    {
        SetEntProp(client, Prop_Send, "m_nModelIndex", modelIndices[modTeam]);
    }

    return true;
}