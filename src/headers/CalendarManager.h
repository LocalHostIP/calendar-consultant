#ifndef CalendarManager_H_
#define CalendarManager_H_

#include <map>
#include <iostream>
#include <fstream>
#include "../../include/chilkat/CkRest.h"
#include "../../include/chilkat/CkOAuth2.h"
#include "../../include/chilkat/CkStringBuilder.h"
#include "../../include/chilkat/CkJsonObject.h"

class CalendarManager{
    CkRest rest;
    CkOAuth2 oauth2;
    CkJsonObject jsConfig;
    std::string config_path;

    bool loadConfig();

    bool refreshToken();

public:
    void getCalendars();
    CalendarManager(const std::string &config_path);
};

#endif