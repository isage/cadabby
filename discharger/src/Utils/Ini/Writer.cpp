#include "../Ini/Writer.h"

#include "../Ini/File.h"

namespace Ironhead
{
namespace Utils
{
namespace Ini
{

Writer::Writer(const File &ini) : _ini(ini)
{
}

Writer::~Writer()
{
}

void Writer::write(std::ostream &stream)
{
    stream << "# This file contains default configuration for the ironhead engine." << std::endl;

    for (auto &section : _ini)
    {
        stream << "[" << section.first << "]" << std::endl;

        for (auto &properties : *section.second)
        {
            stream << properties.first <<  "=" << properties.second.value() << std::endl;
        }

        stream << std::endl;
    }
}

}
}
}
