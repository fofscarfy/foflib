#if defined _foflib_fofweapons_included
    #endinput
#endif
#define _foflib_fofweapons_included

enum
{
    FofWeapon_Invalid = -2,
    FofWeapon_Worldspawn = -1,
    FofWeapon_HorseRam = 0,
    FofWeapon_KickFall = 1,
    FofWeapon_Unknown2 = 2,
    FofWeapon_ThrownGun = 3,
    FofWeapon_Kick = 4,
    FofWeapon_Blast = 5,
    FofWeapon_Physics = 6,
    FofWeapon_BlackDynamiteKill = 7,
    FofWeapon_YellowDynamite = 8,
    FofWeapon_DynamiteKill = 9,
    FofWeapon_Arrow = 10,
    FofWeapon_BlackBowArrow = 11,
    FofWeapon_XBowArrow = 12,
    FofWeapon_ThrownKnife = 13,
    FofWeapon_ThrownAxe = 14,
    FofWeapon_ThrownMachete = 15,
    FofWeapon_Fists = 16,
    FofWeapon_BrassKnucklesKill = 17,
    FofWeapon_GhostGun = 18,
    FofWeapon_RightHanded = 19,
    FofWeapon_LeftHanded = 20,
    FofWeapon_Fanning = 21,
    FofWeapon_Ambidextrous = 22,
    FofWeapon_GunThrow = 23,
    FofWeapon_Jumpmaster = 24,
    FofWeapon_Slide = 25,
    FofWeapon_HeavyLoads = 26,
    FofWeapon_BrassKnuckles = 27,
    FofWeapon_Knife = 28,
    FofWeapon_Boots = 29,
    FofWeapon_Deringer = 30,
    FofWeapon_Dynamite = 31,
    FofWeapon_HeavyLoadsBox = 32,
    FofWeapon_Volcanic = 33,
    FofWeapon_ColtNavy = 34,
    FofWeapon_Hatchet = 35,
    FofWeapon_Bow = 36,
    FofWeapon_SawedOffShotgun = 37,
    FofWeapon_Hammerless = 38,
    FofWeapon_RemingtonArmy = 39,
    FofWeapon_MaresLeg = 40,
    FofWeapon_Schofield = 41,
    FofWeapon_SmithCarbine = 42,
    FofWeapon_Peacemaker = 43,
    FofWeapon_BlackBow = 44,
    FofWeapon_Yellowboy = 45,
    FofWeapon_Coachgun = 46,
    FofWeapon_SpencerCarbine = 47,
    FofWeapon_Machete = 48,
    FofWeapon_PumpShotgun = 49,
    FofWeapon_BlackDynamite = 50,
    FofWeapon_SharpsRifle = 51,
    FofWeapon_ColtWalker = 52,
    FofWeapon_DynamiteBelt = 53,
    FofWeapon_PortableWhiskey = 54,
    FofWeapon_XBow = 55,
    FofWeapon_Mauser = 56,
    FofWeapon_GhostFists = 57,
    FofWeapon_Tank = 58,
    FofWeapon_Cannonball = 59
}

static const int equippables[40] = 
{
    FofWeapon_GhostGun,
    FofWeapon_RightHanded,
    FofWeapon_LeftHanded,
    FofWeapon_Fanning,
    FofWeapon_Ambidextrous,
    FofWeapon_GunThrow,
    FofWeapon_Jumpmaster,
    FofWeapon_Slide,
    FofWeapon_HeavyLoads,
    FofWeapon_BrassKnuckles,
    FofWeapon_Knife,
    FofWeapon_Boots,
    FofWeapon_Deringer,
    FofWeapon_Dynamite,
    FofWeapon_HeavyLoadsBox,
    FofWeapon_Volcanic,
    FofWeapon_ColtNavy,
    FofWeapon_Hatchet,
    FofWeapon_Bow,
    FofWeapon_SawedOffShotgun,
    FofWeapon_Hammerless,
    FofWeapon_RemingtonArmy,
    FofWeapon_MaresLeg,
    FofWeapon_Schofield,
    FofWeapon_SmithCarbine,
    FofWeapon_Peacemaker,
    FofWeapon_BlackBow,
    FofWeapon_Yellowboy,
    FofWeapon_Coachgun,
    FofWeapon_SpencerCarbine,
    FofWeapon_Machete,
    FofWeapon_PumpShotgun,
    FofWeapon_BlackDynamite,
    FofWeapon_SharpsRifle,
    FofWeapon_ColtWalker,
    FofWeapon_DynamiteBelt,
    FofWeapon_PortableWhiskey,
    FofWeapon_XBow,
    FofWeapon_Mauser,
    FofWeapon_GhostFists
}

static const int HasKillIcon[37] =
{
    FofWeapon_HorseRam,
    FofWeapon_KickFall,
    FofWeapon_ThrownGun,
    FofWeapon_Kick,
    FofWeapon_Blast,
    FofWeapon_Physics,
    FofWeapon_BlackDynamiteKill,
    FofWeapon_YellowDynamite,
    FofWeapon_DynamiteKill,
    FofWeapon_Arrow,
    FofWeapon_BlackBowArrow,
    FofWeapon_XBowArrow,
    FofWeapon_ThrownKnife,
    FofWeapon_ThrownAxe,
    FofWeapon_ThrownMachete,
    FofWeapon_Fists,
    FofWeapon_BrassKnucklesKill,
    FofWeapon_Knife,
    FofWeapon_Deringer,
    FofWeapon_Volcanic,
    FofWeapon_ColtNavy,
    FofWeapon_Hatchet,
    FofWeapon_Bow,
    FofWeapon_SawedOffShotgun,
    FofWeapon_Hammerless,
    FofWeapon_RemingtonArmy,
    FofWeapon_MaresLeg,
    FofWeapon_Schofield,
    FofWeapon_SmithCarbine,
    FofWeapon_Peacemaker,
    FofWeapon_Yellowboy,
    FofWeapon_Coachgun,
    FofWeapon_SpencerCarbine,
    FofWeapon_PumpShotgun,
    FofWeapon_SharpsRifle,
    FofWeapon_ColtWalker
}

static const char killstrings[60][20] = 
{
    "",
    "worldspawn",
    "horse-ram",
    "kick-fall",
    "",
    "thrown_gun",
    "kick",
    "blast",
    "physics",
    "dynamite_black",
    "dynamite_yellow",
    "dynamite",
    "arrow",
    "arrow_black",
    "x-arrow",
    "thrown_knife",
    "thrown_axe",
    "thrown_machete",
    "fists",
    "fists_brass",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "knife",
    "",
    "deringer",
    "dynamite",
    "",
    "volcanic",
    "coltnavy",
    "axe",
    "bow",
    "sawedoff_shotgun",
    "hammerless",
    "remington_army",
    "maresleg",
    "schofield",
    "carbine",
    "peacemaker",
    "bow_black",
    "henryrifle",
    "coachgun",
    "spencer",
    "machete",
    "shotgun",
    "dynamite_black",
    "sharps",
    "walker",
    "dynamite_belt",
    "whiskey",
    "xbow",
    "mauser",
    "fists_ghost"
}

static const char entname[60][32] =
{
    "",
    "",
    "fof_horse",
    "",
    "",
    "",
    "",
    "",
    "",
    "weapon_dynamite_black",
    "weapon_dynamite_belt",
    "weapon_dynamite",
    "arrow",
    "arrow_black",
    "xarrow",
    "thrown_knife",
    "thrown_axe",
    "thrown_machete"
}

methodmap FofWeapon
{
    public static int FromLoadoutNumber(int number)
    {
        int value = FofWeapon_Invalid;

        switch (number)
        {
            case 1: value = FofWeapon_Knife;
            case 2: value = FofWeapon_ColtNavy;
            case 3: value = FofWeapon_Deringer;
            case 4: value = FofWeapon_Yellowboy;
            case 5: value = FofWeapon_SmithCarbine;
            case 6: value = FofWeapon_Coachgun;
            case 7: value = FofWeapon_Dynamite;
            case 11: value = FofWeapon_Bow;
            case 12: value = FofWeapon_SharpsRifle;
            case 13: value = FofWeapon_SawedOffShotgun;
            case 14: value = FofWeapon_GunThrow;
            case 15: value = FofWeapon_Jumpmaster;
            case 16: value = FofWeapon_PortableWhiskey;
            case 17: value = FofWeapon_DynamiteBelt;
            case 18: value = FofWeapon_SpencerCarbine;
            case 19: value = FofWeapon_Hatchet;
            case 20: value = FofWeapon_Boots;
            case 21: value = FofWeapon_Peacemaker;
            case 22: value = FofWeapon_Hammerless;
            case 23: value = FofWeapon_Slide;
            case 24: value = FofWeapon_BlackBow;
            case 25: value = FofWeapon_HeavyLoads;
            case 26: value = FofWeapon_Machete;
            case 27: value = FofWeapon_BlackDynamite;
            case 28: value = FofWeapon_Volcanic;
            case 29: value = FofWeapon_PumpShotgun;
            case 30: value = FofWeapon_RemingtonArmy;
            case 31: value = FofWeapon_Schofield;
            case 32: value = FofWeapon_ColtWalker;
            case 33: value = FofWeapon_MaresLeg;
            case 34: value = FofWeapon_BrassKnuckles;
            case 41: value = FofWeapon_RightHanded;
            case 42: value = FofWeapon_LeftHanded;
            case 43: value = FofWeapon_Ambidextrous;
            case 44: value = FofWeapon_Fanning;
        }

        return value;
    }

    public static int ToLoadoutNumber(int weapon)
    {
        weapon = FofWeapon.GetEquipable(weapon);

        int number = -1;

        switch (weapon)
        {
            case FofWeapon_Knife: number = 1;
            case FofWeapon_ColtNavy: number = 2;
            case FofWeapon_Deringer: number = 3;
            case FofWeapon_Yellowboy: number = 4;
            case FofWeapon_SmithCarbine: number = 5;
            case FofWeapon_Coachgun: number = 6;
            case FofWeapon_Dynamite: number = 7;
            case FofWeapon_Bow: number = 11;
            case FofWeapon_SharpsRifle: number = 12;
            case FofWeapon_SawedOffShotgun: number = 13;
            case FofWeapon_GunThrow: number = 14;
            case FofWeapon_Jumpmaster: number = 15;
            case FofWeapon_PortableWhiskey: number = 16;
            case FofWeapon_DynamiteBelt: number = 17;
            case FofWeapon_SpencerCarbine: number = 18;
            case FofWeapon_Hatchet: number = 19;
            case FofWeapon_Boots: number = 20;
            case FofWeapon_Peacemaker: number = 21;
            case FofWeapon_Hammerless: number = 22;
            case FofWeapon_Slide: number = 23;
            case FofWeapon_BlackBow: number = 24;
            case FofWeapon_HeavyLoads: number = 25;
            case FofWeapon_Machete: number = 26;
            case FofWeapon_BlackDynamite: number = 27;
            case FofWeapon_Volcanic: number = 28;
            case FofWeapon_PumpShotgun: number = 29;
            case FofWeapon_RemingtonArmy: number = 30;
            case FofWeapon_Schofield: number = 31;
            case FofWeapon_ColtWalker: number = 32;
            case FofWeapon_MaresLeg: number = 33;
            case FofWeapon_BrassKnuckles: number = 34;
            case FofWeapon_RightHanded: number = 41;
            case FofWeapon_LeftHanded: number = 42;
            case FofWeapon_Ambidextrous: number = 43;
            case FofWeapon_Fanning: number = 44;
        }

        return number;
    }

    public static int GetEquipable(int weapon)
    {
        if (FofWeapon_GhostGun <= weapon && weapon <= FofWeapon_GhostFists) return weapon;
        switch (weapon)
        {
            case FofWeapon_Kick: return FofWeapon_Boots;
            case FofWeapon_BlackDynamiteKill: weapon = FofWeapon_BlackDynamite;
            case FofWeapon_YellowDynamite: weapon = FofWeapon_DynamiteBelt;
            case FofWeapon_DynamiteKill: weapon = FofWeapon_Dynamite;
            case FofWeapon_Arrow: weapon = FofWeapon_Bow;
            case FofWeapon_BlackBowArrow: weapon = FofWeapon_BlackBow;
            case FofWeapon_XBowArrow: weapon = FofWeapon_XBow;
            case FofWeapon_ThrownKnife: weapon = FofWeapon_Knife;
            case FofWeapon_ThrownAxe: weapon = FofWeapon_Hatchet;
            case FofWeapon_ThrownMachete: weapon = FofWeapon_Machete;
            case FofWeapon_Fists: weapon = FofWeapon_Fists;
            case FofWeapon_BrassKnucklesKill: weapon = FofWeapon_BrassKnuckles;
            default: weapon = FofWeapon_Invalid;
        }

        return weapon;
    }

    public static int GetKillFeedable(int weapon)
    {
        switch (weapon)
        {
            case FofWeapon_RightHanded, FofWeapon_LeftHanded,
                 FofWeapon_Ambidextrous, FofWeapon_Fanning,
                 FofWeapon_GhostGun, FofWeapon_Jumpmaster,
                 FofWeapon_Slide, FofWeapon_HeavyLoads,
                 FofWeapon_PortableWhiskey:
                weapon = FofWeapon_Invalid;
            case FofWeapon_Boots: weapon = FofWeapon_Kick;
            case FofWeapon_Bow: weapon = FofWeapon_Arrow;
            case FofWeapon_BlackBow: weapon = FofWeapon_BlackBowArrow;
            case FofWeapon_XBow: weapon = FofWeapon_XBowArrow;
            case FofWeapon_Dynamite: weapon = FofWeapon_DynamiteKill;
            case FofWeapon_BlackDynamite: weapon = FofWeapon_DynamiteKill;
            case FofWeapon_DynamiteBelt: weapon = FofWeapon_YellowDynamite;
            case FofWeapon_GunThrow: weapon = FofWeapon_ThrownGun;
            case FofWeapon_BrassKnuckles: weapon = FofWeapon_BrassKnucklesKill;
        }

        return weapon;
    }

    public static bool HasMenuIcon(int weapon)
    {
        return FofWeapon_GhostGun <= weapon && weapon <= FofWeapon_XBow;
    }

    public static bool IsEquippable(int weapon)
    {
        return FofWeapon_GhostGun <= weapon && weapon <= FofWeapon_GhostFists;
    }

    public static int GetEquippables(int[] arr, int size)
    {
        int wrote;
        for (wrote = 0; wrote < size && wrote < sizeof(equippables); wrote++)
        {
            arr[wrote] = equippables[wrote];
        }

        return wrote;
    }

    public static int GetKillfeedables(int[] arr, int size)
    {
        int wrote;
        for (wrote = 0; wrote < size && wrote < sizeof(HasKillIcon); wrote++)
        {
            arr[wrote] = HasKillIcon[wrote];
        }

        return wrote;
    }

    public static int GetKillString(int weapon, char[] killstring, int size)
    {
        if (weapon < -2 || weapon > sizeof(killstrings) - 2) return -1;

        int copied = strcopy(killstring, size, killstrings[weapon + 2]);

        return copied;
    }
}