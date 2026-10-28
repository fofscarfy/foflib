/**
 * TODO: Fuck... I should be doing all damage events shouldn't I...
 */

#if defined _foflib_damagemanager_included
    #endinput
#endif
#define _foflib_damagemanager_included

#include <../include/sourcemod>
#include <../include/sdktools>
#include <../include/sdkhooks>

#include "playerutils.inc"
#include "callbackutils.inc"

// static ArrayList g_damageFuncs = null;
// static bool g_enabled = true;
// static PrivateForward g_forward = null;

static CallbackManager g_cm;
static bool g_dmActive;

// Use handle: `Action MyDamageFunction(DamageEvent ev);`

enum struct DamageEvent
{
    int victim;
    int attacker;
    int inflictor;
    float damage;
    int damageType;
    int weapon;
    float damageForce[3];
    float damagePosition[3];
}

// stock bool DamageManager_AddDamageRule(Function func, Handle plugin=null, bool startEnabled=true)
// {
//     if (g_damageFuncs == null) 
//     {
//         _EnableCustomDamage();
//     }

//     bool result = CallbackUtils_AddCallbackRule(g_damageFuncs, plugin, func, startEnabled);

//     if (result && g_forward == null) g_forward = new PrivateForward(ET_Hook, Param_Array);
//     g_forward.AddFunction(plugin, func);

//     return result;
// }

stock bool DamageManager_AddDamageRule(Function func, Handle plugin=null, bool startEnabled=true)
{
    bool uninitialized = g_cm.funcs == null;
    if (uninitialized) g_cm.Init(ET_Hook);

    bool result = g_cm.Add(func, plugin, startEnabled);
    if (result && uninitialized) _EnableCustomDamage();

    return result;
}

// stock bool DamageManager_RemoveDamageRule(Function func, Handle plugin=null)
// {
//     if (!CallbackUtils_RemoveCallbackRule(g_damageFuncs, plugin, func)) return false;
//     if (g_damageFuncs == null) _DisableCustomDamage();

//     g_forward.RemoveFunction(plugin, func);
//     if (g_forward.FunctionCount == 0)
//     {
//         delete g_forward;
//         g_forward = null;
//     }

//     return true;
// }

stock bool DamageManager_RemoveDamageRule(Function func, Handle plugin=null)
{
    bool result = g_cm.Remove(func, plugin);
    if (result && g_cm.funcs == null) _DisableCustomDamage();

    return result;
}

// stock bool DamageManager_ToggleAllDamageRules(bool enable)
// {
//     g_enabled = enable;
//     return true;
// }

stock bool DamageManager_ToggleAllDamageRules(bool enable)
{
    return g_cm.ToggleAll(enable);
}

// stock bool DamageManager_ToggleDamageRulesByPlugin(Handle plugin, bool enable)
// {
//     return CallbackUtils_ToggleCallbackRulesByPlugin(g_damageFuncs, plugin, enable);
// }

stock bool DamageManager_ToggleDamageRulesByPlugin(Handle plugin, bool enable)
{
    return g_cm.ToggleByPlugin(plugin, enable);
}

// stock bool DamageManager_ToggleDamageRule(Handle plugin, Function func, bool enable)
// {
//     return CallbackUtils_ToggleCallbackRule(g_damageFuncs, plugin, func, enable);
// }

stock bool DamageManager_ToggleDamageRule(Handle plugin, Function func, bool enable)
{
    return g_cm.Toggle(func, plugin, enable);
}

// stock bool DamageManager_RemoveDamageRulesByPlugin(Handle plugin)
// {
//     if (!CallbackUtils_RemoveCallbackRulesByPlugin(g_damageFuncs, plugin)) return false;
//     if (g_damageFuncs == null) _DisableCustomDamage();

//     return true;
// }

stock bool DamageManager_RemoveDamageRulesByPlugin(Handle plugin)
{
    bool result = g_cm.RemoveByPlugin(plugin);
    if (result && g_cm.funcs == null) _DisableCustomDamage();

    return result;
}

// stock void DamageManager_RemoveAllDamageRules()
// {
//     CallbackUtils_RemoveAllCallbackRules(g_damageFuncs);
//     DisableCustomDamage();
// }

stock void DamageManager_RemoveAllDamageRules()
{
    g_cm.Destroy();
}

// static void _EnableCustomDamage()
// {
//     for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
//     {
//         SDKUnhook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
//         SDKHook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
//     }

//     HookEvent("player_connect_fof", _HookCustomDamageOnJoin);
// }

static void _EnableCustomDamage()
{
    if (g_dmActive) return;
    g_dmActive = true;

    for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
    {
        SDKUnhook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
        SDKHook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
    }

    HookEvent("player_connect_fof", _HookCustomDamageOnJoin);
}

// static void _DisableCustomDamage()
// {    
//     for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
//     {
//         SDKUnhook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
//     }

//     UnhookEvent("player_connect_fof", _HookCustomDamageOnJoin);
// }

static void _DisableCustomDamage()
{
    if (!g_dmActive) return;
    g_dmActive = false;

    for (int c = 0; PlayerUtils_GetNextValidPlayer(c);)
    {
        SDKUnhook(c, SDKHook_OnTakeDamage, _ManageCustomDamage);
    }

    UnhookEvent("player_connect_fof", _HookCustomDamageOnJoin);
}

// public Action _HookCustomDamageOnJoin(Event event, const char[] name, bool dontBroadcast)
// {
//     int client = GetClientOfUserId(event.GetInt("userid"));
//     if (!PlayerUtils_IsValidPlayer(client)) return Plugin_Continue;

//     if (g_damageFuncs == null) return Plugin_Continue;
//     SDKUnhook(client, SDKHook_OnTakeDamage, _ManageCustomDamage);
//     SDKHook(client, SDKHook_OnTakeDamage, _ManageCustomDamage);

//     return Plugin_Continue;
// }

static Action _HookCustomDamageOnJoin(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    if (!PlayerUtils_IsValidPlayer(client)) return Plugin_Continue;

    if (!g_dmActive) 
    {
        UnhookEvent("player_connect_fof", _HookCustomDamageOnJoin);
        return Plugin_Continue;
    }

    SDKUnhook(client, SDKHook_OnTakeDamage, _ManageCustomDamage);
    SDKHook(client, SDKHook_OnTakeDamage, _ManageCustomDamage);

    return Plugin_Continue;
}

// public Action _ManageCustomDamage(int victim, int &attacker, int &inflictor, float &damage, int &damageType, int &weapon, float damageForce[3], float damagePosition[3])
// {
//     if (g_damageFuncs == null)
//     {
//         SDKUnhook(victim, SDKHook_OnTakeDamage, _ManageCustomDamage);
//         return Plugin_Continue;
//     }

//     if (!g_enabled || g_forward == null) return Plugin_Continue;

//     DamageEvent ev;
//     ev.victim = victim;
//     ev.attacker = attacker;
//     ev.inflictor = inflictor;
//     ev.damage = damage;
//     ev.weapon = weapon;
//     ev.damageType = damageType;
//     ev.damageForce = damageForce;
//     ev.damagePosition = damagePosition;

//     Action endAction = Plugin_Continue;

//     Call_StartForward(g_forward);
//     Call_PushArray(ev, sizeof(ev));
//     int error = Call_Finish(endAction);

//     if (error != SP_ERROR_NONE)
//     {
//         LogError("Callback g_forward failed! Error %d", error);
//     }

//     attacker = ev.attacker;
//     inflictor = ev.inflictor;
//     damage = ev.damage;
//     weapon = ev.weapon;
//     damageType = ev.damageType;
//     damageForce = ev.damageForce;
//     damagePosition = ev.damagePosition;

//     return endAction;
// }

static Action _ManageCustomDamage(int victim, int &attacker, int &inflictor, float &damage, int &damageType, int &weapon, float damageForce[3], float damagePosition[3])
{
    if (!g_dmActive)
    {
        SDKUnhook(victim, SDKHook_OnTakeDamage, _ManageCustomDamage);
        return Plugin_Continue;
    }

    if (g_cm.fwd == null) return Plugin_Continue;

    DamageEvent ev;
    ev.victim = victim;
    ev.attacker = attacker;
    ev.inflictor = inflictor;
    ev.damage = damage;
    ev.weapon = weapon;
    ev.damageType = damageType;
    ev.damageForce = damageForce;
    ev.damagePosition = damagePosition;

    Action endAction;

    Call_StartForward(g_cm.fwd);
    Call_PushArray(ev, sizeof(ev));
    int error = Call_Finish(endAction);

    if (error != SP_ERROR_NONE)
    {
        LogError("Callback g_forward failed! Error %d", error);
    }

    attacker = ev.attacker;
    inflictor = ev.inflictor;
    damage = ev.damage;
    weapon = ev.weapon;
    damageType = ev.damageType;
    damageForce = ev.damageForce;
    damagePosition = ev.damagePosition;

    return endAction;
}