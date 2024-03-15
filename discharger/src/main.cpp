// C++ standard includes
#include <memory>
#include <SDL.h>

#include "Game/Game.h"
#include "Utils/Logger.h"
#include "Settings.h"
#include "ResourceManager.h"
#include "State/Start.h"

// Third party includes

using namespace Ironhead;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Utils::Logger::init(ResourceManager::getInstance()->getPrefPath("debug.log"));

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                  LOG_INFO("Opened controller {}", i);
                  LOG_INFO("Name: {}", SDL_GameControllerName(controller));
                  LOG_INFO("Mapping: {}", SDL_GameControllerMapping(controller));
                  SDL_Joystick* joy = SDL_GameControllerGetJoystick(controller);
                  SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(joy);
                  if (haptic == NULL)
                  {
                    LOG_INFO("No force feedback support");
                  }
//                  else
//                  {
//                    if (SDL_HapticRumbleInit(haptic) != 0)
//                      LOG_WARN("Coiuldn't init simple rumble");
//                  }
            }
        }
    }

    auto game = Game::Game::getInstance();
    game->init();

    game->setState(new State::Start());

    game->run();
    game->shutdown();
    return 0;
}

