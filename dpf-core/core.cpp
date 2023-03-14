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

dpf::Plugin::Plugin(std::string server_name, std::string name, int version)
    : _server_name(server_name), _name(name), _version(version) {}

std::string dpf::Plugin::Name(void) { return _name; }

std::string dpf::Plugin::ServerName(void) { return _server_name; }

int dpf::Plugin::Version(void) { return _version; }

dpf::LibraryLoader::~LibraryLoader(void) { this->Free(); }

bool dpf::LibraryLoader::Load(const std::string file_name) {
#ifdef _WIN32
  _handle = LoadLibraryA(file_name.c_str());
#else
  // todo add linux support
#endif
  return (_handle != NULL);
}

dpf::fnRegisterLibrary* dpf::LibraryLoader::RegisterFunction(void) {
#ifdef _WIN32
  return reinterpret_cast<dpf::fnRegisterLibrary*>(
      GetProcAddress(_handle, "register_dpf_package"));
#else
  // todo implement support for linux
#endif
}

void dpf::LibraryLoader::Free(void) {
  if (_handle != NULL) {
#ifdef _WIN32
    FreeLibrary(_handle);
#else
    // todo implement support for linux
#endif
  }
}

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

dpf::Server::Server(const std::string name, int min_plugin_version)
    : _name(name), _min_plugin_version(min_plugin_version) {}

dpf::Server::~Server(void) { delete_all_values(_plugins); }

std::string dpf::Server::Name(void) { return _name; }

int dpf::Server::MinimumPluginVersion(void) { return _min_plugin_version; }

std::map<std::string, dpf::Plugin*>& dpf::Server::Plugins(void) {
  return _plugins;
}

void dpf::Server::Clear(void) { delete_all_values(_plugins); }

dpf::Core::~Core(void) {
  delete_all_values(_servers);
  delete_all_values(_libraries);
}

void dpf::Core::AddServer(std::string name, int min_plugin_version) {
  _servers[name] = new Server(name, min_plugin_version);
}

bool dpf::Core::AddPlugin(dpf::Plugin* plugin){
  bool ret_val = false;
  
  dpf::Server* server = NULL;
  if(plugin != NULL){
    server = _GetServer(plugin->ServerName()); 
  
    if(server != NULL && server->MinimumPluginVersion() <= plugin->Version()){
      server->Plugins()[plugin->Name()] = plugin;
      ret_val = true;
    }
  }
  return ret_val;
}

template <class PluginType>
auto dpf::Core::GetPlugin(const std::string& server_name, const std::string& name) -> PluginType{
  auto server = _GetServer(server_name);

  if(server != NULL){
    auto plug
  }
}