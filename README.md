# FoFLib

This hosts utility functions and modules for developing Fistful of Frags plugins.

# Presentable

[`playerutils.inc`](./playerutils.inc)
- Fetching and checking for players

[`configutils.inc`](./configutils.inc)
- Constants library for defining gamemode configs

[`basicplayerglow.inc`](./basicplayerglow.inc)
- Lets players glow white.

[`browsermode.inc`](./browsermode.inc)
- Requires [SteamWorks extension](https://forums.alliedmods.net/showthread.php?t=229556)
- Allows plugins to change the gamemode name to whatever they want or set it back to default.

[`callbackutils.inc`](./callbackutils.inc)
- Generic class for callback management, such as in damagemanager

[`convarmanager.inc`](./convarmanager.inc)
- Struct for easily storing and swapping large numbers of convars.
- Useful for a convar-reliant script being used by another script

[`damagemanager.inc`](./damagemanager.inc)
- Allows (multiple) custom damage functions that toggleable.

# Usable/Needs Work

[`blackout.inc`](./blackout.inc)
- Causes the player's screen to go black.
- Works, just lacks flexibility and should use an enum struct for the usermsg

[`customscorer.inc`](./customscorer.inc)
- Allows plugins to spoof player scores and implement custom, event-based scoring
- Uses `scoreboardmanager.inc` and `NotorietyMsg` from `usermsgs.inc`
- Works for shootout!!!!
- Needs more work to incorporate the extra Notoriety messages.

[`equipping.inc`](./equipping.inc)
- Used to manage player weapons and perks.
- Needs addition of ammo manipulation

[`fofweapons.inc`](./fofweapons.inc)
- Huge set of constants for identifying and working with weapons
- Needs to be un-methodmapped, cleaned up

[`playerhooks.inc`](./playerhooks.inc)
- Generic utility for managing SDKHooks for players.
- Needs to be un-methodmapped

[`scoreboardmanager.inc`](./scoreboardmanager.inc)
- Hooks into the scoreboard update

[`teams.inc`](./teams.inc)
- Allows plugins to change player teams.
- Needs cleanup

[`usermsgs.inc`](./usermsgs.inc)
- Neatly wraps user messages in enum structs.
- Existing messages work; there are many more messages not accounted for.

# Unfinished

[`backgroundmusic.inc`](./backgroundmusic.inc)
- Never really started
- Meant to facilitate starting/stopping/looping music
- Would have features to add different playlists and music preferences
 
[`botcount.inc`](./botcount.inc)
- Not working
- Meant to hard lock a number of bots on the server.
 
[`fofmenus.inc`](./fofmenus.inc)
- Untested, does not include crash safety for >10 items on a menu.

[`weapondropmanager.inc`](./weapondropmanager.inc)
- Not started

[`damagemanager2.inc`](./damagemanager2.inc)
- Like DamageManager1 but resting on top of `playerhooks.inc`
- Barely started

# Deprecated

[`customscorer_old_old.inc`]
[`customscorer_old.inc`]
[`scoreboardmanager_old_old.inc`]
[`scoreboardmanager_old.inc`]
[`scorelistener.inc`]
