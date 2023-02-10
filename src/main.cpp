#include <iostream>
#include <vector>
#include <thread>
#include <exception>
#include <chrono>

#include "headers/CalendarManager.h"
#include "headers/CalendarEvent.h"
#include "headers/SerialCom.h"

using namespace std;

#define MAX_EVENTS 5
#define MAX_LENGTH_BUFFER_SEND 300

std::string getEventsForSending(CalendarManager &cm){
    vector<CalendarEvent> events = cm.getUpcommingEvents(MAX_EVENTS);
    string send_buffer = "";
    for (auto e:events){
        send_buffer += e.getSummary()+"\t"+ e.getTimeStartDate() + " " + e.getTimeStartHour()+"\t";
    }
    if (send_buffer.length()>MAX_LENGTH_BUFFER_SEND){
        send_buffer=send_buffer.substr(0,MAX_LENGTH_BUFFER_SEND-1);
    }
    cout << "Data to send: " << send_buffer << "\n";
    return send_buffer;
}

int main(){
    //Check UPcomming events from Calendar
    CalendarManager cm{"./../config/config.json"};
    string send_buffer = getEventsForSending(cm);
    auto time_lastUpdate = chrono::steady_clock::now();

    //Send Events to Microcontroller
    ///dev/ttyACM0
    int const timeout=10;
    int const maxChar=0;
    auto ctime = chrono::steady_clock::now();
    int passed_time;
    const int next_time = 60;

    try{
        SerialCom serial("dev/ttyACM0");

        //Set Config to Wait until receibe one byte
        serial.setReadConfig(maxChar,timeout);

        while(true){
            //Wait until recibe data from Micro
            cout << "Waiting for data from STM32... \n";
            string r_data = serial.readData();
            if (r_data.length()>0){
                cout << "Recibed data: " << r_data << "\n";
                cout << "Sending events \n";
                //Send Data
                serial.writeData(send_buffer);
            }

            ctime = std::chrono::steady_clock::now();
            passed_time = chrono::duration_cast<chrono::seconds>(ctime - time_lastUpdate).count();
            //If enough time has passed then update events
            if (passed_time >= next_time){
                cout << "\nGetting new upcoming events\n";
                send_buffer = getEventsForSending(cm);
                time_lastUpdate = chrono::steady_clock::now();
            }
        }
        
        serial.closeConnection();

    }catch(const std::runtime_error &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
} 