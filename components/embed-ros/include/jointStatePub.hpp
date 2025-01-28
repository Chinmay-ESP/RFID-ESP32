
#ifndef JOINT_STATE_PUB_HPP
#define JOINT_STATE_PUB_HPP


#include <rclc/rclc.h>
#include <urosElement.hpp>
#include <sensor_msgs/msg/joint_state.h>


class jointStatePub : public urosElement
{

public:


    jointStatePub();

    void init();

    void addJoint(joint&);



private:

    rcl_publisher_t m_rcl_pub;
    rcl_timer_t m_tf_timer;

    sensor_msgs__msg__JointState m_msg;


    static void tf_timer_callback(rcl_timer_t * tmr, int64_t);

};
#endif //  JOINT_STATE_PUB_HPP