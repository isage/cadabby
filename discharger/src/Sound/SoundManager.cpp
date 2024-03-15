#include "SoundManager.h"

#include "../ResourceManager.h"
#include "../common/misc.h"
#include "../Utils/Logger.h"
//#include "../game.h"
#include "../Settings.h"
#include "Ogg.h"

#include <json.hpp>
#include <fstream>
#include <iostream>


namespace Ironhead
{
namespace Sound
{

SoundManager::SoundManager() {}
SoundManager::~SoundManager() {}

SoundManager *SoundManager::getInstance()
{
  return Base::Singleton<SoundManager>::get();
}

bool SoundManager::init()
{
  LOG_INFO("Sound system init");
  if (Mix_Init(MIX_INIT_OGG) == -1)
  {
    LOG_ERROR("Unable to init mixer.");
    return false;
  }

#if SDL_MIXER_PATCHLEVEL >= 2
  if (Mix_OpenAudioDevice(SAMPLE_RATE, AUDIO_S16, 2, 2048, NULL, 0) == -1)
  {
    LOG_ERROR("Unable to open audio device.");
    return false;
  }
#else
  if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16, 2, 2048) == -1)
  {
    LOG_ERROR("Unable to open audio device.");
    return false;
  }
#endif
  Mix_AllocateChannels(64);

  updateSfxVolume();
  updateMusicVolume();
  return true;
}

void SoundManager::shutdown()
{
  Mix_CloseAudio();
  Mix_Quit();
  LOG_INFO("Sound system shutdown");
}

void SoundManager::music(uint32_t songno, bool resume)
{
  if (songno == _currentSong)
    return;

  _lastSong    = _currentSong;
  _currentSong = songno;

  LOG_DEBUG(" >> music({})", songno);

  _songlooped  = Ogg::getInstance()->looped();
  _lastSongPos = Ogg::getInstance()->stop();

  Ogg::getInstance()->start("test", "music/", resume ? _lastSongPos : 0, resume ? _songlooped : false, false);

}

void SoundManager::enableMusic(int newstate)
{
  if (newstate != Settings::getInstance()->audioEnabled())
  {
    LOG_DEBUG("enableMusic({})", newstate);

    Settings::getInstance()->setAudioEnabled(newstate);
  }
}

void SoundManager::fadeMusic()
{
  Ogg::getInstance()->fade();
}

void SoundManager::runFade()
{
  Ogg::getInstance()->runFade();
}

void SoundManager::pause()
{
  Mix_Pause(-1);
  Ogg::getInstance()->pause();
}

void SoundManager::resume()
{
  Mix_Resume(-1);
  Ogg::getInstance()->resume();
}

void SoundManager::updateSfxVolume()
{
  Mix_Volume(-1, (int)(128. / 100. * Settings::getInstance()->sfxVolume()));
}

void SoundManager::updateMusicVolume()
{
  Ogg::getInstance()->updateVolume();
}

} // namespace Sound
} // namespace Ironhead
