#include "../State/FadeOut.h"

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
        FadeOut::FadeOut() : State()
        {
        }

        FadeOut::~FadeOut()
        {
        }

        void FadeOut::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(false);
        }

        void FadeOut::think(uint32_t delta)
        {
            auto game = Game::Game::getInstance();
            State::think(delta);

            INC_PER_DELTA(_timer);

            if (_timer > 50.)
            {
                game->popState();
            }
        }

        void FadeOut::render()
        {
            int x, y;
            int startframe = 0;

//            if (_timer > 50.) return;

            for(y = Renderer::getInstance()->screenHeight - 16; y >= 0; y -= 16)
            {
                int f = 0;

                for(x = 0; x < Renderer::getInstance()->screenWidth; x+=16)
                {
                        int frame = (static_cast<int>(_timer) - startframe - f);

                        if (frame < 0) frame = 0;
                        if (frame > 15) frame = 15;

                        if (frame)
                            Renderer::getInstance()->sprites.drawSprite("Fade", x, y, frame);

                        f++;
                }

                startframe++;
            }
        }



    }
}
