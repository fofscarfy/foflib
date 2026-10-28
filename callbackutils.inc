#if defined _foflib_callbackutils_included
    #endinput
#endif
#define _foflib_callbackutils_included

#define FOFLIB_PLUGINCALLBACK_PLUGIN_INDEX 0
#define FOFLIB_PLUGINCALLBACK_FUNC_INDEX 1
#define FOFLIB_PLUGINCALLBACK_ENABLED_INDEX 2

enum struct PluginCallback
{
    Handle plugin;
    Function func;
    bool enabled;

    // Quick constructor function for the PluginCallback data structure.
    //
    // @param plugin            The plugin containing the callback function.
    //                          Pass 'null' to indicate the calling plugin.
    // @param func              The callback function.
    // @param enabled           Whether this callback function is currently
    //                          enabled.
    void Populate(Handle plugin, Function func, bool enabled)
    {
        this.plugin = plugin;
        this.func = func;
        this.enabled = enabled;
    }
}

enum struct CallbackManager
{
    PrivateForward fwd;
    ArrayList funcs;
    ExecType et;

    void Init(ExecType et=ET_Hook)
    {
        this.et = et;
        this.funcs = new ArrayList(sizeof(PluginCallback));
    }

    int Find(Function func, Handle plugin=null)
    {
        if (this.funcs == null) return -1;

        PluginCallback pc;
        for (int i = 0; i < this.funcs.Length; i++)
        {
            this.funcs.GetArray(i, pc, sizeof(pc));
            if (pc.plugin == plugin && pc.func == func) return i;
        }

        return -1;
    }

    bool Add(Function func, Handle plugin=null, bool startEnabled=true)
    {
        if (this.Find(func, plugin) != -1) return false;
        if (this.funcs == null) this.Init();

        PluginCallback pc; pc.Populate(plugin, func, startEnabled);
        this.funcs.PushArray(pc, sizeof(pc));

        if (startEnabled)
        {
            if (this.fwd == null) this.fwd = new PrivateForward(this.et, Param_Array);
            this.fwd.AddFunction(plugin, func);
        }

        return true;
    }

    bool Toggle(Function func, Handle plugin=null, bool enable=true)
    {
        int idx = this.Find(func, plugin);
        if (idx == -1) return false;

        PluginCallback pc; this.funcs.GetArray(idx, pc, sizeof(pc));
        if (pc.enabled == enable) return true;

        pc.enabled = enable;
        this.funcs.SetArray(idx, pc, sizeof(pc));

        bool result;
        if (enable)
        {
            if (this.fwd == null) this.fwd = new PrivateForward(this.et, Param_Array);
            result = this.fwd.AddFunction(plugin, func);
        }
        else 
        {
            if (this.fwd == null) return false;
            result = this.fwd.RemoveFunction(plugin, func);
            if (this.fwd.FunctionCount == 0)
            {
                delete this.fwd;
                this.fwd = null;
            }
        }

        return result;
    }

    bool ToggleByPlugin(Handle plugin=null, bool enable=true)
    {
        if (this.funcs == null) return false;

        if (this.fwd == null)
        {
            if (enable) this.fwd = new PrivateForward(this.et, Param_Array);
            else return true;
        }

        PluginCallback pc;
        bool result = true;
        for (int i = 0; i < this.funcs.Length; i++)
        {
            this.funcs.GetArray(i, pc, sizeof(pc));
            if (pc.plugin != plugin || pc.enabled == enable) continue;
            
            pc.enabled = enable;
            this.funcs.SetArray(i, pc, sizeof(pc));

            if (enable) result &= this.fwd.AddFunction(pc.plugin, pc.func);
            else result &= this.fwd.RemoveFunction(pc.plugin, pc.func);
        }

        if (this.fwd.FunctionCount == 0)
        {
            delete this.fwd; 
            this.fwd = null;
        }

        return result;
    }

    bool ToggleAll(bool enable=true)
    {
        if (this.funcs == null) return false;

        if (this.fwd == null)
        {
            if (enable) this.fwd = new PrivateForward(this.et, Param_Array);
            else return true;
        }

        PluginCallback pc;
        bool result = true;
        for (int i = 0; i < this.funcs.Length; i++)
        {
            this.funcs.GetArray(i, pc, sizeof(pc));
            if (pc.enabled == enable) continue;

            pc.enabled = enable;
            this.funcs.SetArray(i, pc, sizeof(pc));

            if (enable) result &= this.fwd.AddFunction(pc.plugin, pc.func);
        }

        if (!enable || this.fwd.FunctionCount == 0)
        {
            delete this.fwd;
            this.fwd = null;
        }

        return result;
    }

    bool Remove(Function func, Handle plugin=null)
    {
        int idx = this.Find(func, plugin);
        if (idx == -1) return false;

        PluginCallback pc; this.funcs.GetArray(idx, pc, sizeof(pc));

        this.funcs.Erase(idx);
        if (this.funcs.Length == 0) 
        {
            this.Destroy();
            return true;
        }

        if (this.fwd != null && pc.enabled)
        {
            this.fwd.RemoveFunction(plugin, func);
        }

        return true;
    }

    bool RemoveByPlugin(Handle plugin=null)
    {
        bool pluginFound = false;
        if (this.funcs == null) return false;

        PluginCallback pc;
        for (int i = this.funcs.Length - 1; i >= 0; i--)
        {
            this.funcs.GetArray(i, pc, sizeof(pc));
            if (pc.plugin != plugin) continue;
            
            pluginFound = true;
            this.funcs.Erase(i);

            if (pc.enabled)
            {
                if (this.fwd == null) continue;

                this.fwd.RemoveFunction(pc.plugin, pc.func);
                if (this.fwd.FunctionCount == 0)
                {
                    delete this.fwd;
                    this.fwd = null;
                }
            }
        }

        if (this.funcs.Length == 0) this.Destroy();

        return pluginFound;
    }

    void Destroy()
    {
        if (this.funcs != null)
        {
            delete this.funcs;
            this.funcs = null;
        }

        if (this.fwd != null)
        {
            delete this.fwd;
            this.fwd = null;
        }
    }
}

stock bool CallbackUtils_AddCallbackRule(ArrayList &funcs, Handle plugin, Function func, bool startEnabled=true)
{
    if (funcs == null) funcs = new ArrayList(sizeof(PluginCallback));

    if (_CallbackUtils_FindPluginCallbackIndex(funcs, plugin, func) != -1) return false;

    PluginCallback pc;
    pc.plugin = plugin;
    pc.func = func;
    pc.enabled = startEnabled;

    funcs.PushArray(pc, sizeof(pc));

    return true;
}

stock bool CallbackUtils_ToggleCallbackRule(ArrayList funcs, Handle plugin, Function func, bool enable)
{
    if (funcs == null) return false;

    int index = _CallbackUtils_FindPluginCallbackIndex(funcs, plugin, func);
    if (index == -1) return false;

    // Setting field in array directly
    funcs.Set(index, enable, FOFLIB_PLUGINCALLBACK_ENABLED_INDEX);

    return true;
}

stock bool CallbackUtils_RemoveCallbackRule(ArrayList &funcs, Handle plugin, Function func)
{
    if (funcs == null) return false;

    int index = _CallbackUtils_FindPluginCallbackIndex(funcs, plugin, func);
    if (index == -1) return false;

    funcs.Erase(index);

    if (funcs.Length == 0)
    {
        delete funcs;
        funcs = null;
    }

    return true;
}

stock bool CallbackUtils_ToggleCallbackRulesByPlugin(ArrayList funcs, Handle plugin, bool enable)
{
    if (funcs == null) return false;

    bool anyChanged = false;

    for (int i = 0; i < funcs.Length; i++)
    {
        if (plugin == funcs.Get(i, FOFLIB_PLUGINCALLBACK_PLUGIN_INDEX))
        {
            funcs.Set(i, enable, FOFLIB_PLUGINCALLBACK_ENABLED_INDEX);
            anyChanged = true;
        }
    }

    return anyChanged;
}

stock bool CallbackUtils_RemoveCallbackRulesByPlugin(ArrayList &funcs, Handle plugin)
{
    if (funcs == null) return false;

    bool anyChanged = false;

    for (int i = funcs.Length - 1; i >= 0; i--)
    {
        if (funcs.Get(i, FOFLIB_PLUGINCALLBACK_PLUGIN_INDEX) == plugin)
        {
            funcs.Erase(i);
            anyChanged = true;
        }
    }

    if (funcs.Length == 0)
    {
        delete funcs;
        funcs = null;
    }

    return anyChanged;
}

stock bool CallbackUtils_RemoveAllCallbackRules(ArrayList &funcs)
{
    if (funcs == null) return false;

    funcs.Clear();
    delete funcs;
    funcs = null;

    return true;
}

// For beginning start from after = -1
stock bool CallbackUtils_GetNextEnabledCallbackRule(ArrayList funcs, int &after, Handle &plugin, Function &func)
{
    if (funcs == null) return false;
    if (after < -1 || after >= funcs.Length) return false;
    
    PluginCallback pc;
    while (++after < funcs.Length)
    {
        funcs.GetArray(after, pc, sizeof(pc));
        if (pc.enabled)
        {
            plugin = pc.plugin;
            func = pc.func;
            return true;
        }
    }

    return false;
}

static int _CallbackUtils_FindPluginCallbackIndex(ArrayList funcs, Handle plugin, Function func)
{
    if (funcs == null) return -1;

    PluginCallback pc;
    for (int i = 0; i < funcs.Length; i++)
    {
        funcs.GetArray(i, pc, sizeof(pc));
        if (pc.plugin == plugin && pc.func == func) return i;
    }

    return -1;
}