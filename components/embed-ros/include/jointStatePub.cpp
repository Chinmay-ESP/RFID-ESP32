
#include "jointStatePub.hpp"
#include <rosidl_runtime_c/string_functions.h>

#include <esp_log.h>



const rosidl_message_type_support_t * jointStateSupportType = ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, SointState);

jointStatePub::jointStatePub(){
    m_msg.name = 

}


void jointStatePub::init(){
    rclc_publisher_init_default(&m_rcl_pub, node, jointStateSupportType, "/joint_states");

    rclc_timer_init_default(&m_tf_timer, support, RCL_MS_TO_NS(TF_PUBLISH_DELAY_MS), TfPub::tf_timer_callback);
    rclc_executor_add_timer(exec, &m_tf_timer);
}


void jointStatePub::addJoint(joint &jnt){


}
