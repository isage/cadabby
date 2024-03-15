#ifndef SOUND_SOUNDMANAGER_H_
#define SOUND_SOUNDMANAGER_H_

#include "../Base/Singleton.h"

#include <SDL_mixer.h>
#include <cstdint>
#include <string>
#include <vector>

#define SAMPLE_RATE 44100

namespace Ironhead
{
namespace Sound
{


class SoundManager
{

public:
  static SoundManager *getInstance();

  bool init();
  void shutdown();

  void music(uint32_t songno, bool resume = false);
  void enableMusic(int newstate);

  void fadeMusic();
  void runFade();
  void pause();
  void resume();
  void updateMusicVolume();
  void updateSfxVolume();

protected:
  friend class Base::Singleton<SoundManager>;

  SoundManager();
  ~SoundManager();
  SoundManager(const SoundManager &) = delete;
  SoundManager &operator=(const SoundManager &) = delete;

private:

  uint32_t _lastSong    = 0;
  uint32_t _lastSongPos = 0;
  uint32_t _currentSong = 0;
  bool _songlooped      = false;

};



} // namespace Sound
} // namespace Ironhead
#endif
