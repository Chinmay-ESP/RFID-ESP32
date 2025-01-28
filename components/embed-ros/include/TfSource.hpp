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


#ifndef TF_SOURCE_HPP
#define TF_SOURCE_HPP
#include <geometry_msgs/msg/transform_stamped.h>

/**
 * @brief transform source
 * 
 * various subclassed of this base class provides TF (ros2 TF2) data
 * to be used with tfPub
 */
class TfSource {    

public:
    geometry_msgs__msg__TransformStamped* transform = new geometry_msgs__msg__TransformStamped();

    
    TfSource& operator=(geometry_msgs__msg__TransformStamped*);

private:
    bool internal_mem = true;

};

#endif //  TF_SOURCE_HPP
