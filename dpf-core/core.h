/**
 * @file core.h
 * @author Kai Harten (kaiharten@gmail.com)
 * @brief Header file containing the declaration of classes for a plugin
 * architecture
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 * This header file contains declarations for the Core, Server, Plugin,
 * LibraryLoader, and Library classes. Core is the main class of this library,
 * and is responsible for managing servers, plugins, and libraries. Server
 * represents a plugin server, and holds information about the server name,
 * minimum plugin version, and a map of plugins. Plugin represents a plugin, and
 * holds information about the plugin name, server name, and version.
 * LibraryLoader is a helper class used by Library to load a dynamic library and
 * extract a function pointer to the library's registration function. Library
 * represents a dynamic library, and is responsible for loading the library,
 * registering the library's plugins with the Core class, and freeing the
 * library. This header file also includes platform-specific code for loading
 * dynamic libraries.
 */
#ifndef DPF_CORE_H_
#define DPF_CORE_H_

#include <map>
#include <string>
#include <vector>

// Include platform-specific libraries for dynamic loading
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <iostream>

namespace dpf {

/**
 * @brief Class to represent a plugin in the DPF system. Plugins are loaded and
 * managed by the Core class.
 */
class Plugin {
 public:
  Plugin(std::string server_name, std::string name, int version);
  virtual ~Plugin() {}
  /**
   * @brief Returns the name of the plugin
   */
  std::string Name();
  /**
   * @brief Returns the name of the server where the plugin is part of
   */
  std::string ServerName();
  /**
   * @brief Returns the version of the plugin
   */
  int Version();

 private:
  std::string _name;
  std::string _server_name;
  int _version;
};

class IPlugin {
 public:
  IPlugin() {}
  virtual ~IPlugin() {}
  virtual std::string name() = 0;

  static const int version = 1;
  const std::string ServerName() { return "Default"; }
};

class PluginCreator : public Plugin {
 public:
  PluginCreator(std::string name, std::string server_name, int version)
      : Plugin(server_name, name, version) {}
  virtual IPlugin* create() = 0;
};

class Core;
typedef void fnRegisterLibrary(Core*);

/**
 * @brief Class to represent a library loader for dynamic loading of libraries
 * on different platforms.
 */
class LibraryLoader {
 public:
  ~LibraryLoader(void);
  /**
   * @brief Loads a dynamically linked library that conforms to the api (.so or
   * .dll)
   */
  bool Load(const std::string file_name);
  /**
   * @brief Registers the functions that takes in a dpf::Core
   */
  fnRegisterLibrary* RegisterFunction(void);

  /**
   * @brief Frees a loaded library
   */
  void Free();

 private:
#ifdef _WIN32
  HMODULE _handle;
#else
  void* _handle;
#endif
};

/**
 * @brief Class to represent a dynamically loaded library in the DPF system.
 * Libraries contain plugins that are loaded and managed by the Core class.
 */
class Library {
 public:
  Library(void);
  ~Library(void);
  bool Load(const std::string& filename);
  void RegisterLibrary(Core* core);

 private:
  fnRegisterLibrary* _register_function;
  LibraryLoader _library_loader;
};

class Server {
 public:
  Server(const std::string name, int min_plugin_version);
  ~Server(void);

  std::string Name(void);
  int MinimumPluginVersion(void);
  std::map<std::string, Plugin*>& Plugins();
  void Clear();

 private:
  std::string _name;
  int _min_plugin_version;
  std::map<std::string, Plugin*> _plugins;
};

class Core {
 public:
  virtual ~Core(void);

  void AddServer(std::string name, int min_plugin_version);
  bool AddPlugin(Plugin* plugin);

  template <class PluginType>
  PluginType* GetPlugin(const std::string& server_name,
                        const std::string& name);

  template <typename PluginType>
  std::vector<PluginType*> GetAllPlugins(const std::string& server_name) {
    std::vector<PluginType*> plugins;

    auto server = _GetServer(server_name);
    if (!server) {
      return plugins;
    }

    for (std::map<std::string, Plugin*>::iterator iter =
             server->Plugins().begin();
         iter != server->Plugins().end(); ++iter) {
      plugins.push_back(static_cast<PluginType*>(iter->second));
    }

    return plugins;
  }

  bool LoadLibrary(const std::string& file_name);
  void ClearPlugins(void);
  void Clear(void);

 private:
  std::map<std::string, Server*> _servers;
  std::vector<Library*> _libraries;

  Server* _GetServer(const std::string& name);
};
}  // namespace dpf
#endif