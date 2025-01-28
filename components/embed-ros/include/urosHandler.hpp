// embed-ros - ros2utilities for embedded systems 
// Copyright (C) 2023  akshay bansod <akshayb@gmx.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef UROS_HANDLER_HPP
#define UROS_HANDLER_HPP

#include <string>
#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>  

#include "urosElement.hpp"


#ifndef UROS_OVER_USB
#define UROS_OVER_USB 0
#endif

/**
 * @brief urosHandler to manage micro ros initialization
 */
class urosHandler {

public:

    /**
     * @brief Construct a new uros Handler object
     * 
     * @param node_name ros2 node name
     * @param usb use USB for communication
     */
    urosHandler(std::string node_name = "uros_node");

    /**
     * @brief create a rclc executor on a new thread   
     * 
     * @param elements vector of urosElement pointer to run on these threaded executor
     * @param CPUID  cpuid to run executor thread 
     */
    void addThreadExecutor(std::vector<urosElement*> elements, int CPUID = PRO_CPU_NUM);


private:


    static QueueHandle_t m_urosAccess;
    static void executorTask(void* param);

    rcl_allocator_t allocator;
    rclc_support_t support;
    rmw_init_options_t* rmw_options;
    rcl_node_t node;
};

    

#endif // UROS_HANDLER_HPP
