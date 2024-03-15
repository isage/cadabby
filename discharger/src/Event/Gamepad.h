#ifndef EVENT_GAMEPAD_H
#define EVENT_GAMEPAD_H

#include "../Event/Event.h"
#include <SDL.h>

namespace Ironhead
{
    namespace Event
    {
        class Gamepad : public Event
        {
            public:
                enum class Type
                {
                    BUTTON_DOWN,
                    BUTTON_UP,
                    AXIS
                };

                static const char* typeToString(Type);

                Gamepad(Type type);
                Gamepad(const Gamepad& event, const std::string& newName);
                Gamepad(const Gamepad& event);
                ~Gamepad() override;

                /**
                 * @brief Type of an original event from OS.
                 */
                Type originalType() const;

                int button() const;
                void setButton(int value);

                int axis() const;
                void setAxis(int value);
                int axisValue() const;
                void setAxisValue(int value);

                int getAxisValue(SDL_GameControllerAxis axis) const;

                int gamepad() const;
                void setGamepad(int value);

            protected:
                int _gamepad = 0;
                int _button = 0;
                int _axis = 0;
                int _axisvalue = 0;

                Type _type;
        };
    }
}
#endif // EVENT_GAMEPAD_H
