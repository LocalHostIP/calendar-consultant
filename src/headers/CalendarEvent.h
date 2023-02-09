#ifndef CalendarEvent_H_
#define CalendarEvent_H_
#include <iostream>

class CalendarEvent{
    std::string time_start;
    std::string time_end;
    std::string time_created;
    std::string summary;
    std::string id;
    bool is_time_start = false;
    bool is_time_end = false;
    bool is_summary = false;
public:
    CalendarEvent(const char * id,const char * time_created,const char * summary);
    void setTimeStart(const char * timeStart);  
    void setTimeEnd(const char * timeEnd);
    
    std::string getSummary();
    std::string getId();
    std::string getTimeStartDate();
    std::string getTimeStartHour();
    std::string getTimeEndDate();
    std::string getTimeEndTime();

    void print();
};

#endif