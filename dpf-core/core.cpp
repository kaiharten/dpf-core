/**
 * @file core.cpp
 * @author Kai Harten (kaiharten@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "core.h"

template <class K, class V>
void delete_all_values(std::map<K, V>& map) {
  for (typename std::map<K, V>::iterator iter = map.begin(); iter != map.end();
       ++iter) {
    delete iter->second;
  }
}
template <class V>
void delete_all_values(std::vector<V>& vec) {
  for (typename std::vector<V>::iterator iter = vec.begin(); iter != vec.end();
       ++iter) {
    delete *iter;
  }
}

/** Plugin
 * ------------------------------------------------------------------------*/
dpf::Plugin::Plugin(std::string server_name, std::string name, int version)
    : _server_name(server_name), _name(name), _version(version) {}

std::string dpf::Plugin::Name(void) { return _name; }

std::string dpf::Plugin::ServerName(void) { return _server_name; }

int dpf::Plugin::Version(void) { return _version; }

/* Library loader
 * ---------------------------------------------------------------*/
dpf::LibraryLoader::~LibraryLoader(void) { this->Free(); }

bool dpf::LibraryLoader::Load(const std::string file_name) {
#ifdef _WIN32
  _handle = LoadLibraryA(file_name.c_str());
#else
  _handle = dlopen(file_name.c_str(), RTLD_NOW);
#endif
  return (_handle != NULL);
}

dpf::fnRegisterLibrary* dpf::LibraryLoader::RegisterFunction(void) {
#ifdef _WIN32
  return reinterpret_cast<dpf::fnRegisterLibrary*>(
      GetProcAddress(_handle, "register_dpf_package"));
#else
  return reinterpret_cast<fnRegisterLibrary*>(
      dlsym(_handle, "register_dpf_package"));
#endif
}

void dpf::LibraryLoader::Free(void) {
  if (_handle != NULL) {
#ifdef _WIN32
    FreeLibrary(_handle);
#else
    dlclose(_handle);
#endif
  }
}

/* Library
 * ---------------------------------------------------------------------*/
dpf::Library::Library(void) : _register_function(NULL) {}

dpf::Library::~Library(void) { _library_loader.Free(); }

bool dpf::Library::Load(const std::string& filename) {
  bool ret_val = false;

  if (_library_loader.Load(filename)) {
    _register_function = _library_loader.RegisterFunction();

    if (_register_function != NULL) {
      ret_val = true;
    } else {
      _library_loader.Free();
    }
  }
  return ret_val;
}

void dpf::Library::RegisterLibrary(dpf::Core* core) {
  _register_function(core);
}

/* PluginServer --------------------------------------------------------------*/
dpf::Server::Server(const std::string name, int min_plugin_version)
    : _name(name), _min_plugin_version(min_plugin_version) {}

dpf::Server::~Server(void) { delete_all_values(_plugins); }

std::string dpf::Server::Name(void) { return _name; }

int dpf::Server::MinimumPluginVersion(void) { return _min_plugin_version; }

std::map<std::string, dpf::Plugin*>& dpf::Server::Plugins(void) {
  return _plugins;
}

void dpf::Server::Clear(void) { delete_all_values(_plugins); }

/* DPF Core ------------------------------------------------------------------*/
dpf::Core::~Core(void) {
  delete_all_values(_servers);
  delete_all_values(_libraries);
}

void dpf::Core::AddServer(std::string name, int min_plugin_version) {
  _servers[name] = new Server(name, min_plugin_version);
}

bool dpf::Core::AddPlugin(dpf::Plugin* plugin) {
  if (!plugin) return false;

  dpf::Server* server = _GetServer(plugin->ServerName());
  if (!server){
    return NULL;
  }

  if (server->MinimumPluginVersion() > plugin->Version()) return false;

  server->Plugins()[plugin->Name()] = plugin;
  return true;
}

template <class PluginType>
PluginType* dpf::Core::GetPlugin(const std::string& server_name,
                       const std::string& name) {
  dpf::Server* server = _GetServer(server_name);
  if (!server) return NULL;

  std::map<std::string, dpf::Plugin*>::iterator plugin_iter =
      server->Plugins().find(name);
  if (plugin_iter == server->Plugins().end()) return NULL;
  return static_cast<PluginType*>(plugin_iter->second);
}

bool dpf::Core::LoadLibrary(const std::string& file_name) {
  bool ret_val = false;
  dpf::Library* library = new dpf::Library();
  if (library->Load(file_name)) {
    library->RegisterLibrary(this);
    _libraries.push_back(library);
    ret_val = true;
  } else {
    delete library;
  }

  return ret_val;
}

void dpf::Core::ClearPlugins(void) {
  for (auto& [name, server] : _servers) {
    server->Clear();
  }
}

void dpf::Core::Clear(void) {
  delete_all_values(_servers);
  delete_all_values(_libraries);
}

dpf::Server* dpf::Core::_GetServer(const std::string& name) {
  std::map<std::string, dpf::Server*>::iterator server_iter = _servers.find(name);
  if (server_iter == _servers.end()) {
    return NULL;
  } else {
    return server_iter->second;
  }
}
