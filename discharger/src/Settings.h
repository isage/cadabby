#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include "Base/Singleton.h"

namespace Ironhead
{

class Settings
{
public:
    static Settings *getInstance();


    bool save();
    bool load();

    unsigned int screenWidth() const;

    unsigned int screenHeight() const;

    bool displayFps() const;

    bool audioEnabled() const;
    void setAudioEnabled(bool _enabled);

    void setSfxVolume(double _sfxVolume);
    double sfxVolume() const;
    void setMusicVolume(double _musicVolume);
    double musicVolume() const;
    void setMasterVolume(double _masterVolume);
    double masterVolume() const;

    std::string musicPath() const;

    void setDisplayMode(unsigned int _scale);
    unsigned int displayMode() const;
    void setFullscreen(bool _fullscreen);
    bool fullscreen() const;

protected:
    friend class Base::Singleton<Settings>;
    Settings();
    ~Settings();

    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

private:
    bool _displayFps = false;
    unsigned int _displayMode = 1;
    bool _fullscreen = false;

    // [sound]
    std::string _musicPath = "data/music/";
    bool _audioEnabled = true;
    double _masterVolume = 1.0;
    double _musicVolume = 1.0;
    double _sfxVolume = 1.0;
};

} // Ironhead

#endif // SETTINGS_H
