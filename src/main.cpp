#include <iostream>
#include <stdlib.h>
#include <boost/program_options.hpp>

#include "botnet.hpp"

int main(int argc, char** argv) {
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "print this message")
        ("daemonize,d", "run as daemon")
        ("prefix,p", po::value<std::string>()->default_value("."), "prefix path")
        ;
    namespace po = boost::program_options;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    
    } catch (po::error e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    char* prefix = realpath(vm["prefix"].as<std::string>().c_str(), NULL);   
    Botnet botnet(prefix);

    return 0;
}
