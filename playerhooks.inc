#if defined _foflib_playerhooks_included
    #endinput
#endif
#define _foflib_playerhooks_included

#include <../include/sourcemod>
#include <../include/sdktools>
#include <../include/sdkhooks>

#include "playerutils.inc"

static ArrayList g_spawnHooks;

enum struct _SDKCallback
{
    SDKHookCB callback;
    SDKHookType type;
    bool enabled;
}

stock bool PlayerHooks_Destroy()
{
    return PlayerHooks_RemoveAllHooks();
}

stock bool PlayerHooks_AddHook(SDKHookType type, SDKHookCB callback, bool enabled=true, bool hookAll=true)
{
    if (g_spawnHooks == null) 
    {
        g_spawnHooks = new ArrayList(sizeof(_SDKCallback));
        HookEvent("player_connect_fof", _Event_AddAllHooks);
    }
    else if (PlayerHooks_FindIndex(type, callback) != -1) return false;

    if (enabled && hookAll) PlayerHooks_HookAllOnce(type, callback);

    _SDKCallback sc;
    sc.type = type;
    sc.callback = callback;
    sc.enabled = enabled;

    g_spawnHooks.PushArray(sc, sizeof(sc));

    return true;
}

stock bool PlayerHooks_RemoveHook(SDKHookType type, SDKHookCB callback, bool unhookAll=true)
{
    int idx = PlayerHooks_FindIndex(type, callback);
    if (idx == -1) return false;

    if (unhookAll)
    {
        _SDKCallback sc;
        g_spawnHooks.GetArray(idx, sc, sizeof(sc));

        for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
        {
            SDKUnhook(c, sc.type, sc.callback);
        }
    }

    g_spawnHooks.Erase(idx);
    if (g_spawnHooks.Length == 0)
    {
        delete g_spawnHooks;
        g_spawnHooks = null;

        UnhookEvent("player_connect_fof", _Event_AddAllHooks);
    }

    return true;
}

stock void PlayerHooks_HookAllOnce(SDKHookType type, SDKHookCB callback)
{
    for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
    {
        SDKUnhook(c, type, callback);
        SDKHook(c, type, callback);
    }
}

stock void PlayerHooks_UnhookAllOnce(SDKHookType type, SDKHookCB callback)
{
    for (int c = 0; PlayerUtils_GetNextValidPlayer(c);) SDKUnhook(c, type, callback);
}

stock bool PlayerHooks_RemoveHooksByType(SDKHookType type, bool unhookAll=true)
{
    if (g_spawnHooks == null) return false;

    _SDKCallback sc;
    for (int i = g_spawnHooks.Length - 1; i >= 0; i--)
    {
        g_spawnHooks.GetArray(i, sc, sizeof(sc));

        if (sc.type != type) continue;
        g_spawnHooks.Erase(i);

        if (unhookAll)
        {
            for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
            {
                SDKUnhook(c, sc.type, sc.callback);
            }
        }
    }

    if (g_spawnHooks.Length == 0)
    {
        delete g_spawnHooks;
        g_spawnHooks = null;
        UnhookEvent("player_connect_fof", _Event_AddAllHooks);
    }

    return true;
}

stock bool PlayerHooks_RemoveAllHooks(bool unhookAll=true)
{
    if (g_spawnHooks == null) return false;

    if (unhookAll)
    {
        _SDKCallback sc;
        for (int i = 0; i < g_spawnHooks.Length; i++)
        {
            g_spawnHooks.GetArray(i, sc, sizeof(sc));

            for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
            {
                SDKUnhook(c, sc.type, sc.callback);
            }
        }
    }

    delete g_spawnHooks;
    g_spawnHooks = null;
    UnhookEvent("player_connect_fof", _Event_AddAllHooks);

    return true;
}

stock bool PlayerHooks_ToggleHook(SDKHookType type, SDKHookCB callback, bool enabled, bool manageHooks=true)
{
    int idx = PlayerHooks_FindIndex(type, callback);
    if (idx == -1) return false;

    _SDKCallback sc;
    g_spawnHooks.GetArray(idx, sc, sizeof(sc));
    if (enabled != sc.enabled)
    {
        sc.enabled = enabled;
        g_spawnHooks.SetArray(idx, sc, sizeof(sc));
    }

    if (manageHooks)
    {
        for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
        {
            SDKUnhook(c, type, callback);
            if (enabled) SDKHook(c, type, callback);
        }
    }

    return true;
}

stock bool PlayerHooks_ToggleHooksOfType(SDKHookType type, bool enabled, bool manageHooks=true)
{
    if (g_spawnHooks == null) return false;

    _SDKCallback sc;
    for (int i = 0; i < g_spawnHooks.Length; i++)
    {
        g_spawnHooks.GetArray(i, sc, sizeof(sc));
        if (sc.type != type) continue;

        if (sc.enabled != enabled)
        {
            sc.enabled = enabled;
            g_spawnHooks.SetArray(i, sc, sizeof(sc));
        }

        if (manageHooks)
        {
            for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
            {
                SDKUnhook(c, type, sc.callback);
                if (enabled) SDKHook(c, type, sc.callback);
            }
        }
    }

    return true;
}

stock bool PlayerHooks_ToggleAllHooks(bool enabled, bool manageHooks=true)
{
    if (g_spawnHooks == null) return false;

    _SDKCallback sc;
    for (int i = 0; i < g_spawnHooks.Length; i++)
    {
        g_spawnHooks.GetArray(i, sc, sizeof(sc));

        if (sc.enabled != enabled)
        {
            sc.enabled = enabled;
            g_spawnHooks.SetArray(i, sc, sizeof(sc));
        }

        if (manageHooks)
        {
            for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
            {
                SDKUnhook(c, sc.type, sc.callback);
                if (enabled) SDKHook(c, sc.type, sc.callback);
            }
        }
    }

    return true;
}

stock int PlayerHooks_FindIndex(SDKHookType type, SDKHookCB callback)
{
    if (g_spawnHooks == null) LogMessage("WTF g_spawnHooks = null");
    if (g_spawnHooks == null) return -1;

    _SDKCallback sc;
    for (int i = 0; i < g_spawnHooks.Length; i++)
    {
        g_spawnHooks.GetArray(i, sc, sizeof(sc));

        if (sc.type == type && sc.callback == callback) return i;
    }

    return -1;
}

static Action _Event_AddAllHooks(Event event, const char[] name, bool dontBroadcast)
{
    if (g_spawnHooks == null) return Plugin_Continue;
    
    int client = GetClientOfUserId(event.GetInt("userid"));
    if (client == 0) return Plugin_Continue;

    _SDKCallback sc;
    for (int i = 0; i < g_spawnHooks.Length; i++)
    {
        g_spawnHooks.GetArray(i, sc, sizeof(sc));
        if (sc.enabled) SDKHook(client, sc.type, sc.callback);
    }

    return Plugin_Continue;
}