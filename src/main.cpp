#include <iostream>
#include "headers/CalendarManager.h"
#include "headers/CalendarEvent.h"
#include "headers/SerialCom.h"
#include <vector>

int main(){
    //Check UPcomming events from Calendar
    CalendarManager cm{"./../config/config.json"};
    std::vector<CalendarEvent> events = cm.getUpcommingEvents(1);
    std::string send_buffer = events[0].getSummary()+"\t"+ events[0].getTimeStartDate() + " " + events[0].getTimeStartHour();
    std::cout << "Data to send: " << send_buffer << "\n";
    //Send Events to Microcontroller
    ///dev/ttyACM0
    SerialCom serial("dev/ttyACM0");
    
    //Set Config to Wait until receibe one byte
    serial.setReadConfig(1,0);

    while(true){
        //Wait until recibe data from Micro
        std::cout << "Waiting for data from STM32... \n";
        std::string r_data = serial.readData();
        std::cout << "Recibed data: " << r_data << "\n";
        //Send Data
        serial.writeData(send_buffer);
    }
    
    serial.closeConnection();

    return 0;
}