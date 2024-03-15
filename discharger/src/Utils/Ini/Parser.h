#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <memory>
#include <string>
#include <vector>

namespace Ironhead
{
namespace Utils
{
namespace Ini
{

class File;
class Value;

class Parser
{
public:
    Parser(std::istream &stream);
    ~Parser();

    std::shared_ptr<File> parse();

private:
    std::istream &_stream; // stream to parse
    std::string  _section; // current section

protected:
    void _trim(std::string &line);

    void _rtrim(std::string &line);

    void _ltrim(std::string &line);

    void _toLower(std::string &line);

    bool _tryBool(std::string &line, bool* val);

    int _tryDecimal(std::string &line, int* intval, double* doubleval);

    bool _parseBool(std::string &name, std::string &line, std::shared_ptr<File> ini);

    bool _parseDecimal(std::string &name, std::string &line, std::shared_ptr<File> ini);

    bool _parseArray(std::string &name, std::string &line, std::shared_ptr<File> ini);

    bool _parseArrayDecimal(std::vector<Value> &vec, std::string val);
    bool _parseArrayBool(std::vector<Value> &vec, std::string val);

};

}
}
}
#endif // INI_PARSER_H
