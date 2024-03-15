#include <fstream>
#include <string>
#include <stdexcept>

#include "ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/Ini/File.h"
#include "Utils/Ini/Section.h"
#include "Utils/Ini/Writer.h"
#include "Settings.h"

namespace Ironhead
{

Settings *Settings::getInstance()
{
  return Base::Singleton<Settings>::get();
}


Settings::Settings()
{
}

Settings::~Settings()
{
}

bool Settings::save()
{
    return true;
}


bool Settings::load()
{
    return true;
}

bool Settings::displayFps() const
{
    return _displayFps;
}

bool Settings::audioEnabled() const
{
    return _audioEnabled;
}

void Settings::setAudioEnabled(bool _enabled)
{
    this->_audioEnabled = _enabled;
}

void Settings::setSfxVolume(double _sfxVolume)
{
    this->_sfxVolume = _sfxVolume;
}

double Settings::sfxVolume() const
{
    return _sfxVolume;
}

void Settings::setMusicVolume(double _musicVolume)
{
    this->_musicVolume = _musicVolume;
}


double Settings::musicVolume() const
{
    return _musicVolume;
}

void Settings::setMasterVolume(double _masterVolume)
{
    this->_masterVolume = _masterVolume;
}

double Settings::masterVolume() const
{
    return _masterVolume;
}


void Settings::setDisplayMode(unsigned int _scale)
{
    this->_displayMode = _displayMode;
}

unsigned int Settings::displayMode() const
{
    return _displayMode;
}

void Settings::setFullscreen(bool _fullscreen)
{
    this->_fullscreen = _fullscreen;
}

bool Settings::fullscreen() const
{
    return _fullscreen;
}

}
