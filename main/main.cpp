#include "rc522.hpp"
#include <iostream>
#include "rfid_pub.hpp"
#include "urosHandler.hpp"
#include <rcl/error_handling.h>
#include <rc522.hpp>


urosHandler* handler;


extern "C" void app_main()
{
    // Initialize Micro-ROS handler
    handler = new urosHandler();


    // Publisher for RFID data
    simPub* pub = new simPub();
    

    // Add the publisher to the thread executor
    handler->addThreadExecutor({
        pub
    }, APP_CPU_NUM);
    
    

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
        
}