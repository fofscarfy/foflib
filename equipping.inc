#if defined _foflib_equipping_included
    #endinput
#endif
#define _foflib_equipping_included

#include <../include/sourcemod>
#include <../include/sdktools>
#include <../include/sdkhooks>

#include "playerutils.inc"

#define FOFLIB_MYWEAPONS_SIZE 48
#define FOFLIB_PLAYERINFO_BRASSKNUCKLES (1 << 2)
#define FOFLIB_PLAYERINFO_HASEQUIPMENT (1 << 4)
#define FOFLIB_PLAYERINFO_GUNTHROW (1 << 6)
#define FOFLIB_PLAYERINFO_BOOTS (1 << 7)
#define FOFLIB_PLAYERINFO_SLIDE (1 << 20)
#define FOFLIB_PLAYERINFO_JUMPMASTER (1 << 21)
#define FOFLIB_PLAYERINFO_HEAVYLOADS (1 << 23)

static const float WEAPON_TAKE_TIME = 0.1;

enum
{
    Handedness_Ambi = 0,
    Handedness_Right = 1,
    Handedness_Left = 2,
    Handedness_Fanning = 3
}

stock void ToggleTakeWeaponsOnSpawn(bool enable)
{
    static bool enabled = false;
    if (enabled == enable) return;

    if (enable)
    {
        HookEvent("player_spawn", _Event_TakeWeaponsOnSpawn);
    }
    else
    {
        UnhookEvent("player_spawn", _Event_TakeWeaponsOnSpawn);
    }
}

stock void RemoveWeaponsFromPlayer(int client, bool keepFists=true)
{
    int seen[FOFLIB_MYWEAPONS_SIZE];
    int seenIdx = 0;

    int weapon;
    bool found;
    char classname[64];
    for (int i = 0; i < FOFLIB_MYWEAPONS_SIZE; i++)
    {
        weapon = GetEntPropEnt(client, Prop_Send, "m_hMyWeapons", i);
        if (weapon <= MaxClients || !IsValidEntity(weapon)) continue;

        found = false;
        for (int j = 0; j < seenIdx; j++)
        {
            if (seen[j] == weapon)
            {
                found = true;
                break;
            }
        }

        if (found) continue;

        seen[seenIdx++] = weapon;
        if (keepFists)
        {
            GetEntityClassname(weapon, classname, sizeof(classname));
            if (StrEqual(classname, "weapon_fists")) continue;
        }
        RemovePlayerItem(client, weapon);
        SetEntPropEnt(client, Prop_Send, "m_hMyWeapons", INVALID_ENT_REFERENCE, i);
        if (IsValidEdict(weapon)) AcceptEntityInput(weapon, "kill");
    }
}

stock void GivePlayerFists(int client, int brass = -1)
{
    if (!PlayerUtils_IsLivingPlayer(client)) return;

    int playerFlags = GetEntProp(client, Prop_Send, "m_nPlayerInfo");

    if (brass == 0) playerFlags &= ~FOFLIB_PLAYERINFO_BRASSKNUCKLES;
    else if (brass == 1) playerFlags |= FOFLIB_PLAYERINFO_BRASSKNUCKLES;

    SetEntProp(client, Prop_Send, "m_nPlayerInfo", playerFlags);

    GivePlayerItem(client, "weapon_fists");
}


public Action _Event_TakeWeaponsOnSpawn(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    CreateTimer(WEAPON_TAKE_TIME, _Timer_RemoveWeapons, EntIndexToEntRef(client));

    return Plugin_Continue;
}

public Action _Timer_RemoveWeapons(Handle timer, int clientref)
{
    int client = EntRefToEntIndex(clientref);
    if (PlayerUtils_IsLivingPlayer(client)) RemoveWeaponsFromPlayer(client);

    return Plugin_Stop;
}

// Dropping and replacing
stock void ForceDropAndReplaceWeapon(int client, int weaponent)
{
    SDKHooks_DropWeapon(client, weaponent);
    // CreateTimer(REPLACE_TIME, Timer_Equipping_ReplaceWeapon, EntIndexToEntRef(weaponent));
    RequestFrame(_Frame_ReplaceWeapon, EntIndexToEntRef(weaponent));
}

static void _Frame_ReplaceWeapon(int entref)
{
    int weapon = EntRefToEntIndex(entref);
    if (weapon == -1) return;

    char classname[64];
    GetEntityClassname(weapon, classname, sizeof(classname));

    float pos[3], ang[3], vel[3], angvel[3];
    GetEntPropVector(weapon, Prop_Data, "m_vecAbsOrigin", pos);
    GetEntPropVector(weapon, Prop_Data, "m_angAbsRotation", ang);
    GetEntPropVector(weapon, Prop_Send, "m_vecVelocity", vel);
    GetEntPropVector(weapon, Prop_Data, "m_vecAngVelocity", angvel);

    RemoveEntity(weapon);

    // Spawning duplicate
    int dup = CreateEntityByName(classname);
    if (dup == -1) return;

    TeleportEntity(dup, pos, ang, vel);
    SetEntPropVector(dup, Prop_Data, "m_vecAngVelocity", angvel);
    SetEntPropVector(dup, Prop_Data, "m_vecVelocity", vel);
    DispatchSpawn(dup);
}