#ifndef INI_WRITER_H
#define INI_WRITER_H

#include <ostream>

namespace Ironhead
{
namespace Utils
{
namespace Ini
{
class File;

class Writer
{
public:
    Writer(const File &ini);
    ~Writer();

    void write(std::ostream &stream);

private:
    const File &_ini;
};

}
}
}
#endif // INI_WRITER_H
