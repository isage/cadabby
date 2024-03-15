#include "../Event/Gamepad.h"
#include "../Utils/Logger.h"

namespace Ironhead
{
    namespace Event
    {
        Gamepad::Gamepad(Gamepad::Type type) : Event(typeToString(type))
        {
            _type = type;
        }

        Gamepad::Gamepad(const Gamepad& event, const std::string& newName) : Event(newName)
        {
            _button = event._button;
            _axis = event._axis;
            _axisvalue = event._axisvalue;
            _gamepad = event._gamepad;
        }

        Gamepad::Gamepad(const Gamepad& event) : Gamepad(event, event._name)
        {
        }

        Gamepad::~Gamepad()
        {
        }

        const char* Gamepad::typeToString(Gamepad::Type type)
        {
            switch (type)
            {
                case Type::BUTTON_DOWN: return "buttdown";
                case Type::BUTTON_UP:   return "buttup";
                case Type::AXIS:        return "axis";
                default: return "Gamepad";
            }
        }

        Gamepad::Type Gamepad::originalType() const
        {
            return _type;
        }

        int Gamepad::button() const
        {
            return _button;
        }

        void Gamepad::setButton(int value)
        {
            _button = value;
        }

        int Gamepad::axis() const
        {
            return _axis;
        }

        void Gamepad::setAxis(int value)
        {
            _axis = value;
        }

        int Gamepad::axisValue() const
        {
            return _axisvalue;
        }

        int Gamepad::getAxisValue(SDL_GameControllerAxis axis) const
        {
            SDL_GameController* c = SDL_GameControllerFromInstanceID(_gamepad);
            return SDL_GameControllerGetAxis(c, axis);
        }

        void Gamepad::setAxisValue(int value)
        {
            _axisvalue = value;
        }

        int Gamepad::gamepad() const
        {
            return _gamepad;
        }

        void Gamepad::setGamepad(int value)
        {
            _gamepad = value;
        }

    }
}
