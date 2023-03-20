/**
 *
 */

#include "dpf-core/core.h"
#include "interface.h"

class ExampleA : public ExampleType {
 public:
  ExampleA(void) {}
  std::string name() override { return "ExampleA"; }
  int DoSomething(void) override { return 15; }
};

class ExampleACreator : public ExampleTypeCreator {
 public:
  ExampleACreator(void)
      : ExampleTypeCreator("ExampleAPlugin", ExampleA::version) {}
  ExampleA* create() override { return new ExampleA(); }
};

#ifdef _WIN32
#define EXPORTIT __declspec(dllexport)
#else
#define EXPORTIT
#endif

extern "C" EXPORTIT void register_dpf_package(dpf::Core* core) {
  core->AddPlugin(new ExampleACreator());
}
