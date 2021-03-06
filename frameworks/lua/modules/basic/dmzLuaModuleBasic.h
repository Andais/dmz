#ifndef DMZ_LUA_MODULE_BASIC_DOT_H
#define DMZ_LUA_MODULE_BASIC_DOT_H

#include <dmzFileCache.h>
#include <dmzLuaExt.h>
#include <dmzLuaKernelErrorHandler.h>
#include <dmzLuaModule.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeExit.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimePluginContainer.h>
#include <dmzSystemFile.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>

#include <luacpp.h>

namespace dmz {

   class LuaModuleBasic :
         public Plugin,
         public LuaModule,
         public LuaErrorHandler,
         public FileCacheAction {

      public:
         LuaModuleBasic (const PluginInfo &Info, Config &local, Config &global);
         ~LuaModuleBasic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // LuaModule Interface
         virtual void add_lua_path (const String &Path);

         virtual void register_lua_observer (
            const UInt32 CallbackMask,
            LuaObserver &observer);

         virtual void release_lua_observer (
            const UInt32 CallbackMask,
            LuaObserver &observer);

         virtual Boolean reset_lua ();
         virtual void set_hook_count (const Int32 Count);
         virtual Int32 get_hook_count ();

         // LuaErrorHandler Interface
         virtual int handle_lua_error  (lua_State *L);

         // FileCacheAction Interface
         virtual void process_file (
            const FileCacheResultEnum RequestResult,
            const String &LocalFilePath,
            const String &RequestedFileLocation,
            const String &RequestedFileName);

         // LuaModuleBasic Interface
         void lua_hook_function (lua_State *L, lua_Debug *ar);

      protected:

         struct ScriptStruct;

         struct InstanceStruct {

            const String Name;
            const String Scope;
            const ScriptStruct &Script;

            InstanceStruct (
                  const String &TheName,
                  const String &TheScope,
                  const ScriptStruct &TheScript) :
                  Name (TheName),
                  Scope (TheScope),
                  Script (TheScript) {;}
         };

         struct ScriptStruct {

            const String Name;
            const String URL;
            const String File;
            const Boolean Globals;
            String foundFile;
            Boolean loaded;
            HashTableStringTemplate<InstanceStruct> instanceTable;

            ScriptStruct (
                  const String &TheName,
                  const String &TheURL,
                  const String &TheFile,
                  const Boolean TheGlobals) :
                  Name (TheName),
                  URL (TheURL),
                  File (TheFile),
                  Globals (TheGlobals),
                  loaded (False) {;}
            ~ScriptStruct () { instanceTable.clear (); }
         };

         void _open_lua ();
         void _start_lua_plugins ();
         void _stop_lua_plugins ();
         void _close_lua ();
         void _update_hook_function ();
         ScriptStruct *_request_script (Config &script);
         void _load_script (ScriptStruct &ss);
         void _instance_script (InstanceStruct &is);
         void _register_instance (InstanceStruct &is);
         void _discover_ext (const Plugin *PluginPtr);
         void _remove_ext (const Plugin *PluginPtr);
         void _add_lua_paths ();
         void _start_optimizer ();
         void _init (Config &local, Config &global);

         Exit _exit;
         Boolean _exitOnError;
         Boolean _startOptimizer;

         lua_State *_luaState;
         Boolean _started;

         Int32 _hookCount;

         HashTableString _keywordTable;
         HashTableStringTemplate<InstanceStruct> _instanceTable;
         HashTableStringTemplate<ScriptStruct> _scriptTable;
         HashTableStringTemplate<ScriptStruct> _pathTable;
         HashTableStringTemplate<ScriptStruct> _preloadTable;

         HashTableHandleTemplate<LuaObserver> _obsCallTable;
         HashTableHandleTemplate<LuaObserver> _obsReturnTable;
         HashTableHandleTemplate<LuaObserver> _obsLineTable;
         HashTableHandleTemplate<LuaObserver> _obsCountTable;
         HashTableHandleTemplate<LuaObserver> _obsCountUpdateTable;
         HashTableHandleTemplate<LuaObserver> _obsScriptTable;
         HashTableHandleTemplate<LuaObserver> _obsErrorTable;

         PluginContainer _extensions;
         HashTableHandleTemplate<LuaExt> _extTable;

         PathContainer _luaPaths;

         Config _global;

         Log _log;

      private:
         LuaModuleBasic ();
         LuaModuleBasic (const LuaModuleBasic &);
         LuaModuleBasic &operator= (const LuaModuleBasic &);

   };
};

#endif // DMZ_LUA_MODULE_BASIC_DOT_H
