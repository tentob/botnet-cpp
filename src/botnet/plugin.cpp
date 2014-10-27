#include "plugin.hpp"

Plugin::Plugin(char* path) {
    this->path = realpath(path, NULL); 
    load();    
}

Plugin::~Plugin() {
    free(path);
    unload();
}

void Plugin::unload() {
    if(handle)
        dlclose(handle);
}

void Plugin::load() {
    if(path) {
        handle = dlopen(path, RTLD_LOCAL | RTLD_NOW);
        if(!handle)
            std::cerr << "Couldnt load plugin" << std::endl; 
        else regen_symbols();
    }
}

char* Plugin::getPath() {
    return path;
}

void Plugin::setPath(char* path) {
    free(this->path);
    this->path = realpath(path, NULL); 
}

void* Plugin::func(char* name) {
    void* func = dlsym(handle, name);
    if (!func) {
        std::cerr << "Cannot load symbol '" << name <<"': " << dlerror() << std::endl;
        return NULL;
    }
    return func;
}

bool Plugin::symbolExists(char* name) {
    if(std::find(symbols.begin(), symbols.end(), name) != symbols.end())
        return true;
    return false;
}

std::vector<char*> Plugin::getSymbols() {
    return symbols;
}

size_t Plugin::findVal(ElfW(Dyn)* dyn, ElfW(Sxword) tag) {
    for(; dyn->d_tag != DT_NULL; ++dyn) {
        if(dyn->d_tag == tag)
            return dyn->d_un.d_val;
    }
    return 0;
}

void* Plugin::findPtr(ElfW(Dyn)* dyn, ElfW(Sxword) tag) {
    for (; dyn->d_tag != DT_NULL; ++dyn) {
        if (dyn->d_tag == tag)
            return (void*)(dyn->d_un.d_ptr);
    }
    return 0;
}


void Plugin::regen_symbols() {
    symbols.clear();
    struct link_map* map = 0;
    if(dlinfo(handle, RTLD_DI_LINKMAP, &map)) {
        std::cerr << "Couldnt generate link_map" << std::endl;
        return;
    }
    ElfW(Dyn)* dyn_start = map->l_ld;
    size_t symentsize = findVal(dyn_start, DT_SYMENT);
    ElfW(Sym)* syment = (ElfW(Sym)*) findVal(dyn_start, DT_SYMTAB);
    if(syment == NULL) {
        std::cerr << "Couldnt get syment" << std::endl;
    }
    char* strtab = (char*) findPtr(dyn_start, DT_STRTAB);

    while(syment != NULL) {
        if(syment->st_name > 0x1000000)
            break;
        if(syment->st_size) {
            symbols.push_back(&strtab[syment->st_name]);
        }
        syment = (ElfW(Sym)*)((uint8_t*)syment + symentsize);
    }
}
