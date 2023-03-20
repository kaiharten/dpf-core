#include <iostream>
#include <map>
#include <vector>

#include "dpf-core/core.h"
#include "interface.h"

int main(void) {
  dpf::Core core;
  core.AddServer(ExampleType::ServerName(), ExampleType::version);

  bool status = core.LoadLibrary(
      "/workspaces/dpf-core/build/examples/create_and_load_plugin/"
      "libplugin.so");

  if (!status) return 0;

  std::vector<ExampleType*> plugins;
  std::string server_name = ExampleType::ServerName();
  std::vector<ExampleTypeCreator*> examples =
      core.GetAllPlugins<ExampleTypeCreator>(server_name);

  if (examples.size() > 0) {
    for (std::vector<ExampleTypeCreator*>::iterator iter = examples.begin();
         iter != examples.end(); ++iter) {
      ExampleTypeCreator& plugin = *(*iter);
      std::cout << plugin.ServerName() << std::endl;
      ExampleType* instance = plugin.create();
      std::cout << instance->name() << " is " << instance->DoSomething()
                << " years old." << std::endl;
    }
  }
  return 0;
}
