#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#if defined(__unix__) || defined(__APPLE__) || defined(__VITA__) || defined(__SWITCH__)
#include <sys/stat.h>
#elif defined(__HAIKU__)
#include <posix/sys/stat.h> // ugh
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#if defined(__VITA__)
#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#endif

#include "ResourceManager.h"
#include "common/misc.h"

#include <json.hpp>
#include <SDL.h>

namespace Ironhead
{

bool ResourceManager::fileExists(const std::string &filename)
{
#if defined(__unix__) || defined(__APPLE__) || defined(__HAIKU__) || defined(__VITA__) || defined(__SWITCH__)
  struct stat st;

  if (stat(filename.c_str(), &st) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
#elif defined(_WIN32) || defined(WIN32) // Windows
  DWORD attrs = GetFileAttributes(widen(filename).c_str());

  // Assume path exists
  if (attrs != INVALID_FILE_ATTRIBUTES)
  {
    return true;
  }
  else
  {
    return false;
  }
#else
#error Platform not supported
#endif
  return false;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager() {}

ResourceManager *ResourceManager::getInstance()
{
  return Base::Singleton<ResourceManager>::get();
}

void ResourceManager::shutdown() {}

std::string ResourceManager::getLocalizedPath(const std::string &filename)
{
  std::vector<std::string> _paths;

#if defined(__linux__)
  char *home = getenv("HOME");

  if (home != NULL)
  {
    _paths.push_back(std::string(home) + "/.local/share/ironhead/data/" + filename);
  }

  _paths.push_back("/usr/share/ironhead/data/" + filename);

  _paths.push_back("/usr/local/share/ironhead/data/" + filename);

  _paths.push_back("../share/ironhead/data/" + filename);

#elif defined(__APPLE__)
  char *home = SDL_GetPrefPath(NULL, "ironhead");

  if (home != NULL)
  {
    _paths.push_back(std::string(home) + "/data/" + filename);
    SDL_free(home);
  }


#elif defined(__VITA__)
  _paths.push_back("ux0:/data/ironhead/data/" + filename);
  _paths.push_back("app0:/data/" + filename);

#elif defined(__SWITCH__)
  _paths.push_back("data/" + filename);
  _paths.push_back("romfs:/data/" + filename);

#endif

  _paths.push_back("data/" + filename);

  for (auto &_tryPath: _paths)
  {
    if (fileExists(_tryPath))
      return _tryPath;
  }

  return _paths.back();
}

std::string ResourceManager::getPrefPath(const std::string &filename)
{
  std::string _tryPath;

#if defined(__VITA__)
  sceIoMkdir("ux0:/data/ironhead/", 0700);
  _tryPath = std::string("ux0:/data/ironhead/") + std::string(filename);
#elif defined(__SWITCH__)
//  mkdir("ironhead/", 0700);
  _tryPath = std::string(filename);
#else
  char *prefpath      = SDL_GetPrefPath(NULL, "ironhead");
  _tryPath = std::string(prefpath) + std::string(filename);
  SDL_free(prefpath);
#endif

  return _tryPath;
}

std::string ResourceManager::getPathForDir(const std::string &dir)
{
  std::vector<std::string> _paths;

#if defined(__linux__)
  char *home = getenv("HOME");
  if (home != NULL)
  {
    _paths.push_back(std::string(home) + "/.local/share/ironhead/data/" + dir);
  }

  _paths.push_back("/usr/share/ironhead/data/" + dir);
  _paths.push_back("/usr/local/share/ironhead/data/" + dir);
  _paths.push_back("../share/ironhead/data/" + dir);

#elif defined(__APPLE__)
  char *home = SDL_GetPrefPath(NULL, "ironhead");
  if (home != NULL)
  {
    _paths.push_back(std::string(home) + "/data/" + dir);
    SDL_free(home);
  }

#elif defined(__VITA__)
  _paths.push_back("ux0:/data/ironhead/data/" + dir);
  _paths.push_back("app0:/data/" + dir);

#elif defined(__SWITCH__)
  _paths.push_back("data/" + dir);
  _paths.push_back("romfs:/data/" + dir);

#endif

#if !defined(__VITA__) and !defined(__SWITCH__)
  _paths.push_back("data/" + dir);
#endif

  for (auto &_tryPath: _paths)
  {
    if (fileExists(_tryPath))
      return _tryPath;
  }

  return _paths.back();
}

}

