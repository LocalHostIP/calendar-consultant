#ifndef CalendarManager_H_
#define CalendarManager_H_

#include <map>
#include <iostream>
#include <fstream>

class CalendarManager{
    std::map <std::string,std::string> config;

    void LoadConfig(const std::string &path);

public:
    CalendarManager(const std::string &config_path);
};

#endif