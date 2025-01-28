// embed-ros - ros2 utilities for embedded systems 
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

#ifndef UROS_ELEMENT_HPP
#define UROS_ELEMENT_HPP

#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <vector>

#include <rclc_parameter/rclc_parameter.h>



struct paramDeclaration {
    rclc_parameter_type_t type;
    const char* name;
    const char* description;
};


class urosElement {
    
    // urosElement(std::vector<paramDeclaration>&);

public:
    rclc_support_t* support = 0;
    rclc_executor_t* exec = 0;
    rcl_node_t* node = 0;
    rcl_allocator_t* alloc = 0;


    void setupParameters(std::vector<paramDeclaration>& params);


    

    virtual void init() = 0;

};



#endif // UROS_ELEMENT_HPP
