#include "headers/CalendarEvent.h"

CalendarEvent::CalendarEvent(const char * id,const char * time_created,const char * summary){
    this->time_created = std::string(time_created);
    this->id = std::string(id);
    this->summary = std::string(summary);
}

std::string CalendarEvent::getId(){
    return id;
}

std::string CalendarEvent::getSummary(){
    return summary;
}

std::string CalendarEvent::getTimeStartDate(){
    //Get Only date with no hours, year with 2 digits only
    //(2023-02-07T12:38:00.000Z) -> 23-02-07
    if (is_time_start==false){
        return "(No Time)";
    }
    else{
        return time_start.substr(2,8);
    }
}

std::string CalendarEvent::getTimeStartHour(){
    if (is_time_start==false){
        return "";
    }
    else{
        return time_start.substr(11,5);
    }
}

void CalendarEvent::setTimeStart(const char * timeStart){
    this->time_start = std::string(timeStart);
    is_time_start = true;
}

void CalendarEvent::setTimeEnd(const char * timeEnd){
    this->time_end = std::string(timeEnd);
    is_time_end = true;
}

void CalendarEvent::print(){
    std::cout << "Summary: \t\t" << summary << "\n";
    if (is_time_start){
        std::cout << "Start Time: \t\t" << time_start << "\n";
    }
    if (is_time_end){
        std::cout << "End Time: \t\t" << time_end << "\n";
    }
    std::cout << "Creation Time: \t\t"<<time_created << "\n";
}