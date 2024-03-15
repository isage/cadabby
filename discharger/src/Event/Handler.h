#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../Base/Delegate.h"

namespace Ironhead
{
namespace Event
{
class Event;
class Mouse;
class Keyboard;
class Gamepad;
class State;

using Handler = Base::Delegate<Event*>;
using MouseHandler = Base::Delegate<Mouse*>;
using KeyboardHandler = Base::Delegate<Keyboard*>;
using GamepadHandler = Base::Delegate<Gamepad*>;
using StateHandler = Base::Delegate<State*>;

}
}

#endif //EVENT_HANDLER_H
