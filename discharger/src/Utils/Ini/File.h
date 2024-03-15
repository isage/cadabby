#ifndef INI_FILE_H
#define	INI_FILE_H

// C++ standard includes
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <sstream>
#include <algorithm>

// Falltergeist includes
#include "../Ini/Parser.h"
#include "../Ini/Section.h"

// Third party includes

namespace Ironhead
{
namespace Utils
{

namespace Ini
{

class File
{
public:
    using iterator = std::map<std::string, std::shared_ptr<Section>>::iterator;
    using const_iterator = std::map<std::string, std::shared_ptr<Section>>::const_iterator;

    File();
    ~File();
    std::shared_ptr<Section> section(const std::string &name);
    std::map<std::string, std::shared_ptr<Section>>* sections();

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    bool hasSection(const std::string &name) const;

private:
    std::map<std::string, std::shared_ptr<Section>> _sections;

};

}
}
}
#endif	// INI_FILE_H
