#if defined _foflib_convarmanager_included
    #endinput
#endif
#define _foflib_convarmanager_included

#include <../include/sourcemod>

// ConVarManager is used to swap and restore several ConVars
// at a time. Register several ConVar values and activate/deactivate them
// all simultaneously.
enum struct ConVarManager
{
    // starts as null, null, false, which is what I want
    StringMap holder;
    ArrayList keys;
    bool active;

    // ConVarManager destructor function.
    void Destroy()
    {
        if (this.holder != null)
        {
            delete this.holder;
            this.holder = null;
        }

        if (this.keys != null)
        {
            delete this.keys;
            this.keys = null;
        }

        this.active = false;
    }

    // Gets the value of a ConVar as stored in the manager.
    //
    // @param key           The name of the ConVar.
    // @param value         The output buffer to store the ConVar value.
    // @param size          The size for the value buffer.
    // @return              Whether or not the operation was successful.
    bool GetConVar(const char[] key, char[] value, int size)
    {
        if (size < 0) return false;
        if (this.holder == null) return false;
        if (!this.holder.ContainsKey(key)) return false;

        this.holder.GetString(key, value, size);
        return true;
    }

    // Sets the value specified in the ConVar manager.
    // If the manager is currently active, it immediately applies
    // the value to the convar.
    //
    // @param key           The name of the ConVar.
    // @param value         The value to set for the ConVar.
    // @param replace       Allows replacement if the key is already present.
    // @return              Whether or not the operation was successful.
    bool SetConVar(const char[] key, const char[] value, bool replace=true)
    {
        ConVar cv = FindConVar(key)
        if (cv == null) return false;
        
        if (this.holder == null) this.holder = new StringMap();

        // Don't need to delete, will never already have key if just created
        bool hasKey = this.holder.ContainsKey(key);
        if (!replace && hasKey) return false;

        if (this.active)
        {
            if (!hasKey)
            {
                char realval[PLATFORM_MAX_PATH];
                cv.GetString(realval, sizeof(realval));
                this.holder.SetString(key, realval);
            }

            cv.SetString(value);
        }
        else
        {
            this.holder.SetString(key, value);    
        }

        if (!hasKey)
        {
            if (this.keys == null) this.keys = new ArrayList(PLATFORM_MAX_PATH / 4);
            this.keys.PushString(key);
        }
        
        return true;
    }

    // Will store the value in the manager based on the current ConVar value.
    //
    // @param key           The name of the ConVar.
    // @param replace       Allows replacement if the key is already present.
    // @return              Whether or not the operation was successful.
    bool SetConVarFromActual(const char[] key, bool replace=true)
    {
        ConVar cv = FindConVar(key);
        if (cv == null) return false;

        if (this.holder == null) this.holder = new StringMap();

        // Don't need to delete, will never already have key if just created
        bool hasKey = this.holder.ContainsKey(key);
        if (!replace && hasKey) return false;

        char value[PLATFORM_MAX_PATH];
        if (this.active && hasKey)
        {
            // Holder has actual
            this.holder.GetString(key, value, sizeof(value));
            cv.SetString(value, sizeof(value));
        }
        else
        {
            // ConVar has actual
            cv.GetString(value, sizeof(value));
            this.holder.SetString(key, value);
        }

        if (!hasKey)
        {
            if (this.keys == null) this.keys = new ArrayList();
            this.keys.PushString(key);
        }

        return true;
    }

    // Removes a ConVar from tracking.
    // If the manager is active, the original ConVar value is restored.
    // The erased value can optionally be written out into a string buffer.
    //
    // @param key           The name of the ConVar.
    // @param value         A buffer for the erased ConVar value.
    // @param size          The size of the value buffer.
    bool RemoveConVar(const char[] key, char[] value="", int size=0)
    {
        if (this.holder == null) return false;
        if (!this.holder.ContainsKey(key)) return false;
        
        ConVar cv = FindConVar(key);
        if (this.active && cv != null)
        {
            char move[PLATFORM_MAX_PATH];
            if (size > 0) cv.GetString(value, size);

            this.holder.GetString(key, move, sizeof(move));
            cv.SetString(move);
        }
        else
        {
            if (size > 0) this.holder.GetString(key, value, size);
        }

        this.holder.Remove(key);
        if (this.holder.Size == 0)
        {
            delete this.holder; this.holder = null;
            delete this.keys; this.keys = null;
        }
        else
        {
            this.keys.Erase(this.keys.FindString(key));
        }

        return true;
    }

    // Activates all ConVars stored in the manager.
    // For each ConVar stored in the manager, the current value is stored and
    // replaced with the ConVarManager's value.
    bool Activate()
    {
        if (this.active) return false;
        this.active = true;

        // No more work to be done, will be managed on adding and subtracting
        if (this.holder == null) return true;

        char key[PLATFORM_MAX_PATH], holderValue[PLATFORM_MAX_PATH], cvValue[PLATFORM_MAX_PATH];
        ConVar cv;

        // Setting forward
        for (int i = 0; i < this.keys.Length; i++)
        {
            this.keys.GetString(i, key, sizeof(key));
            cv = FindConVar(key);

            if (cv == null)
            {
                this.holder.Remove(key);
                this.keys.Erase(i--);
            }
            else
            {
                this.holder.GetString(key, holderValue, sizeof(holderValue));
                cv.GetString(cvValue, sizeof(cvValue));

                this.holder.SetString(key, cvValue, sizeof(cvValue));
                cv.SetString(holderValue, sizeof(holderValue));
            }
        }

        if (this.holder.Size == 0)
        {
            delete this.holder; this.holder = null;
            delete this.keys; this.keys = null;
        }

        return true;
    }

    // Deactivates all ConVars stored in the manager.
    // The original values of all tracked ConVars are restored.
    bool Deactivate()
    {
        if (!this.active) return false;
        this.active = false;

        // No more work to be done, will be managed on adding and subtracting
        if (this.holder == null) return true;

        char key[PLATFORM_MAX_PATH], holderValue[PLATFORM_MAX_PATH], cvValue[PLATFORM_MAX_PATH];
        ConVar cv;

        // Restoring in reverse
        for (int i = this.keys.Length - 1; i >= 0; i--)
        {
            this.keys.GetString(i, key, sizeof(key));
            cv = FindConVar(key);

            if (cv == null)
            {
                this.holder.Remove(key);
                this.keys.Erase(i);
            }
            else
            {
                this.holder.GetString(key, holderValue, sizeof(holderValue));
                cv.GetString(cvValue, sizeof(cvValue));

                this.holder.SetString(key, cvValue, sizeof(cvValue));
                cv.SetString(holderValue, sizeof(holderValue));
            }
        }

        if (this.holder.Size == 0)
        {
            delete this.holder; this.holder = null;
            delete this.keys; this.keys = null;
        }

        return true;
    }
}