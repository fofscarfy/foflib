// NOTE: For now there are only events to handle the following:
// (This means shootout is covered)
// - Notoriety_Kill
// - Notoriety_KillStreak
// - Notoriety_Assist
// - Notoriety_Nemesis
// - Notoriety_HatDrop
// - Notoriety_Suicide

#if defined _foflib_customscorer_included
    #endinput
#endif
#define _foflib_customscorer_included

#include "scoreboardmanager.inc"
#include "playerutils.inc"
#include "usermsgs.inc"

typeset ScoreCallback
{
    // Kill
    // KillStreak
    // Assist
    // Nemesis
    function Action (ScoreDeathEvent event);

    // Hatdrop
    function Action (ScoreHatDropEvent event);
}

// Flags corresponding to different scoring events
enum NotorietyFlags
{
    Notoriety_None = 0,
    Notoriety_Kill = (1 << FOFLIB_NOTOMSG_KILL),
    Notoriety_KillStreak = (1 << FOFLIB_NOTOMSG_KSKILL),
    Notoriety_Assist = (1 << FOFLIB_NOTOMSG_ASSIST),
    Notoriety_Nemesis = (1 << FOFLIB_NOTOMSG_NEMESIS),
    Notoriety_Gentleman = (1 << FOFLIB_NOTOMSG_GENTLEMAN),
    Notoriety_Capture = (1 << FOFLIB_NOTOMSG_CAPTURE),
    Notoriety_HatDrop = (1 << FOFLIB_NOTOMSG_HATDROP),
    Notoriety_Suicide = (1 << FOFLIB_NOTOMSG_SUICIDE),
    Notoriety_LootDelivery = (1 << FOFLIB_NOTOMSG_LOOTDELIVERY),
    Notoriety_Token_Green = (1 << FOFLIB_NOTOMSG_TOKEN_GREEN),
    Notoriety_Token_Red = (1 << FOFLIB_NOTOMSG_TOKEN_RED),
    Notoriety_WeaponGift = (1 << FOFLIB_NOTOMSG_WEAPONGIFT),
    Notoriety_Token_Olive = (1 << FOFLIB_NOTOMSG_TOKEN_OLIVE),
    Notoriety_Token_Olive2 = (1 << FOFLIB_NOTOMSG_TOKEN_OLIVE2),
    Notoriety_LeaderDefeated = (1 << FOFLIB_NOTOMSG_LEADERDEFEATED),
    Notoriety_All = 0x7FFF
}

// Event containing fields related to a death
// This gets passed to scoring events around death.
//
// Use "Action MyFunction(ScoreDeathEvent event)" to define callbacks
enum struct ScoreDeathEvent
{
    NotorietyFlags toBroadcast;
    int victim;
    int victimScore;
    int attacker;
    int attackScore;
    int killStreakBonus;
    int nemesisBonus;
    int assistant;
    int assistScore;
    char weapon[32];
    bool headshot;
    bool penetration;
    int damagebits;
}

// Event containing fields related to a hat drop
// This is passed to hat drop scoring events.
//
// Use "Action MyFunction(ScoreHatDropEvent event)" to define callbacks
enum struct ScoreHatDropEvent
{
    NotorietyFlags toBroadcast;
    int shooter;
    int score;
}

enum CustomScorerHook
{
    CustomScorerHook_OnDeath,
    CustomScorerHook_OnHatDrop
}

enum struct _PlayerScoreData
{
    int score;
    int kills;
}

static NotorietyFlags g_passthrough, g_disabled, g_custom;
static PrivateForward g_onDeath, g_onHatDrop;

static NotorietyMsg g_events[FOFLIB_NOTOMSG_NUMTYPES];

static any g_scores[FOFLIB_SCOREBOARD_NUMENTRIES], g_kills[FOFLIB_SCOREBOARD_NUMENTRIES];
static bool g_trackKills, g_allowKillstreaks;
static bool g_dontTrack;

static bool g_active;

static Scoreboard g_scoreboard;
static StringMap g_scoreStorage;

methodmap CustomScorer __nullable__
{
    public CustomScorer(bool trackKills = false, bool allowKillstreaks=true)
    {
        g_scoreboard = new Scoreboard();

        g_trackKills = trackKills;

        g_passthrough = Notoriety_All;
        g_disabled = Notoriety_None;
        g_custom = Notoriety_None;

        g_allowKillstreaks = allowKillstreaks;

        return view_as<CustomScorer>(1);
    }

    public CustomScorer Destroy(bool destroyScoreboard=true)
    {
        if (g_onDeath != null) { delete g_onDeath; g_onDeath = null; }
        if (g_onHatDrop != null) { delete g_onHatDrop; g_onHatDrop = null; }

        if (g_scoreStorage != null) { delete g_scoreStorage; g_scoreStorage = null; }

        if (destroyScoreboard) g_scoreboard = g_scoreboard.Destroy(true);

        return null;
    }



    /**
     * Hooks
     */

    public bool HookScoreEvent(CustomScorerHook hookType, ScoreCallback callback, Handle plugin=INVALID_HANDLE)
    {
        bool result = false;

        switch (hookType)
        {
            case CustomScorerHook_OnDeath:
            {
                if (g_onDeath == null)
                {
                    g_onDeath = new PrivateForward(ET_Ignore, Param_Array);
                }

                result = g_onDeath.AddFunction(plugin, callback);
                if (g_onDeath.FunctionCount == 0) { delete g_onDeath; g_onDeath = null; }
            }

            case CustomScorerHook_OnHatDrop:
            {
                if (g_onHatDrop == null)
                {
                    g_onHatDrop = new PrivateForward(ET_Ignore, Param_CellByRef, 
                            Param_CellByRef, Param_CellByRef);
                }
                
                result = g_onHatDrop.AddFunction(plugin, callback);
                if (g_onHatDrop.FunctionCount == 0) { delete g_onHatDrop; g_onHatDrop = null; }
            }
        }

        return result;
    }

    public bool UnhookScoreEvent(CustomScorerHook hookType, ScoreCallback callback, Handle plugin=INVALID_HANDLE)
    {
        bool result = false;

        switch (hookType)
        {
            case CustomScorerHook_OnDeath:
            {
                if (g_onDeath == null) return false;

                result = g_onDeath.RemoveFunction(plugin, callback);
                if (g_onDeath.FunctionCount == 0) { delete g_onDeath; g_onDeath = null; }
            }

            case CustomScorerHook_OnHatDrop:
            {
                if (g_onHatDrop == null) return false;

                result = g_onHatDrop.RemoveFunction(plugin, callback);
                if (g_onHatDrop.FunctionCount == 0) { delete g_onHatDrop; g_onHatDrop = null; }
            }
        }

        return result;
    }



    /**
     * Activation
     */

    public void ToggleScoring(bool enable, bool toggleScoreboardOnDisable=true)
    {
        if (g_active == enable) return;
        g_active = enable;

        if (enable)
        {
            g_scoreboard.HandlingFlags |= BoardFlag_Notoriety;
            if (g_trackKills) g_scoreboard.HandlingFlags |= BoardFlag_Frags;
            g_scoreboard.HookUpdate(_Hook_CopyScore, true);
            g_scoreboard.Active = true;
            HookUserMessage(FOFLIB_USERMSG_NOTORIETY, _Hook_OnScoreMessage, true);
            HookEvent("player_death", _Event_PairWithNotoMessage, EventHookMode_Pre);
            HookEvent("hatshot", _Event_ScoreHatDrop, EventHookMode_Pre);
            HookEvent("player_disconnect", _Event_StorePlayerScore);
            HookEvent("player_connect_fof", _Event_RestorePlayerScore);
        }
        else
        {
            g_scoreboard.UnhookUpdate(_Hook_CopyScore, true);
            g_scoreboard.HandlingFlags &= ~BoardFlag_Notoriety;
            if (g_trackKills) g_scoreboard.HandlingFlags &= ~BoardFlag_Frags;
            if (toggleScoreboardOnDisable) g_scoreboard.Active = false;
            UnhookUserMessage(FOFLIB_USERMSG_NOTORIETY, _Hook_OnScoreMessage, true);
            UnhookEvent("player_death", _Event_PairWithNotoMessage, EventHookMode_Pre);
            UnhookEvent("hatshot", _Event_ScoreHatDrop, EventHookMode_Pre);
            UnhookEvent("player_disconnect", _Event_StorePlayerScore);
            UnhookEvent("player_connect_fof", _Event_RestorePlayerScore);
        }
    }



    /**
     * Getters/Setters
     */

    public bool GetScoreSingle(int client, int &value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;
        value = g_scores[client];
        return true;
    }

    public bool SetScoreSingle(int client, int value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;
        g_scores[client] = value;
        return true;
    }

    public bool GetKillsSingle(int client, int &value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;
        value = g_kills[client];
        return true;
    }

    public bool SetKillsSingle(int client, int &value)
    {
        if (client < 0 || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;
        g_kills[client] = value;
        return;
    }

    public void GetScores(int[] scores, int size)
    {
        for (int i = 0; i < size && i < sizeof(g_scores); i++)
        {
            scores[i] = g_scores[i];
        }
    }

    public void SetScores(int[] scores, int size)
    {
        for (int i = 0; i < size && i < sizeof(g_scores); i++)
        {
            g_scores[i] = scores[i];
        }
    }

    public void GetKills(int[] kills, int size)
    {
        for (int i = 0; i < size && i < sizeof(g_kills); i++)
        {
            kills[i] = g_kills[i];
        }
    }

    public void SetKills(int[] kills, int size)
    {
        for (int i = 0; i < size && i < sizeof(g_kills); i++)
        {
            g_kills[i] = kills[i];
        }
    }



    /**
     * Sending Events
     */

    public bool SendScoreEventSingle(int client, NotorietyFlags eventType, int score, int bonus=0)
    {
        if (!PlayerUtils_IsValidPlayer(client) || client >= FOFLIB_SCOREBOARD_NUMENTRIES) return false;

        if (_NotoFlagToInt(eventType) == -1)
        {
            return false;
        }

        NotorietyMsg ev;
        ev.players[0] = client;
        ev.numPlayers = 1;
        ev.eventType = _NotoFlagToInt(eventType);
        ev.score = score;
        ev.bonus = bonus;

        g_dontTrack = true;
        ev.Send();
        g_dontTrack = false;

        return true;
    }

    public bool SendScoreEvent(int[] clients, int numClients, NotorietyFlags eventType, int score, int bonus=0)
    {
        for (int i = 0; i < numClients; i++)
        {
            if (!PlayerUtils_IsValidPlayer(clients[i]) || clients[i] >= FOFLIB_SCOREBOARD_NUMENTRIES)
            {
                return false;
            }
        }

        if (_NotoFlagToInt(eventType) == -1)
        {
            return false;
        }

        NotorietyMsg ev;
        for (int i = 0; i < numClients && i < sizeof(ev.players); i++) 
        {
            ev.players[i] = clients[i];
        }
        ev.numPlayers = numClients < sizeof(ev.players) ? numClients : sizeof(ev.players);
        ev.eventType = _NotoFlagToInt(eventType);
        ev.score = score;
        ev.bonus = bonus;

        g_dontTrack = true;
        ev.Send();
        g_dontTrack = false;

        return true;
    }



    /**
     * Utilities
     */

    public void Zero()
    {
        if (g_scoreStorage != null)
        {
            delete g_scoreStorage;
            g_scoreStorage = null;
        }

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMENTRIES; i++)
        {
            g_kills[i] = 0;
            g_scores[i] = 0;
        }
    }

    public void Reset()
    {
        g_passthrough = Notoriety_All;
        g_disabled = Notoriety_None;
        g_custom = Notoriety_None;

        if (g_onDeath != null)
        {
            delete g_onDeath;
            g_onDeath = null;
        }

        if (g_onHatDrop != null)
        {
            delete g_onHatDrop;
            g_onHatDrop = null;
        }

        if (g_scoreStorage != null)
        {
            delete g_scoreStorage;
            g_scoreStorage = null;
        }

        for (int i = 0; i < FOFLIB_SCOREBOARD_NUMENTRIES; i++)
        {
            g_kills[i] = 0;
            g_scores[i] = 0;
        }
    }



    /**
     * Properties
     */

    property NotorietyFlags AsIsMessages
    {
        public get() { return g_passthrough; }
        public set(NotorietyFlags flags)
        {
            g_passthrough |= flags;
            g_disabled &= ~flags;
            g_custom &= ~flags;
        }
    }

    property NotorietyFlags DisabledMessages
    {
        public get() { return g_disabled; }
        public set(NotorietyFlags flags)
        {
            g_passthrough &= ~flags;
            g_disabled |= flags;
            g_custom &= ~flags;
        }
    }

    property NotorietyFlags CustomHandledMessages
    {
        public get() { return g_custom; }
        public set(NotorietyFlags flags)
        {
            g_passthrough &= ~flags;
            g_disabled &= ~flags;
            g_custom |= flags;
        }
    }

    property bool TrackKills
    {
        public get() { return g_trackKills; }
        public set(bool enabled)
        {
            if (enabled == g_trackKills) return;
            g_trackKills = enabled;

            if (enabled)
            {
                g_scoreboard.HandlingFlags |= BoardFlag_Frags;
            }
            else
            {
                g_scoreboard.HandlingFlags &= ~BoardFlag_Frags;
            }
        }
    }

    property bool AllowKillStreaks
    {
        public get() { return g_allowKillstreaks; }
        public set(bool allowKillstreaks) { g_allowKillstreaks = allowKillstreaks; }
    }

    property Scoreboard ScorerScoreboard
    {
        public get() { return g_scoreboard; }
    }
}

/**
 * Callbacks
 */

static void _Hook_CopyScore()
{
    g_scoreboard.SetAll(BoardFlag_Notoriety, g_scores);
    if (g_trackKills) g_scoreboard.SetAll(BoardFlag_Frags, g_kills);
}

static Action _Hook_OnScoreMessage(UserMsg msg_id, BfRead msg, const int[] players, int playersNum, bool reliable, bool init)
{
    if (msg.BytesLeft != 6) 
    {
        LogMessage("BytesLeft != 6! (%d)", msg.BytesLeft);
        char bytestr[256], hexstr[1024], tmp[8];
        for (int i = 0; i < sizeof(bytestr) && msg.BytesLeft > 0; i++)
        {
            bytestr[i] = msg.ReadByte();
            Format(tmp, sizeof(tmp), "%02X ", bytestr[i] & 0xFF);
            StrCat(hexstr, sizeof(hexstr), tmp);
        }
        LogMessage("Msg=%s", hexstr);

        return Plugin_Continue;
    }

    NotorietyMsg ev;
    ev.Decode(msg);

    if (!g_allowKillstreaks && ev.eventType == FOFLIB_NOTOMSG_KSKILL)
    {
        ev.eventType = Notoriety_Kill;
        ev.bonus = 0;
    }

    if (ev.eventType == -1) return Plugin_Continue;
    NotorietyFlags flag = view_as<NotorietyFlags>(1 << ev.eventType);

    ev.numPlayers = playersNum;
    
    for (int i = 0; i < playersNum && i < sizeof(ev.players); i++)
    {
        ev.players[i] = players[i];
    }

    if (!g_dontTrack) g_events[ev.eventType] = ev;

    // Standard score update, no notification
    if (flag & g_passthrough || g_dontTrack)
    {
        // The language tokens that start with "Cash_" give money instead of points
        if (strncmp(ev.text, "Cash_", 5)) return Plugin_Continue;

        for (int i = 0; i < playersNum; i++)
        {
            g_scores[players[i]] += ev.score;
            if (ev.eventType == FOFLIB_NOTOMSG_KSKILL) g_scores[players[i]] += ev.bonus;

            if (ev.eventType <= FOFLIB_NOTOMSG_KSKILL) g_kills[players[i]]++;
        }

        return Plugin_Continue;
    }

    // Custom scoring
    return Plugin_Handled;
}

static Action _Event_ScoreHatDrop(Event event, const char[] name, bool dontBroadcast)
{
    if (g_events[FOFLIB_NOTOMSG_HATDROP].eventType != FOFLIB_NOTOMSG_HATDROP) return Plugin_Continue;

    if (!(g_custom & Notoriety_HatDrop)) return Plugin_Continue;

    if (g_onHatDrop != null)
    {
        ScoreHatDropEvent hatDropEvent;

        hatDropEvent.toBroadcast = Notoriety_HatDrop;
        hatDropEvent.shooter = g_events[FOFLIB_NOTOMSG_HATDROP].players[0];
        hatDropEvent.score = g_events[FOFLIB_NOTOMSG_HATDROP].score;

        Call_StartForward(g_onHatDrop);
        Call_PushArrayEx(hatDropEvent, sizeof(hatDropEvent), SM_PARAM_COPYBACK);
        int error = Call_Finish();
        if (error != SP_ERROR_NONE)
        {
            LogError("Error with g_hatDropForward! Error %d", error);
        }

        g_events[FOFLIB_NOTOMSG_HATDROP].players[0] = hatDropEvent.shooter;
        g_events[FOFLIB_NOTOMSG_HATDROP].score = hatDropEvent.score;
        g_events[FOFLIB_NOTOMSG_HATDROP].eventType = hatDropEvent.toBroadcast & Notoriety_HatDrop ? FOFLIB_NOTOMSG_HATDROP : -1;
    }

    if (g_events[FOFLIB_NOTOMSG_HATDROP].eventType == FOFLIB_NOTOMSG_HATDROP) 
    {
        g_dontTrack = true;
        g_events[FOFLIB_NOTOMSG_HATDROP].Send();
        g_dontTrack = false;
    }

    int client = g_events[FOFLIB_NOTOMSG_HATDROP].players[0];
    g_events[FOFLIB_NOTOMSG_HATDROP].Zero();

    if (PlayerUtils_IsValidPlayer(client))
    {
        event.SetInt("entindex_hatshot", GetClientUserId(client));
        return Plugin_Changed;
    }

    return Plugin_Continue;
}

static Action _Event_PairWithNotoMessage(Event event, const char[] name, bool dontBroadcast)
{
    int victim = GetClientOfUserId(event.GetInt("userid"));
    bool headshot = event.GetBool("headshot");
    int damagebits = event.GetInt("damagebits");
    char weapon[64]; event.GetString("weapon", weapon, sizeof(weapon));
    bool penetration = event.GetBool("penetration");
    int weaponidx = event.GetInt("weapon_index");

    NotorietyFlags flags;

    g_events[FOFLIB_NOTOMSG_KILL].Zero(true);
    g_events[FOFLIB_NOTOMSG_KSKILL].Zero(true);
    g_events[FOFLIB_NOTOMSG_ASSIST].Zero(true);
    g_events[FOFLIB_NOTOMSG_NEMESIS].Zero(true);
    g_events[FOFLIB_NOTOMSG_SUICIDE].Zero(true);

    if (g_events[FOFLIB_NOTOMSG_KILL].eventType == FOFLIB_NOTOMSG_KILL)
    {
        flags |= Notoriety_Kill;
        g_events[FOFLIB_NOTOMSG_KSKILL] = g_events[FOFLIB_NOTOMSG_KILL];
        g_events[FOFLIB_NOTOMSG_KSKILL].eventType = FOFLIB_NOTOMSG_KSKILL;
        g_events[FOFLIB_NOTOMSG_KILL].eventType = -1;
    }
    else if (g_events[FOFLIB_NOTOMSG_KSKILL].eventType == FOFLIB_NOTOMSG_KSKILL)
    {
        flags |= Notoriety_KillStreak;
    }
    else if (g_events[FOFLIB_NOTOMSG_SUICIDE].eventType == FOFLIB_NOTOMSG_SUICIDE)
    {
        flags |= Notoriety_Suicide;
    }

    if (g_events[FOFLIB_NOTOMSG_ASSIST].eventType == FOFLIB_NOTOMSG_ASSIST)
    {
        flags |= Notoriety_Assist;
    }

    if (g_events[FOFLIB_NOTOMSG_NEMESIS].eventType == FOFLIB_NOTOMSG_NEMESIS)
    {
        flags |= Notoriety_Nemesis;
    }

    flags &= g_custom;

    if (flags == Notoriety_None)
    {
        // Nothing custom to do

        g_events[FOFLIB_NOTOMSG_KILL].Zero();
        g_events[FOFLIB_NOTOMSG_KSKILL].Zero();
        g_events[FOFLIB_NOTOMSG_ASSIST].Zero();
        g_events[FOFLIB_NOTOMSG_NEMESIS].Zero();
        g_events[FOFLIB_NOTOMSG_SUICIDE].Zero();
        return Plugin_Continue;
    }

    if (g_onDeath != null)
    {
        ScoreDeathEvent deathEvent;

        deathEvent.toBroadcast = flags;
        deathEvent.victim = victim;
        deathEvent.victimScore = g_events[FOFLIB_NOTOMSG_SUICIDE].score;
        deathEvent.attacker = g_events[FOFLIB_NOTOMSG_KSKILL].players[0];
        deathEvent.attackScore = g_events[FOFLIB_NOTOMSG_KSKILL].score;
        deathEvent.killStreakBonus = g_events[FOFLIB_NOTOMSG_KSKILL].bonus;
        deathEvent.nemesisBonus = g_events[FOFLIB_NOTOMSG_NEMESIS].score;
        strcopy(deathEvent.weapon, sizeof(deathEvent.weapon), weapon);
        deathEvent.headshot = headshot;
        deathEvent.penetration = penetration;
        deathEvent.damagebits = damagebits;

        Call_StartForward(g_onDeath);
        Call_PushArrayEx(deathEvent, sizeof(deathEvent), SM_PARAM_COPYBACK);
        int error = Call_Finish();

        if (error != SP_ERROR_NONE)
        {
            LogError("Callback g_killForward failed! Error %d", error);
        }

        g_events[FOFLIB_NOTOMSG_SUICIDE].score = deathEvent.victimScore;
        g_events[FOFLIB_NOTOMSG_KSKILL].players[0] = deathEvent.attacker;
        g_events[FOFLIB_NOTOMSG_KSKILL].score = deathEvent.attackScore;
        g_events[FOFLIB_NOTOMSG_KSKILL].bonus = deathEvent.killStreakBonus;
        g_events[FOFLIB_NOTOMSG_NEMESIS].score = deathEvent.nemesisBonus;

        // Updating the event
        event.SetInt("userid", GetClientUserId(deathEvent.victim));
        if (g_events[FOFLIB_NOTOMSG_KSKILL].players[0] != 0)
            event.SetInt("attacker", GetClientUserId(g_events[FOFLIB_NOTOMSG_KSKILL].players[0]));
        if (g_events[FOFLIB_NOTOMSG_ASSIST].players[0] != 0)
            event.SetInt("assist", GetClientUserId(g_events[FOFLIB_NOTOMSG_ASSIST].players[0]));
        event.SetInt("weapon_index", weaponidx);
        event.SetString("weapon", weapon);
        event.SetBool("headshot", headshot);
        event.SetBool("penetration", penetration);
        event.SetInt("damagebits", damagebits);
    }

    // Swapping back kill if no ks
    if (g_events[FOFLIB_NOTOMSG_KSKILL].bonus == 0)
    {
        g_events[FOFLIB_NOTOMSG_KILL] = g_events[FOFLIB_NOTOMSG_KSKILL];
        g_events[FOFLIB_NOTOMSG_KILL].eventType = FOFLIB_NOTOMSG_KILL;
        g_events[FOFLIB_NOTOMSG_KSKILL].eventType = -1;
    }

    if (g_events[FOFLIB_NOTOMSG_SUICIDE].eventType == FOFLIB_NOTOMSG_SUICIDE)
    {
        g_events[FOFLIB_NOTOMSG_SUICIDE].players[1] = victim;
        g_events[FOFLIB_NOTOMSG_SUICIDE].numPlayers = 1;
    }

    g_dontTrack = true;

    // Sending and processing the messages
    if (_ValidEvent(FOFLIB_NOTOMSG_ASSIST, flags))
    {
        g_events[FOFLIB_NOTOMSG_ASSIST].Send();
    }

    if (_ValidEvent(FOFLIB_NOTOMSG_SUICIDE, flags))
    {
        g_events[FOFLIB_NOTOMSG_SUICIDE].Send();
    }
    else if (_ValidEvent(FOFLIB_NOTOMSG_KILL, flags))
    {
        g_events[FOFLIB_NOTOMSG_KILL].Send();
    }
    else if (_ValidEvent(FOFLIB_NOTOMSG_KSKILL, flags))
    {
        g_events[FOFLIB_NOTOMSG_KSKILL].Send();
    }

    if (_ValidEvent(FOFLIB_NOTOMSG_NEMESIS, flags))
    {
        g_events[FOFLIB_NOTOMSG_NEMESIS].Send();
    }

    g_dontTrack = false;

    g_events[FOFLIB_NOTOMSG_KILL].Zero();
    g_events[FOFLIB_NOTOMSG_KSKILL].Zero();
    g_events[FOFLIB_NOTOMSG_ASSIST].Zero();
    g_events[FOFLIB_NOTOMSG_NEMESIS].Zero();
    g_events[FOFLIB_NOTOMSG_SUICIDE].Zero();

    return Plugin_Changed;
}

static Action _Event_StorePlayerScore(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    if (!PlayerUtils_IsValidPlayer(client, false)) return Plugin_Continue;

    char steamid[32]; Format(steamid, sizeof(steamid), "%d", GetSteamAccountID(client));

    _PlayerScoreData ps;
    ps.kills = g_kills[client];
    ps.score = g_scores[client];    

    if (g_scoreStorage == null)
    {
        g_scoreStorage = new StringMap();
    }

    g_scoreStorage.SetArray(steamid, ps, sizeof(ps));

    return Plugin_Continue;
}

static Action _Event_RestorePlayerScore(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    if (!PlayerUtils_IsValidPlayer(client, false)) return Plugin_Continue;

    char steamid[32]; Format(steamid, sizeof(steamid), "%d", GetSteamAccountID(client));

    _PlayerScoreData ps;
    if (g_scoreStorage != null && g_scoreStorage.ContainsKey(steamid))
    {
        g_scoreStorage.GetArray(steamid, ps, sizeof(ps));

        g_kills[client] = ps.kills;    
        g_scores[client] = ps.score;

        g_scoreStorage.Remove(steamid);
    }
    else
    {
        g_kills[client] = 0;    
        g_scores[client] = 0;
    }

    if (g_scoreStorage != null && g_scoreStorage.Size == 0) { delete g_scoreStorage; g_scoreStorage = null; }

    return Plugin_Continue;
}



/**
 * Utilities
 */

static int _NotoFlagToInt(NotorietyFlags flag)
{
    switch (flag)
    {
        case Notoriety_Kill: { return FOFLIB_NOTOMSG_KILL; }
        case Notoriety_KillStreak: { return FOFLIB_NOTOMSG_KSKILL; }
        case Notoriety_Assist: { return FOFLIB_NOTOMSG_ASSIST; }
        case Notoriety_Nemesis: { return FOFLIB_NOTOMSG_NEMESIS; }
        case Notoriety_Gentleman: { return FOFLIB_NOTOMSG_GENTLEMAN; }
        case Notoriety_Capture: { return FOFLIB_NOTOMSG_CAPTURE; }
        case Notoriety_HatDrop: { return FOFLIB_NOTOMSG_HATDROP; }
        case Notoriety_Suicide: { return FOFLIB_NOTOMSG_SUICIDE; }
        case Notoriety_LootDelivery: { return FOFLIB_NOTOMSG_LOOTDELIVERY; }
    }

    return -1;
}

static bool _ValidEvent(int eventType, NotorietyFlags flags)
{
    if (g_events[eventType].eventType == -1) return false;
    return (1 << eventType) & view_as<int>(flags);
}