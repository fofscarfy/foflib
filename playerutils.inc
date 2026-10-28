#if defined _foflib_playerutils_included
    #endinput
#endif
#define _foflib_playerutils_included

#define TEAM_SPECTATOR 1

#define PLAYERUTILS_VALID 0
#define PLAYERUTILS_LIVING 1
#define PLAYERUTILS_ACTIVE 2

// Checks if a player is in the game.
//
// @param client            The player to target.
// @param allowBots         If true, bots are considered valid.
// @return                  If the player is in the game.
stock bool PlayerUtils_IsValidPlayer(int client, bool allowBots=true)
{ return _IsPlayer(PLAYERUTILS_VALID, client, allowBots); }

// Checks if a player is in the game and alive.
//
// @param client            The player to target.
// @param allowBots         If true, bots are considered valid.
// @return                  If the player is in the game and alive.
stock bool PlayerUtils_IsLivingPlayer(int client, bool allowBots=true)
{ return _IsPlayer(PLAYERUTILS_LIVING, client, allowBots); }

// Checks if a player is in the game and not on the spectator team.
//
// @param client            The player to target.
// @param allowBots         If true, bots are considered valid.
// @return                  If the player is in the game and not spectating.
stock bool PlayerUtils_IsActivePlayer(int client, bool allowBots=true)
{ return _IsPlayer(PLAYERUTILS_ACTIVE, client, allowBots); }

// Iterator function to cycle through valid players.
// 
// Iterator usage: "for (int c = 0; PlayerUtils_GetNextValidPlayer(c);) {}"
//
// @param start             Search starts at the index after start.
// @param allowBots         If true, bots are considered valid.
// @return                  Whether or not a next player was found.
stock bool PlayerUtils_GetNextValidPlayer(int &start, bool allowBots=true)
{ return _GetNextPlayer(PLAYERUTILS_VALID, start, allowBots); }

// Iterator function to cycle through living players.
// 
// Iterator usage: "for (int c = 0; PlayerUtils_GetNextLivingPlayer(c);) {}"
//
// @param start             Search starts at the index after start.
// @param allowBots         If true, bots are considered valid.
// @return                  Whether or not a next player was found.
stock bool PlayerUtils_GetNextLivingPlayer(int &start, bool allowBots=true)
{ return _GetNextPlayer(PLAYERUTILS_LIVING, start, allowBots); }

// Iterator function to cycle through active players.
// 
// Iterator usage: "for (int c = 0; PlayerUtils_GetNextActivePlayer(c);) {}"
//
// @param start             Search starts at the index after start.
// @param allowBots         If true, bots are considered valid.
// @return                  Whether or not a next player was found.
stock bool PlayerUtils_GetNextActivePlayer(int &start, bool allowBots=true)
{ return _GetNextPlayer(PLAYERUTILS_ACTIVE, start, allowBots); }

// Populates a list with valid players.
//
// @param clients           The list to populate with valid players.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied valid players.
stock int PlayerUtils_GetValidPlayers(int[] clients, int size, bool allowBots=true)
{ return _GetPlayers(PLAYERUTILS_VALID, clients, size, allowBots); }

// Populates a list with living players.
//
// @param clients           The list to populate with living players.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied living players.
stock int PlayerUtils_GetLivingPlayers(int[] clients, int size, bool allowBots=true)
{ return _GetPlayers(PLAYERUTILS_LIVING, clients, size, allowBots); }

// Populates a list with active players.
//
// @param clients           The list to populate with active players.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied active players.
stock int PlayerUtils_GetActivePlayers(int[] clients, int size, bool allowBots=true)
{ return _GetPlayers(PLAYERUTILS_ACTIVE, clients, size, allowBots); }

// Returns the number of currently valid players.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The number of valid players.
stock int PlayerUtils_GetNumValidPlayers(bool allowBots=true)
{ return _GetNumPlayers(PLAYERUTILS_VALID, allowBots); }

// Returns the number of currently living players.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The number of living players.
stock int PlayerUtils_GetNumLivingPlayers(bool allowBots=true)
{ return _GetNumPlayers(PLAYERUTILS_LIVING, allowBots); }

// Returns the number of currently active players.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The number of active players.
stock int PlayerUtils_GetNumActivePlayers(bool allowBots=true)
{ return _GetNumPlayers(PLAYERUTILS_ACTIVE, allowBots); }

// Returns a random valid player. If no valid players, returns -1.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The index of a random valid player, or -1 
//                          if there are none.
stock int PlayerUtils_GetRandomValidPlayer(bool allowBots=true)
{ return _GetRandomPlayer(PLAYERUTILS_VALID, allowBots); }

// Returns a random living player. If no living players, returns -1.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The index of a random living player, or -1 
//                          if there are none.
stock int PlayerUtils_GetRandomLivingPlayer(bool allowBots=true)
{ return _GetRandomPlayer(PLAYERUTILS_LIVING, allowBots); }

// Returns a random active player. If no active players, returns -1.
//
// @param allowBots         If true, bots are considered valid.
// @return                  The index of a random active player, or -1 
//                          if there are none.
stock int PlayerUtils_GetRandomActivePlayer(bool allowBots=true)
{ return _GetRandomPlayer(PLAYERUTILS_ACTIVE, allowBots); }

// Populates a list with shuffled valid players.
//
// @param clients           The list to populate with valid players 
//                          in random order.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied valid players.
stock int PlayerUtils_GetShuffledValidPlayers(int[] clients, int size, bool allowBots=true)
{ return _GetShuffledPlayers(PLAYERUTILS_VALID, clients, size, allowBots); }

// Populates a list with shuffled living players.
//
// @param clients           The list to populate with living players 
//                          in random order.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied living players.
stock int PlayerUtils_GetShuffledLivingPlayers(int[] clients, int size, bool allowBots=true)
{ return _GetShuffledPlayers(PLAYERUTILS_LIVING, clients, size, allowBots); }

// Populates a list with shuffled active players.
//
// @param clients           The list to populate with active players 
//                          in random order.
// @param size              The maximum size of clients.
// @param allowBots         If true, bots are considered valid.
// @return                  The number of copied active players.
stock int PlayerUtils_GetShuffledActivePlayers(int[] clients, int size, bool allowBots=true)
{ return _GetShuffledPlayers(PLAYERUTILS_ACTIVE, clients, size, allowBots); }



static bool _IsPlayer(int type, int client, bool allowBots=true)
{
    if (client < 1 || client > MaxClients || !IsValidEntity(client)) return false;
    if (!IsClientInGame(client)) return false;
    if (!allowBots && IsFakeClient(client)) return false;
    switch (type)
    {
        case PLAYERUTILS_VALID: { return true; }
        case PLAYERUTILS_LIVING: { return IsPlayerAlive(client); }
        case PLAYERUTILS_ACTIVE: { return GetClientTeam(client) != TEAM_SPECTATOR; }
    }

    return true;
}

static bool _GetNextPlayer(int type, int &start, bool allowBots=true)
{
    if (start < 0) start = 0;
    while (++start <= MaxClients) { if (_IsPlayer(type, start, allowBots)) return true; }

    start = -1;
    return false;
}

static int _GetPlayers(int type, int[] clients, int size, bool allowBots=true)
{
    int count = 0;
    for (int c = 0; _GetNextPlayer(type, c, allowBots) && count < size; count++) clients[count] = c;

    return count;
}

static int _GetNumPlayers(int type, bool allowBots=true)
{
    int count = 0;
    for (int c = 0; _GetNextPlayer(type, c, allowBots);) count++;

    return count;
}

static int _GetRandomPlayer(int type, bool allowBots=true)
{
    int clients[MAXPLAYERS + 1];
    int numClients = _GetPlayers(type, clients, sizeof(clients), allowBots);
    if (numClients == 0) return -1;

    return clients[GetRandomInt(0, numClients - 1)];
}

static int _GetShuffledPlayers(int type, int[] clients, int size, bool allowBots=true)
{
    int tmp[MAXPLAYERS + 1];
    int numClients = _GetPlayers(type, tmp, sizeof(tmp), allowBots);
    if (numClients == 0) return -1;
    _FisherYates(tmp, numClients);

    for (int i = 0; i < size && i < numClients; i++) clients[i] = tmp[i];
    return size < numClients ? size : numClients;
}

static void _FisherYates(int[] clients, int size)
{
    int tmp, rand;
    for (int i = 0; i < size - 1; i++)
    {
        rand = GetRandomInt(i, size - 1);
        tmp = clients[i];
        clients[i] = clients[rand];
        clients[rand] = tmp;
    }
}

#undef TEAM_SPECTATOR

#undef PLAYERUTILS_VALID
#undef PLAYERUTILS_LIVING
#undef PLAYERUTILS_ACTIVE