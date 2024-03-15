#include "../State/Exit.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"


using namespace Ironhead::Graphics;

namespace Ironhead
{
    namespace State
    {
        Exit::Exit() : State()
        {
        }

        Exit::~Exit()
        {
        }

        void Exit::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(false);
        }

        void Exit::think(uint32_t delta)
        {
            auto game = Game::Game::getInstance();
            State::think(delta);
            if (game->topState() == this)
                game->quit();
        }

        void Exit::onStateActivate(Event::State* event)
        {
            LOG_DEBUG("activated");
        }

        void Exit::onStateDeactivate(Event::State* event)
        {
            LOG_DEBUG("deactivated");
        }
    }
}
