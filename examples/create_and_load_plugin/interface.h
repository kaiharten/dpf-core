/**
 * @author KH
 */
#include "dpf-core/core.h"

class ExampleType : public dpf::IPlugin {
 public:
  ExampleType(void) {}
  virtual ~ExampleType(void) {}
  static const std::string ServerName(void) {return "Example";}
  virtual std::string name() = 0;
  virtual int DoSomething(void) = 0;
};

class ExampleTypeCreator : public dpf::PluginCreator {
 public:
  ExampleTypeCreator(std::string name, int version)
      : dpf::PluginCreator(name, "Example", version) {}
  virtual ExampleType* create(void) = 0;
};
