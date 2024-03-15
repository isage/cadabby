#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "Base/Singleton.h"

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Ironhead
{
class ResourceManager
{

public:
  static ResourceManager *getInstance();
  static bool fileExists(const std::string &filename);

  void shutdown();
  std::string getLocalizedPath(const std::string &filename);
  std::string getPrefPath(const std::string &filename);
  std::string getPathForDir(const std::string &dir);
protected:
  friend class Base::Singleton<ResourceManager>;

  ResourceManager();
  ~ResourceManager();
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
};
}
#endif
