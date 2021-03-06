#pragma once

#include <iostream>
#include <dlfcn.h>
#include <link.h>
#include <algorithm>
#include <vector>
#include <stdlib.h>

struct Plugin {
public:
    Plugin(char* path);
    ~Plugin();
    
    void* func(char* name);
    bool symbolExists(char* sym);
    std::vector<char*> getSymbols(); 
    void unload();
    void load();
    char* getPath();
    void setPath(char* path);
private:
    void regen_symbols();
    size_t findVal(ElfW(Dyn)* dyn, ElfW(Sxword) tag);
    void* findPtr(ElfW(Dyn)* dyn, ElfW(Sxword) tag);
    std::vector<char*> symbols;
    void *handle;
    char* path;
};
