#ifndef STATE_EXIT_H
#define STATE_EXIT_H

#include "../State/State.h"

namespace Ironhead
{
    namespace State
    {
        class Exit : public State
        {
            public:
                Exit();
                ~Exit() override;

                void think(uint32_t delta) override;
                void init() override;
                void onStateActivate(Event::State* event) override;
                void onStateDeactivate(Event::State* event) override;
        };
    }
}
#endif // STATE_EXIT_H
