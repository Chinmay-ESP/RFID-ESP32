#ifndef RFID_PUB_HPP
#define RFID_PUB_HPP

#include "urosElement.hpp"
#include <std_msgs/msg/int32.h>
#include "esp_log.h"
#include <rc522.hpp>


class simPub : public urosElement {

    public:

        const static char* pub;
        simPub();
        
        // microros initialization
        void init();
        void declareParameters() {};


        // publisher callback functions
        // static void timer_callback(rcl_timer_t* timer, int32_t num);

        static void on_read_event(void *arg, esp_event_base_t base, int32_t event_id, void *data);

    private:

        rcl_timer_t timer;
        rcl_publisher_t simple_publisher;
        int frequency = 10;
        std_msgs__msg__Int32 msg;

        rc522_handle_t scanner;

        static simPub* current;
};

#endif