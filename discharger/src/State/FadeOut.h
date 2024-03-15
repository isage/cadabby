#ifndef STATE_FADEOUT_H
#define STATE_FADEOUT_H

#include "../State/State.h"

namespace Ironhead
{
    namespace State
    {
        class FadeOut : public State
        {
            public:
                FadeOut();
                ~FadeOut() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;

            protected:
                float _timer = 0;
        };
    }
}
#endif // STATE_FADEOUT_H
