/**
 * User Message Definitions
 */
#define FOFLIB_USERMSG_GEIGER               view_as<UserMsg>(0)     // "Geiger"
#define FOFLIB_USERMSG_TRAIN                view_as<UserMsg>(1)     // "Train"
#define FOFLIB_USERMSG_HUDTEXT              view_as<UserMsg>(2)     // "HudText"
#define FOFLIB_USERMSG_SAYTEXT              view_as<UserMsg>(3)     // "SayText"
#define FOFLIB_USERMSG_SAYTEXT2             view_as<UserMsg>(4)     // "SayText2"
#define FOFLIB_USERMSG_TEXTMSG              view_as<UserMsg>(5)     // "TextMsg"
#define FOFLIB_USERMSG_HUDMSG               view_as<UserMsg>(6)     // "HudMsg"
#define FOFLIB_USERMSG_RESETHUD             view_as<UserMsg>(7)     // "ResetHUD"
#define FOFLIB_USERMSG_GAMETITLE            view_as<UserMsg>(8)     // "GameTitle"
#define FOFLIB_USERMSG_ITEMPICKUP           view_as<UserMsg>(9)     // "ItemPickup"
#define FOFLIB_USERMSG_SHOWMENU             view_as<UserMsg>(10)    // "ShowMenu"
#define FOFLIB_USERMSG_SHAKE                view_as<UserMsg>(11)    // "Shake"
#define FOFLIB_USERMSG_FADE                 view_as<UserMsg>(12)    // "Fade"
#define FOFLIB_USERMSG_VGUIMENU             view_as<UserMsg>(13)    // "VGUIMenu"
#define FOFLIB_USERMSG_RUMBLE               view_as<UserMsg>(14)    // "Rumble"
#define FOFLIB_USERMSG_BATTERY              view_as<UserMsg>(15)    // "Battery"
#define FOFLIB_USERMSG_DAMAGE               view_as<UserMsg>(16)    // "Damage"
#define FOFLIB_USERMSG_VOICEMASK            view_as<UserMsg>(17)    // "VoiceMask"
#define FOFLIB_USERMSG_REQUESTSTATE         view_as<UserMsg>(18)    // "RequestState"
#define FOFLIB_USERMSG_CLOSECAPTION         view_as<UserMsg>(19)    // "CloseCaption"
#define FOFLIB_USERMSG_HINTTEXT             view_as<UserMsg>(20)    // "HintText"
#define FOFLIB_USERMSG_KEYHINTTEXT          view_as<UserMsg>(21)    // "KeyHintText"
#define FOFLIB_USERMSG_SQUADMEMBERDIED      view_as<UserMsg>(22)    // "SquadMemberDied"
#define FOFLIB_USERMSG_AMMODENIED           view_as<UserMsg>(23)    // "AmmoDenied"
#define FOFLIB_USERMSG_CREDITSMSG           view_as<UserMsg>(24)    // "CredistMsg"
#define FOFLIB_USERMSG_LOGOTIMEMSG          view_as<UserMsg>(25)    // "LogoTimeMsg"
#define FOFLIB_USERMSG_ACHIEVEMENTEVENT     view_as<UserMsg>(26)    // "AchievementEvent"
#define FOFLIB_USERMSG_UPDATEJALOPYRADAR    view_as<UserMsg>(27)    // "UpdateJalopyRadar"
#define FOFLIB_USERMSG_ICONCOMM             view_as<UserMsg>(28)    // "IconComm"
#define FOFLIB_USERMSG_HUDCIRCLEPROGRESSBAR view_as<UserMsg>(29)    // "HudCircleProgressBar"
#define FOFLIB_USERMSG_CAPMESSAGE           view_as<UserMsg>(30)    // "CapMessage"
#define FOFLIB_USERMSG_SHOWMENUFOF          view_as<UserMsg>(31)    // "ShowMenuFoF"
#define FOFLIB_USERMSG_HITRECON             view_as<UserMsg>(32)    // "HitRecon"
#define FOFLIB_USERMSG_HITRECONRANK         view_as<UserMsg>(33)    // "HitReconRank"
#define FOFLIB_USERMSG_FOFHINT              view_as<UserMsg>(34)    // "FoFHint"
#define FOFLIB_USERMSG_CASH                 view_as<UserMsg>(35)    // "Cash"
#define FOFLIB_USERMSG_NOTORIETY            view_as<UserMsg>(36)    // "Notoriety"
#define FOFLIB_USERMSG_HUDBBMULTI           view_as<UserMsg>(37)    // "HUDBBMulti"
#define FOFLIB_USERMSG_BBNOTICES            view_as<UserMsg>(38)    // "BBNotices"
#define FOFLIB_USERMSG_FOFSLIDE             view_as<UserMsg>(39)    // "FoFSlide"
#define FOFLIB_USERMSG_HITBOW               view_as<UserMsg>(40)    // "HitBow"
#define FOFLIB_USERMSG_GOODBADYOU           view_as<UserMsg>(41)    // "GoodBadYou"
#define FOFLIB_USERMSG_HUDEQUIPITEMS        view_as<UserMsg>(42)    // "HudEquipItems"
#define FOFLIB_USERMSG_COURSEHINT           view_as<UserMsg>(43)    // "CourseHint"
#define FOFLIB_USERMSG_MAXHP                view_as<UserMsg>(44)    // "MaxHP"
#define FOFLIB_USERMSG_SPHAPWEAPEVENT       view_as<UserMsg>(45)    // "SPHapWeapEvent"
#define FOFLIB_USERMSG_HAPDMG               view_as<UserMsg>(46)    // "HapDmg"
#define FOFLIB_USERMSG_HAPPUNCH             view_as<UserMsg>(47)    // "HapPunch"
#define FOFLIB_USERMSG_HAPSETDRAG           view_as<UserMsg>(48)    // "HapSetDrag"
#define FOFLIB_USERMSG_HAPSETCONST          view_as<UserMsg>(49)    // "HapSetConst"
#define FOFLIB_USERMSG_HAPMELEECONTACT      view_as<UserMsg>(50)    // "HapMeleeContact"




/**
 * HudMsg (6)
 */

#define FOFLIB_HUDMSG_MINSIZE 35
#define FOFLIB_HUDMSG_NUMCHANNELS 6

enum struct HudMsg
{
    int channel;
    float x;
    float y;
    int color1[4];
    int color2[4];
    int effect;
    float fadein;
    float fadeout;
    float holdtime;
    float fxtime;
    char text[256];

    int numPlayers;
    int players[33];

    bool Decode(BfRead message)
    {
        if (message.BytesLeft < FOFLIB_HUDMSG_MINSIZE) return false;
        
        this.channel = message.ReadByte();

        this.x = message.ReadFloat();
        this.y = message.ReadFloat();
        
        this.color1[0] = message.ReadByte();
        this.color1[1] = message.ReadByte();
        this.color1[2] = message.ReadByte();
        this.color1[3] = message.ReadByte();

        this.color2[0] = message.ReadByte();
        this.color2[1] = message.ReadByte();
        this.color2[2] = message.ReadByte();
        this.color2[3] = message.ReadByte();

        this.effect = message.ReadByte();
        
        this.fadein = message.ReadFloat();
        this.fadeout = message.ReadFloat();

        this.holdtime = message.ReadFloat();

        this.fxtime = message.ReadFloat();

        int strSize = message.BytesLeft < sizeof(this.text) ? message.BytesLeft : sizeof(this.text);

        message.ReadString(this.text, strSize);

        return true;
    }

    void Zero()
    {
        this.channel = 0;
        this.x = 0.0;
        this.y = 0.0;
        this.color1 = {0, 0, 0, 0};
        this.color2 = {0, 0, 0, 255};
        this.effect = 0;
        this.fadein = 0.0;
        this.fadeout = 0.0;
        this.holdtime = 0.0;
        this.fxtime = 0.0;
        this.text = "";

        this.numPlayers = 0;
        for (int i = 0; i < sizeof(this.players); i++) this.players[i] = 0;
    }

    bool Send()
    {
        if (this.channel < 0 || this.channel >= FOFLIB_HUDMSG_NUMCHANNELS) return false;

        BfWrite message = UserMessageToBfWrite(StartMessage("HudText", this.players, this.numPlayers));
        message.WriteByte(this.channel);

        message.WriteFloat(this.x);
        message.WriteFloat(this.y);

        message.WriteByte(this.color1[0]);
        message.WriteByte(this.color1[1]);
        message.WriteByte(this.color1[2]);
        message.WriteByte(this.color1[3]);

        message.WriteByte(this.color2[0]);
        message.WriteByte(this.color2[1]);
        message.WriteByte(this.color2[2]);
        message.WriteByte(this.color2[3]);

        message.WriteByte(this.effect);

        message.WriteFloat(this.fadein);
        message.WriteFloat(this.fadeout);

        message.WriteFloat(this.holdtime);

        message.WriteFloat(this.fxtime);

        message.WriteString(this.text);

        return true;
    }
}



/**
 * "Fade" (12)
 */

enum struct FadeMsg
{
    int duration;
    int flags;
    char r;
    char g;
    char b;
    char a;

    int numPlayers;
    int players[33];

    void SetColor(char r, char g, char b, char a)
    {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    bool Send()
    {
        BfWrite message = UserMessageToBfWrite(StartMessage("Fade", this.players, this.numPlayers));

        message.WriteShort(this.duration);
        message.WriteShort(0);
        message.WriteShort(this.flags);

        message.WriteByte(this.r);
        message.WriteByte(this.g);
        message.WriteByte(this.b);
        message.WriteByte(this.a);

        EndMessage();

        return true;
    }
}



/**
 * "ShowMenuFoF" (31)
 */

// Send up to/including 10 before client crash
// Consider pairing this with a disable timer or hook on chest opens to prevent said crash

enum struct ShowMenuFoFMsg
{
    char label[64];
    bool final;
    int command;

    int numPlayers;
    int players[33];

    bool Decode(BfRead message)
    {
        message.ReadString(this.label, sizeof(this.label));
        while (message.BytesLeft > 3) message.ReadByte();
        this.final = message.ReadByte() > 0;
        this.command = message.ReadShort();

        return true;
    }

    bool Send()
    {
        BfWrite message = UserMessageToBfWrite(StartMessage("ShowMenuFoF", this.players, this.numPlayers));

        message.WriteString(this.label);
        message.WriteByte(this.final ? 0 : 1);
        message.WriteShort(this.command);

        EndMessage();

        return true;
    }

    void Zero()
    {
        ShowMenuFoFMsg empty;
        this = empty;
    }

    bool SendClear()
    {
        BfWrite message = UserMessageToBfWrite(StartMessage("ShowMenuFoF", this.players, this.numPlayers));

        message.WriteNum(0xFC190000);

        EndMessage();

        return true;
    }
}


/**
 * "Notoriety" (39)
 */

#define FOFLIB_NOTOMSG_MINSIZE 6
#define FOFLIB_NOTOMSG_NUMTYPES 9

#define FOFLIB_NOTOMSG_KILL 0x0
#define FOFLIB_NOTOMSG_KSKILL 0x1
#define FOFLIB_NOTOMSG_ASSIST 0x2
#define FOFLIB_NOTOMSG_NEMESIS 0x3
#define FOFLIB_NOTOMSG_GENTLEMAN 0x4
#define FOFLIB_NOTOMSG_CAPTURE 0x5
#define FOFLIB_NOTOMSG_HATDROP 0x6
#define FOFLIB_NOTOMSG_SUICIDE 0x7
#define FOFLIB_NOTOMSG_LOOTDELIVERY 0x8
#define FOFLIB_NOTOMSG_TOKEN_GREEN 0x9
#define FOFLIB_NOTOMSG_TOKEN_RED 0xA
#define FOFLIB_NOTOMSG_WEAPONGIFT 0xB
#define FOFLIB_NOTOMSG_TOKEN_OLIVE 0xC
#define FOFLIB_NOTOMSG_TOKEN_OLIVE2 0xD // idk what the difference is
#define FOFLIB_NOTOMSG_LEADERDEFEATED 0xE

enum struct NotorietyMsg
{
    int eventType;
    int score;
    int bonus;
    int players[33];
    int numPlayers;
    
    // Uses localization tokens and gets 1 string argument for the points.
    char text[256];

    bool Decode(BfRead message)
    {
        this.eventType = -1;
        if (message.BytesLeft < FOFLIB_NOTOMSG_MINSIZE) return false;
        this.eventType = message.ReadByte();
        this.score = message.ReadNum();
        this.bonus = message.ReadByte();

        if (message.BytesLeft == 0) return true;
        int min = message.BytesLeft < sizeof(this.text) ? message.BytesLeft : sizeof(this.text);
        message.ReadString(this.text, min);

        return true;
    }

    void Zero(bool onlyIfInvalid=false)
    {
        if (onlyIfInvalid && this.eventType != -1) return;
        this.eventType = -1;
        this.score = 0;
        this.bonus = 0;
        this.numPlayers = 0;
        for (int i = 0; i < sizeof(this.players); i++) this.players[i] = 0;
    }

    bool Send()
    {
        if (this.eventType < 0 || this.eventType >= FOFLIB_NOTOMSG_NUMTYPES) return false;

        BfWrite message = UserMessageToBfWrite(StartMessage("Notoriety", this.players, this.numPlayers));
        message.WriteByte(this.eventType);
        message.WriteNum(this.score);
        message.WriteByte(this.bonus);
        if (strlen(this.text) > 0)
        {
            message.WriteString(this.text);
        }

        EndMessage();

        return true;
    }
}


/**
 * HudEquipItems
 */

// Seems to only be usable in versus mode

#define FOFLIB_HUDEQUIPITEMSMSG_SIZE 2

enum struct HudEquipItemsMsg
{
    int weapon;
    int players[33];
    int numPlayers;

    bool Decode(BfRead message)
    {
        if (message.BytesLeft != FOFLIB_HUDEQUIPITEMSMSG_SIZE) return false;
        this.weapon = message.ReadShort();

        return true;
    }

    void Zero()
    {
        this.weapon = -1;
        this.numPlayers = 0;
        for (int i = 0; i < sizeof(this.players); i++) this.players[i] = 0;
    }

    // USAGE NOTE:
    // - Send -1 first, then the weapons you want.
    // - Only send 5 additional weapons, more than 5 crashes the client
    bool Send()
    {
        BfWrite message = UserMessageToBfWrite(StartMessage("HudEquipItems", this.players, this.numPlayers));
        message.WriteShort(this.weapon);

        EndMessage();

        return true;
    }
}