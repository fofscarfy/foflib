#if defined _foflib_scoreboardmanager_included
    #endinput
#endif
#define _foflib_scoreboardmanager_included

#include <../include/sourcemod>
#include <../include/sdktools>
#include <../include/sdkhooks>

#define FOFLIB_SCOREBOARD_NUMFIELDS 9
#define FOFLIB_SCOREBOARD_NUMENTRIES 26
#define FOFLIB_SCOREBOARD_CELL_SIZE 4

#define FOFLIB_FOFSTATE_FLAG_DEV (1 << 0)
#define FOFLIB_FOFSTATE_FLAG_BADGE (1 << 1)
#define FOFLIB_FOFSTATE_FLAG_FOF_LOGO (1 << 2)
#define FOFLIB_FOFSTATE_FLAG_FIRST_TROPHY (1 << 3)
#define FOFLIB_FOFSTATE_FLAG_SECOND_TROPHY (1 << 4)
#define FOFLIB_FOFSTATE_FLAG_THIRD_TROPHY (1 << 5)
#define FOFLIB_FOFSTATE_FLAG_COLOR_LOGO (1 << 6)
#define FOFLIB_FOFSTATE_FLAG_TOP_TEN (1 << 9)
#define FOFLIB_FOFSTATE_FLAG_TOP_FIFTY (1 << 10)
#define FOFLIB_FOFSTATE_FLAG_TOP_HUNDRED (1 << 11)
#define FOFLIB_FOFSTATE_FLAG_FIRST_BADGE (1 << 12)
#define FOFLIB_FOFSTATE_FLAG_SECOND_BADGE (1 << 13)
#define FOFLIB_FOFSTATE_FLAG_THIRD_BADGE (1 << 14)
#define FOFLIB_FOFSTATE_FLAG_TOP_ONE (1 << 16)

enum BoardFlags
{
    BoardFlag_None = 0x000,
    BoardFlag_Ping = 0x001,
    BoardFlag_Frags = 0x002,
    BoardFlag_Notoriety = 0x004,
    BoardFlag_Deaths = 0x008,
    BoardFlag_Connected = 0x010,
    BoardFlag_Team = 0x020,
    BoardFlag_Alive = 0x040,
    BoardFlag_Health = 0x080,
    BoardFlag_FofState = 0x100,
    BoardFlag_All = 0x1FF
}

static const BoardFlags g_flagIdxs[FOFLIB_SCOREBOARD_NUMFIELDS] = 
{
    BoardFlag_Ping, BoardFlag_Frags, BoardFlag_Notoriety,
    BoardFlag_Deaths, BoardFlag_Connected, BoardFlag_Team,
    BoardFlag_Alive, BoardFlag_Health, BoardFlag_FofState
}

static const char g_flagNames[FOFLIB_SCOREBOARD_NUMFIELDS][16] = 
{
    "m_iPing", "m_iScore", "m_iExp",
    "m_iDeaths", "m_bConnected", "m_iTeam",
    "m_bAlive", "m_iHealth", "m_iFoFState"
}

// Taken from netprops
// static const int g_offsets[FOFLIB_SCOREBOARD_NUMFIELDS] = 
// {
//     848, 952, 1056,
//     1160, 1264, 1368,
//     1472, 1576, 1680
// }

typeset ScoreboardUpdateCallback
{
    function void();
}

static bool g_offsetsStored;
static int g_offsets[FOFLIB_SCOREBOARD_NUMFIELDS];

static any g_data[FOFLIB_SCOREBOARD_NUMFIELDS][FOFLIB_SCOREBOARD_NUMENTRIES];
static any g_underlying[FOFLIB_SCOREBOARD_NUMFIELDS][FOFLIB_SCOREBOARD_NUMENTRIES];

static BoardFlags g_managing, g_override, g_subtract;
static BoardFlags g_resolveSubtract, g_updateOnNext;

static PrivateForward g_runBefore, g_runAfter;

static bool g_hooked;

methodmap Scoreboard __nullable__
{
    
    public Scoreboard(BoardFlags handlingFlags=BoardFlag_None, BoardFlags subtractModeFlags=BoardFlag_None)
    {
        g_managing = handlingFlags;

        g_override = BoardFlag_All & ~subtractModeFlags;
        g_subtract = BoardFlag_All & subtractModeFlags;

        return view_as<Scoreboard>(1);
    }



    /**
     * Activation/Destruction
     */

    public void Toggle(bool enable)
    {
        if (!g_offsetsStored) _StoreOffsets();

        if (enable == g_hooked) return;
        g_hooked = enable;

        int scoreboard = GetPlayerResourceEntity();
        if (scoreboard != -1)
        {
            SDKUnhook(scoreboard, SDKHook_ThinkPost, _Hook_UpdateScoreboard);
            if (enable) SDKHook(scoreboard, SDKHook_ThinkPost, _Hook_UpdateScoreboard);
        }

        if (enable) HookEvent("game_start", _Event_HookScoreboardOnNewMap);
        else UnhookEvent("game_start", _Event_HookScoreboardOnNewMap);
    }

    public Scoreboard Destroy(bool zero=true)
    {
        this.Toggle(false);

        if (g_runBefore != null) { delete g_runBefore; g_runBefore = null; }
        if (g_runAfter != null) { delete g_runAfter; g_runAfter = null; }

        if (zero)
        {
            for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
            {
                for (int j = 0; j < FOFLIB_SCOREBOARD_NUMENTRIES; j++)
                {
                    g_data[i][j] = 0;
                }
            }
        }

        return null;
    }



    /**
     * Hooks
     */

    public bool HookUpdate(ScoreboardUpdateCallback func, bool preUpdate=false, Handle plugin=INVALID_HANDLE)
    {
        bool result = false;
        if (preUpdate)
        {
            if (g_runBefore == null) g_runBefore = new PrivateForward(ET_Ignore);
            result = g_runBefore.AddFunction(plugin, func);
            if (g_runBefore.FunctionCount == 0) { delete g_runBefore; g_runBefore = null; }
        }
        else
        {
            if (g_runAfter == null) g_runAfter = new PrivateForward(ET_Ignore);
            result = g_runAfter.AddFunction(plugin, func);
            if (g_runAfter.FunctionCount == 0) { delete g_runAfter; g_runAfter = null; }
        }

        return result;
    }

    public bool UnhookUpdate(ScoreboardUpdateCallback func, bool preUpdate=false, Handle plugin=INVALID_HANDLE)
    {
        bool result = false;
        if (preUpdate)
        {
            if (g_runBefore == null) return false;
            result = g_runBefore.RemoveFunction(plugin, func);
            if (g_runBefore.FunctionCount == 0) { delete g_runBefore; g_runBefore = null; }
        }
        else 
        {
            if (g_runAfter == null) return false;
            result = g_runAfter.RemoveFunction(plugin, func);
            if (g_runAfter.FunctionCount == 0) { delete g_runAfter; g_runAfter = null; }
        }

        return result;
    }



    /**
     * Getters/Setters
     */

    public bool Get(int client, BoardFlags field, any &value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        value = g_data[idx][client];
        return true;
    }

    public bool Set(int client, BoardFlags field, any value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;
        
        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        g_data[idx][client] = value;

        return true;
    }

    public bool GetUnderlying(int client, BoardFlags field, any &value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        if (g_hooked)
        {
            value = g_underlying[idx][client];
        }
        else
        {
            if (!g_offsetsStored) _StoreOffsets();
            int board = GetPlayerResourceEntity();
            if (board == -1) return false;

            value = GetEntData(board, g_offsets[idx] + FOFLIB_SCOREBOARD_CELL_SIZE * client);
        }

        return true;
    }

    public bool GetAll(BoardFlags field, any values[FOFLIB_SCOREBOARD_NUMENTRIES])
    {
        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMENTRIES; i++)
        {
            values[i] = g_data[idx][i];
        }

        return true;
    }

    public bool SetAll(BoardFlags field, any values[FOFLIB_SCOREBOARD_NUMENTRIES])
    {
        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMENTRIES; i++)
        {
            g_data[idx][i] = values[i];
        }

        return true;
    }

    public bool GetAllUnderlying(BoardFlags field, any values[FOFLIB_SCOREBOARD_NUMENTRIES])
    {
        int idx = _GetFirstFlagIndex(field);
        if (idx == -1) return false;

        if (g_hooked)
        {
            for (int i = 0; i < FOFLIB_SCOREBOARD_NUMENTRIES; i++)
            {
                values[i] = g_underlying[idx][i];
            }
        }
        else
        {
            if (!g_offsetsStored) _StoreOffsets();
            int board = GetPlayerResourceEntity();
            if (board == -1) return false;

            GetEntDataArray(board, g_offsets[idx], values, FOFLIB_SCOREBOARD_NUMENTRIES);
        }

        return true;
    }

    public bool GetPlayer(int client, any values[FOFLIB_SCOREBOARD_NUMFIELDS])
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
        {
            values[i] = g_data[i][client];
        }

        return true;
    }

    public bool SetPlayer(int client, any values[FOFLIB_SCOREBOARD_NUMFIELDS])
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
        {
            g_data[i][client] = values[i];
        }

        return true;
    }

    public bool GetPlayerUnderlying(int client, any values[FOFLIB_SCOREBOARD_NUMFIELDS])
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        if (g_hooked)
        {
            for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
            {
                values[i] = g_underlying[i][client];
            }
        }
        else
        {
            if (!g_offsetsStored) _StoreOffsets();
            int board = GetPlayerResourceEntity();
            // In transition
            if (board == -1) return true;

            for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
            {
                values[i] = GetEntProp(board, Prop_Send, g_flagNames[i], FOFLIB_SCOREBOARD_CELL_SIZE, client);
            }
        }

        return true;
    }



    /**
     * Advanced Getters/Setters
     */

    public void Zero(BoardFlags flags=~BoardFlag_None)
    {
        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS && flags; i++)
        {
            if (!(g_flagIdxs[i] & flags)) continue;
            flags &= ~g_flagIdxs[i];

            for (int j = 0; j < FOFLIB_SCOREBOARD_NUMENTRIES; j++)
            {
                g_data[i][j] = 0;
            }
        }
    }

    public void SetAllFromSubtract(BoardFlags flags, BoardFlags clearSubtract=BoardFlag_None)
    {
        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS && flags; i++)
        {
            if (!(g_flagIdxs[i] & flags)) continue;

            for (int j = 0; j < FOFLIB_SCOREBOARD_NUMENTRIES; j++)
            {
                g_data[i][j] = g_underlying[i][j] - g_data[i][j];
            }

            if (g_flagIdxs[i] & clearSubtract) g_subtract &= ~g_flagIdxs[i];
        }
    }

    public void UpdateFromNextUnderlying(BoardFlags flags)
    {
        g_updateOnNext = flags;
    }

    public void UpdateFromNextSubtract(BoardFlags flags)
    {
        g_resolveSubtract = flags;
    }



    /**
     * Properties
     */

    property BoardFlags HandlingFlags
    {
        public get() { return g_managing; }
        public set(BoardFlags flags) { g_managing = flags; }
    }

    property BoardFlags OverrideModeFlags
    {
        public get() { return g_override; }
        public set(BoardFlags flags)
        {
            g_override |= flags;
            g_subtract &= ~flags;
        }
    }

    property BoardFlags SubtractModeFlags
    {
        public get() { return g_subtract; }
        public set(BoardFlags flags)
        {
            g_subtract |= flags;
            g_override &= ~flags;
        }
    }

    property bool Active
    {
        public get() { return g_hooked; }
        public set(bool enable) { this.Toggle(enable); }
    }
}



/**
 * Callbacks
 */

static void _Hook_UpdateScoreboard(int entity)
{
    BoardFlags iterflag, flags = g_managing;
    any subtract[FOFLIB_SCOREBOARD_NUMENTRIES];

    if (g_runBefore != null)
    {
        Call_StartForward(g_runBefore);
        Call_Finish();
    }

    for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
    {
        iterflag = g_flagIdxs[i];
        if (g_updateOnNext & iterflag) GetEntDataArray(entity, g_offsets[i], g_data[i], FOFLIB_SCOREBOARD_NUMENTRIES);

        if (!(flags & iterflag)) continue;
        flags &= ~iterflag;

        GetEntDataArray(entity, g_offsets[i], g_underlying[i], FOFLIB_SCOREBOARD_NUMENTRIES);

        if (g_flagIdxs[i] & (g_subtract | g_resolveSubtract))
        {
            for (int j = 0; j < FOFLIB_SCOREBOARD_NUMENTRIES; j++)
            {
                subtract[j] = g_underlying[i][j] - g_data[i][j];
            }

            if (g_flagIdxs[i] & g_resolveSubtract)
            {
                g_resolveSubtract &= ~g_flagIdxs[i];
                for (int j = 0; j < FOFLIB_SCOREBOARD_NUMENTRIES; j++)
                {
                    g_data[i][j] = subtract[j];
                }
            }
        }

        SetEntDataArray(
                entity, 
                g_offsets[i], 
                g_flagIdxs[i] & g_subtract ? subtract : g_data[i], 
                FOFLIB_SCOREBOARD_NUMENTRIES, 
                FOFLIB_SCOREBOARD_CELL_SIZE, 
                true
        );
    }

    g_updateOnNext = BoardFlag_None;

    if (g_runAfter != null)
    {
        Call_StartForward(g_runAfter);
        Call_Finish();
    }
}

static void _Event_HookScoreboardOnNewMap(Event event, const char[] name, bool dontBroadcast)
{
    int scoreboard = GetPlayerResourceEntity();
    if (scoreboard == -1)
    {
        LogError("Could not hook scoreboard on new game!");
        return;
    }

    SDKUnhook(scoreboard, SDKHook_ThinkPost, _Hook_UpdateScoreboard);
    SDKHook(scoreboard, SDKHook_ThinkPost, _Hook_UpdateScoreboard);
}



/**
 * Utilities
 */

static bool _StoreOffsets()
{

    for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
    {
        g_offsets[i] = FindSendPropInfo("CPlayerResource", g_flagNames[i]);
        if (g_offsets[i] <= 0) return false;
    }

    g_offsetsStored = true;
    return true;
}

static int _GetFirstFlagIndex(BoardFlags boardFlags)
{
    for (int i = 0; i < FOFLIB_SCOREBOARD_NUMFIELDS; i++)
    {
        if (boardFlags & g_flagIdxs[i]) return i;
    }

    return -1;
}