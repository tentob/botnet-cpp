#include "botnet.hpp"

Botnet::Botnet(char* prefix) {
    this->prefix = prefix;
}

Botnet::~Botnet() {
    free(prefix);
}
