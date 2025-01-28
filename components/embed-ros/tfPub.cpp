


#include <tfPub.hpp>
#include <rosidl_runtime_c/string_functions.h>
#include <esp_log.h>


const rosidl_message_type_support_t * tfMsgType = ROSIDL_GET_MSG_TYPE_SUPPORT(tf2_msgs, msg, TFMessage);

TfPub* current = 0;

void tf_timer_callback(rcl_timer_t * tmr, int64_t);

TfPub::TfPub() {
    current = this;

    // init msg data
    tf2_msgs__msg__TFMessage__init(&m_msg);
    m_msg.transforms.capacity = MAX_TANSFORMS;
    m_msg.transforms.size = 0;
    m_msg.transforms.data = m_transform_slots;

}

void TfPub::init()
{
    rclc_publisher_init_default(&m_rcl_pub, node, tfMsgType, "/tf");

    rclc_timer_init_default(&m_tf_timer, support, RCL_MS_TO_NS(TF_PUBLISH_DELAY_MS), TfPub::tf_timer_callback);
    rclc_executor_add_timer(exec, &m_tf_timer);  
}

void TfPub::addSource(const char *parent, const char *child, TfSource & source){
    if(sourceCount == MAX_TANSFORMS) return;

    geometry_msgs__msg__TransformStamped* slot = &m_transform_slots[sourceCount++];

    rosidl_runtime_c__String__init(&slot->child_frame_id);
    rosidl_runtime_c__String__init(&slot->header.frame_id);
    rosidl_runtime_c__String__assign(&slot->child_frame_id, child);
    rosidl_runtime_c__String__assign(&slot->header.frame_id, parent);

    source = slot;
    m_msg.transforms.size++;
}



void TfPub::tf_timer_callback(rcl_timer_t * tmr, int64_t){
    rcl_publish(&current->m_rcl_pub, &current->m_msg, NULL);
};
