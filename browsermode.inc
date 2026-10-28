#if defined _foflib_browsermode_included
    #endinput
#endif
#define _foflib_browsermode_included

// Requires SteamWorks extension
// https://github.com/KyleSanderson/SteamWorks

#include <../include/SteamWorks>
#include "configutils.inc"

// Sets the game mode in the server browser.
//
// @param name          The game mode string visible in the server browser.
stock void FofLib_SetBrowserGamemode(const char[] name)
{
    SteamWorks_SetGameDescription(name);
}

// Resets the game mode string in the server browser based on the 
// current server convars.
stock void FofLib_RestoreBrowserGamemode()
{
    int mode = FindConVar("fof_sv_currentmode").IntValue;
    char gamemode[64];
    switch (mode)
    {
        case FofGamemode_Shootout:
        {
            if (FindConVar("mp_teamplay").IntValue == 0)
            {
                gamemode = "Shootout";
            }
            else
            {
                Format(gamemode, sizeof(gamemode), "%d Teams Shootout", 
                       FindConVar("fof_sv_maxteams").IntValue);
            }
        }
        case FofGamemode_Teamplay:
        {
            gamemode = "Teamplay";
        }
        case FofGamemode_BreakBad:
        {
            gamemode = "Break Bad";
        }
        case FofGamemode_Elimination:
        {
            gamemode = "Elimination";
        }
        case FofGamemode_Versus:
        {
            gamemode = "Versus";
        }
        case FofGamemode_Coop:
        {
            gamemode = "Course Mode";
        }
    }

    SteamWorks_SetGameDescription(gamemode);
}