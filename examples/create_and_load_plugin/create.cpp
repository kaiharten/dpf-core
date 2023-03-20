/**
 *
 */

#include "dpf-core/core.h"
#include "interface.h"

class Bob : public ExampleType {
 public:
  Bob(void) {}
  std::string name() override { return "Bob"; }
  int DoSomething(void) override { return 15; }
};

class BobCreator : public ExampleTypeCreator {
 public:
  BobCreator(void) : ExampleTypeCreator("BobPlugin", Bob::version) {}
  Bob* create() override { return new Bob(); }
};

class Person : public ExampleType {
 public:
  explicit Person(std::string name, int age) : _name(name), _age(age) {}
  std::string name() override { return _name; }
  int DoSomething(void) override { return _age; }

 private:
  std::string _name;
  int _age;
};

class PersonCreator : public ExampleTypeCreator {
 public:
  explicit PersonCreator(std::string name, int age)
      : _name(name),
        _age(age),
        ExampleTypeCreator(name + "Plugin", Person::version) {}
  Person* create() override { return new Person(_name, _age); }

 private:
  std::string _name;
  int _age;
};

#ifdef _WIN32
#define EXPORTIT __declspec(dllexport)
#else
#define EXPORTIT
#endif

extern "C" EXPORTIT void register_dpf_package(dpf::Core* core) {
  core->AddPlugin(new BobCreator());
  core->AddPlugin(new PersonCreator("Alice", 56));
  core->AddPlugin(new PersonCreator("William", 30));
  core->AddPlugin(new PersonCreator("Billiam", 30));
}

