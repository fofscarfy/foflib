#if defined _foflib_basicplayerglow_included
    #endinput
#endif
#define _foflib_basicplayerglow_included

#include <../include/sourcemod>

#include "playerutils.inc"

static Handle g_glowTimers[MAXPLAYERS + 1];

// Enables or disables glow for a player.
// If the player is glowing for a duration, their timer is cleared.
//
// @param client        The player to target.
// @param enable        Whether to enable or disable glow.
// @param validKnown    If true, will skip the check to see if the
//                      player is a valid target.
// @return              Whether or not the operation was successful.
stock bool SetGlow(int client, bool enable, bool validKnown=false)
{
    if (!validKnown && !PlayerUtils_IsValidPlayer(client)) return false;
    _DeletePlayerTimer(client);

    SetEntProp(client, Prop_Send, "m_bGlowEnabled", enable);

    return true;
}

// Passes the player's current glow value to variable 'value'
//
// @param client        The player to target.
// @param value         The output variable assigned the glow status.
// @param validKnown    If true, will skip the check to see if the
//                      player is a valid target.
// @return              Whether or not the operation was successful.
stock bool GetGlow(int client, bool &value, bool validKnown=false)
{
    if (!validKnown && !PlayerUtils_IsValidPlayer(client)) return false;
    value = GetEntProp(client, Prop_Send, "m_bGlowEnabled") != 0;
    return true;
}

// Sets the glow effect for all players.
// If any players are glowing for a duration, it cancels their timers.
//
// @param enable        Whether to enable or disable glow.
stock void SetAllGlow(bool enable)
{
    _DeleteAllTimers();
    for (int client = 0; PlayerUtils_GetNextValidPlayer(client);)
    {
        SetEntProp(client, Prop_Send, "m_bGlowEnabled", enable);
    }
}

// Makes a player glow for a certain amount of time.
//
// @param client        The player to target.
// @param time          The duration of the glow timer.
// @param restart       If the player already has a timer set, the
//                      previous timer is canceled in favor of this timer.
// @param validKnown    If true, will skip the check to see if the
//                      player is a valid target.
// @return              Whether a glow timer was set.
stock bool GlowForDuration(int client, float time, bool restart=true, bool validKnown=false)
{
    if (!validKnown && !PlayerUtils_IsValidPlayer(client)) return false;

    if (!restart && g_glowTimers[client] != INVALID_HANDLE) return false;
    
    SetGlow(client, true, true);
    _DeletePlayerTimer(g_glowTimers[client]);
    g_glowTimers[client] = CreateTimer(time, _Timer_StopGlowing);
    SetEntProp(client, Prop_Send, "m_bGlowEnabled", 1);

    return true;
}

static Action _Timer_StopGlowing(Handle timer, int entref)
{
    int client = EntRefToEntIndex(entref);
    if (client == -1) return Plugin_Stop;

    SetEntProp(client, Prop_Send, "m_GlowEnabled", 0);
    g_glowTimers[client] = INVALID_HANDLE;

    return Plugin_Stop;
}

static void _DeleteAllTimers()
{
    for (int i = 1; i < MaxClients; i++)
    {
        if (g_glowTimers[i] != INVALID_HANDLE)
        {
            delete g_glowTimers[i];
            g_glowTimers[i] = INVALID_HANDLE;
        }
    }
}

static void _DeletePlayerTimer(int client)
{
    if (g_glowTimers[client] != INVALID_HANDLE)
    {
        delete g_glowTimers[client];
        g_glowTimers[client] = INVALID_HANDLE;
    }
}