#pragma once

#include <iostream>
#include "botnet/plugin.hpp"

class Botnet {
public:
    Botnet(char* prefix);
    ~Botnet();
private:
    char* prefix;
};
