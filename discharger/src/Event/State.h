#ifndef EVENT_STATE_H
#define EVENT_STATE_H

#include "../Event/Event.h"

namespace Ironhead
{
    namespace Event
    {
        class State : public Event
        {
            public:
                State(const std::string& name);
                ~State() override;
        };
    }
}
#endif // EVENT_STATE_H
