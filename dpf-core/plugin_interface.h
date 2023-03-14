/**
 * @file plugin.h
 * @author Kai Harten (kaiharten@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef DPF_PLUGIN_INTERFACE_H_
#define DPF_PLUGIN_INTERFACE_H_

#include <string>
#include <map>
#include <vector>

namespace dpf{

class PluginConfigParameter{
 public:
  PluginConfigParameter(void);
 private:
};

class DefaultPluginType{
 public:
  PluginInterface(void){}
  virtual ~PluginInterface(void){}
  
  std::map<std::string, 
  
  virtual std::string Name(void) = 0;
  
 private:

};
}
#endif