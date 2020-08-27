class FactoryMap:
    """
    Contains the code for the map of object types to factory methods.
    """

    def __init__(self):
        self.includes = """
#include "nifactory.hpp"
#include <string>
#include <unordered_map>
#include "nifreader.hpp"
        """
        self.cpp_prefix = """
std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::createCreatorMap() {
        std::unordered_map<std::string, NiObject* (*)(NifReader&)> m;
        """
        self.cpp_suffix = """
        return m;
    }

const std::unordered_map<std::string, NiObject* (*)(NifReader&)> NiFactory::creatorMap = createCreatorMap();
        """
        self.cpp = ''

    
    def append_module(self, module_name):
        """
        Registers a new module name.
        Registered modules are entered into the include directives.
        The map must include all modules.
        """
        self.includes += '#include "' + module_name + '/' + module_name + '.hpp"\n'

    def append_mapping(self, name):
        """
        Registers a factory method for the given object.
        The map must register all objects.
        """
        self.cpp += 'm["'+name+'"] = &'+name+'::create;\n'


    def get_hpp(self):
        """
        Gets the header file code of the map.
        
        Return:
            str
        """
        return """
#pragma once

#include <unordered_map>


class NifReader;
class NiObject;

struct NiFactory {
private:
    static std::unordered_map<std::string, NiObject* (*)(NifReader&)> createCreatorMap();
public:
    static const std::unordered_map<std::string, NiObject* (*)(NifReader&)> creatorMap;
};
"""

    def get_cpp(self):
        """
        Gets the compilation unit code for the map.

        Return:
            str
        """
        return self.includes + self.cpp_prefix + self.cpp + self.cpp_suffix

