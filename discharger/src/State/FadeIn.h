#ifndef STATE_FADEIN_H
#define STATE_FADEIN_H

#include "../State/State.h"

namespace Ironhead
{
    namespace State
    {
        class FadeIn : public State
        {
            public:
                FadeIn();
                ~FadeIn() override;

                void think(uint32_t delta) override;
                void init() override;
                void render() override;

            protected:
                float _timer = 0;
        };
    }
}
#endif // STATE_FADEIN_H
