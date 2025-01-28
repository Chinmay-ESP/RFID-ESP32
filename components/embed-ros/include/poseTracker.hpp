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


#ifndef POSE_TRACKER_HPP
#define POSE_TRACKER_HPP
#include "TfSource.hpp"

#include <esp_dsp.h>


enum driveType {
    MECANUM = 0,
    OMNI,
    DIFFRENTIAL,
};

class poseTracker : public TfSource {

public:

    struct robotInfo {
        driveType type;
        float wheelRadius;
        float wheelDistance;
        float pulseCount;

        float* pulseSource;
        int wheelCount;
    };

    poseTracker(robotInfo&);

    TfSource* getWheelTfs(int index) { return &wheelTFs[index]; }

    void update();

    void startTrackingThread();

private:
    
    robotInfo info;

    TfSource wheelTFs[4];


    float state[3] = {0}, descale = 1.0f;
    float &x = state[0], &y = state[1], &w = state[2];
    dspm::Mat res, tracking, drive, R, dR, prevR;

    void updateTrackingMat(float w);
};


#endif //  POSE_TRACKER_HPP