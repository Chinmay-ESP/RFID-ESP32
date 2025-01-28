
#include <math.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <esp_log.h>

#include "poseTracker.hpp"

#define LOG "POSETRACKER"

float mecanumDriveMatElems[] = {
    0.5f, 0.0f, 0.0f, 0.5f,
    0.0f, 0.5f, 0.5f, 0.0f,
    -0.25f, 0.25f, -0.25f, 0.25f};

dspm::Mat mecanumDrive(mecanumDriveMatElems, 3, 4);

poseTracker::poseTracker(robotInfo &inf) : info(inf), res(state, 3, 1), tracking(3, 3)
{
    tracking.eye(3);

    switch (inf.type)
    {

    case MECANUM:
        dR = dspm::Mat(4, 1);
        prevR = dspm::Mat(4, 1);
        R = dspm::Mat(4, 1);
        drive = mecanumDrive;
        break;

    // do nothing for all other cases
    default:
        break;
    }
}


void poseTracker::update()
{
    // while (1)
    // {
        memcpy(R.data, info.pulseSource, info.wheelCount * sizeof(float));

        ESP_LOGI(LOG, "pulse count %f %f %f %f", R.data[0], R.data[1], R.data[2], R.data[3]); 
        // convert pulses into rotation in radians
        R *= 2.0f * M_PI / info.pulseCount;

        ESP_LOGI(LOG, "radians count %f %f %f %f", R.data[0], R.data[1], R.data[2], R.data[3]); 
        for(int i = 0; i < 4; i++){
            (wheelTFs[i]).transform->transform = {
                .translation = { .x = 0, .y = 0, .z = 0},
                .rotation = {.y = sin( R.data[i] / 2 ), .w = cos( R.data[i] / 2 )}
            };
        }

        // rotations to distance travelled
        R *= info.wheelRadius;
        ESP_LOGI(LOG, "distance %f %f %f %f", R.data[0], R.data[1], R.data[2], R.data[3]); 
        dR = R - prevR;

        ESP_LOGI(LOG, "dR %f %f %f %f", dR.data[0], dR.data[1], dR.data[2], dR.data[3]); 
        ESP_LOGI(LOG, "prevR %f %f %f %f", prevR.data[0], prevR.data[1], prevR.data[2], prevR.data[3]); 
        prevR = R;

        updateTrackingMat(w + M_PI_4);
        res += tracking * drive * dR;
        ESP_LOGI(LOG, "res %f %f %f", x, y, w); 

        transform->transform = {
            .translation = { .x = y, .y = x},
            .rotation = {.z = sin( w / 2 ), .w = cos( w / 2 )}
        };

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }
}


void poseTracker::updateTrackingMat(float w)
{
    tracking(0, 0) = cos(w);
    tracking(0, 1) = -sin(w);
    tracking(1, 0) = sin(w);
    tracking(1, 1) = cos(w);
    tracking(2, 2) = 1.0f / info.wheelDistance;
};