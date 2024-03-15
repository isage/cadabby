#include "../State/Start.h"

#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
#include "../ResourceManager.h"
#include "../Settings.h"
#include "../Utils/Logger.h"
#include "../State/FadeOut.h"
#include "../State/Exit.h"
#include "../Sound/SoundManager.h"
#include "../common/misc.h"

#if defined(__VITA__)
    #include <psp2/power.h>
    #include <psp2/kernel/processmgr.h>
#endif


using namespace Ironhead::Graphics;

namespace Ironhead
{
    namespace State
    {

        Start::Start() : State()
        {
        }

        Start::~Start()
        {
        }

        void Start::init()
        {
            if (_initialized) return;
            State::init();

            setModal(true);
            setFullscreen(true);

            _logoY = 0;
            _logoX = 0;
            _battY = 0;
            _battX = 960;
            _ticks = 0;
            scePowerSetConfigurationMode(SCE_POWER_CONFIGURATION_MODE_C);
        }

        void Start::think(uint32_t delta)
        {
            State::think(delta);

            _ticks += delta;

            if (_dy > 0)
                INC_TIMES_PER_DELTA(_logoY,2);
            else
                DEC_TIMES_PER_DELTA(_logoY,2);

            if (_logoY >= 544-40)
            {
                _dy = -1;
                _r = rand() % 106 + 150;
                _g = rand() % 106 + 150;
                _b = rand() % 106 + 150;

                _bgr = rand() % 106;
                _bgg = rand() % 106;
                _bgb = rand() % 106;
            }
            if (_logoY <= 0)
            {
                _dy = 1;
                _r = rand() % 106 + 150;
                _g = rand() % 106 + 150;
                _b = rand() % 106 + 150;

                _bgr = rand() % 106;
                _bgg = rand() % 106;
                _bgb = rand() % 106;
            }

            if (_dx > 0)
                INC_TIMES_PER_DELTA(_logoX, 2);
            else
                DEC_TIMES_PER_DELTA(_logoX, 2);

            if (_logoX >= 960-78)
            {
                _dx = -1;
                _r = rand() % 256;
                _g = rand() % 256;
                _b = rand() % 256;

                _bgr = rand() % 106;
                _bgg = rand() % 106;
                _bgb = rand() % 106;
            }
            if (_logoX <= 0)
            {
                _dx = 1;
                _r = rand() % 256;
                _g = rand() % 256;
                _b = rand() % 256;

                _bgr = rand() % 106;
                _bgg = rand() % 106;
                _bgb = rand() % 106;
            }


            if (_bdy > 0)
                INC_PER_DELTA(_battY);
            else
                DEC_PER_DELTA(_battY);

            if (_battY >= 544-32) _bdy = -1;
            if (_battY <= 0) _bdy = 1;

            if (_bdx > 0)
                INC_PER_DELTA(_battX);
            else
                DEC_PER_DELTA(_battX);

            if (_battX >= 960-60) _bdx = -1;
            if (_battX <= 0) _bdx = 1;

#if defined(__VITA__)
            sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DEFAULT);
#endif
        }

        void Start::render()
        {
            Renderer::getInstance()->clearScreen(_bgr,_bgg,_bgb);

            Renderer::getInstance()->sprites.drawColoredSprite("Ship", _logoX, _logoY, 0, 255, _r, _g, _b, 0);

#if defined(__VITA__)
            char percent[256];
            snprintf(percent, 255, "%d%%", scePowerGetBatteryLifePercent());

            char voltage[256];
            snprintf(voltage, 255, "%dmV", scePowerGetBatteryVolt());

            Renderer::getInstance()->font.draw(_battX, _battY, percent);
            Renderer::getInstance()->font.draw(_battX, _battY+16, voltage);
#endif
        }

        void Start::onKeyDown(Event::Keyboard* event)
        {
        }

        void Start::onButtonDown(Event::Gamepad* event)
        {
        }

        void Start::onAxis(Event::Gamepad* event)
        {
        }
    }
}
