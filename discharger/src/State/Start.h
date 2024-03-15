#ifndef STATE_START_H
#define STATE_START_H

#include "../State/State.h"
#include <vector>
#include <queue>

namespace Ironhead
{
    namespace State
    {

        class Start : public State
        {
            public:
                Start();
                ~Start() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;
                void onKeyDown(Event::Keyboard* event) override;
                void onButtonDown(Event::Gamepad* event) override;
                void onAxis(Event::Gamepad* event) override;

            protected:
                float _logoX;
                float _logoY;
                int _dx = 1;
                int _dy = 1;

                float _battX;
                float _battY;
                int _bdx = -1;
                int _bdy = 1;
                uint32_t _ticks = 0;
                int _r = 255;
                int _g = 0;
                int _b = 0;

                int _bgr = 0;
                int _bgg = 0;
                int _bgb = 0;
        };
    }
}
#endif // STATE_START_H
