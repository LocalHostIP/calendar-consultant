#include "headers/CalendarEvent.h"

CalendarEvent::CalendarEvent(const char * id,const char * time_created,const char * summary){
    this->time_created = std::string(time_created);
    this->id = std::string(id);
    this->summary = std::string(summary);
}

std::string CalendarEvent::getId(){
    return id;
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