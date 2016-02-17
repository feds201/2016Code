// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#ifndef INIREADER_H
#define INIREADER_H

#include <map>
#include <string>

class INIReader
{
public:
    INIReader(std::string filename);
    std::string get(std::string sectionName, std::string keyName);
    int getInt(std::string sectionName, std::string keyName);
    bool getBool(std::string sectionName, std::string keyName);
    float getFloat(std::string sectionName, std::string keyName);

private:
    std::map<std::string, std::map<std::string, std::string>> values;
};

#endif //INIREADER_H
