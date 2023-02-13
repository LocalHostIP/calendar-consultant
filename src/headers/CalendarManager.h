#ifndef CalendarManager_H_
#define CalendarManager_H_

#include <map>
#include <iostream>
#include <fstream>
#include <string.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../../include/chilkat/CkRest.h"
#include "../../include/chilkat/CkOAuth2.h"
#include "../../include/chilkat/CkStringBuilder.h"
#include "../../include/chilkat/CkJsonObject.h"
#include "CalendarEvent.h"

class CalendarManager{
    CkRest rest;
    CkOAuth2 oauth2;
    CkJsonObject jsConfig;
    std::string config_path;
    std::string user_calendar;

    bool loadConfig();

    bool refreshToken();

public:
    std::vector<CalendarEvent> getUpcommingEvents(int maxEvents);
    CalendarManager(const std::string &config_path);
    bool deleteEvent(CalendarEvent event);
};

#endif