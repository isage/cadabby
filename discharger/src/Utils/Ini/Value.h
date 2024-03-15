#ifndef INI_VALUE_H
#define INI_VALUE_H

#include <string>
#include <iosfwd>
#include <vector>

namespace Ironhead
{
namespace Utils
{
namespace Ini
{

class Value
{
public:
    enum class Tag
    {
        DOUBLE,
        INTEGER,
        BOOLEAN,
        STRING,
        ARRAY
    };

    Value();
    Value(double doubleVal);
    Value(int integerVal);
    Value(bool booleanVal);
    Value(const std::string& stringVal);
    Value(const Value &rhs);
    Value(std::vector<Value> iniVal);
    ~Value();

    Value & operator=(const Value &rhs);

    Value::Tag tag() const;
    static std::string tagString(Value::Tag tag);
    std::string value() const;
    int intValue() const;
    bool boolValue() const;
    double doubleValue() const;

    friend class Section;
private:
    Tag _tag;

    union
    {
        double _doubleVal;
        int _integerVal;
        bool _booleanVal;
        std::string _stringVal;
        std::vector<Value> _iniVal;
    };
};

}
}
}
#endif // INI_VALUE_H
