#ifndef TF_PUB_HPP
#define TF_PUB_HPP


#include <rclc/rclc.h>
#include <urosElement.hpp>
#include <tf2_msgs/msg/tf_message.h>
// #include <quadrature.hpp>
// #include <esp_dsp.h>

#include <TfSource.hpp>



#define MAX_TANSFORMS (7)
#define TF_PUBLISH_DELAY_MS (100)

class TfPub : public urosElement {

public:

    TfPub();

    void init();

    void addSource(const char* parent, const char* child, TfSource&);


private:

    int sourceCount = 0;
    rcl_timer_t m_tf_timer;
    rcl_publisher_t m_rcl_pub;

    tf2_msgs__msg__TFMessage m_msg;
    geometry_msgs__msg__TransformStamped m_transform_slots[MAX_TANSFORMS];
    geometry_msgs__msg__TransformStamped* transform = new geometry_msgs__msg__TransformStamped();


    static void tf_timer_callback(rcl_timer_t * tmr, int64_t);
};



#endif //  TF_PUB_HPP 