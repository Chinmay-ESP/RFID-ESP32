#include "rfid_pub.hpp"

const char * pub_topic_name = "/read_rfid";
char* ptr2;

#define TAG "RFID"

simPub* simPub::current;
const char* simPub::pub;


simPub::simPub() { 
    rc522_spi_create(&driver_config, &driver);
    rc522_driver_install(driver);
    rc522_config_t scanner_config = {
        .driver = driver,
    };


    rc522_create(&scanner_config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_PICC_STATE_CHANGED, simPub::on_read_event, NULL);
    rc522_start(scanner);

    current = this;
}

// message type support
const rosidl_message_type_support_t * pub_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32);


// Initializes the ROS 2 publisher and message
void simPub::init() {
    //ESP_LOGI(TAG, "Initializing line following publisher...");


    rclc_publisher_init_default(&simple_publisher, node, pub_type_support, pub_topic_name);
    std_msgs__msg__Int32__init(&msg);


    // rclc_timer_init_default(&timer, support, RCL_MS_TO_NS(1000 / frequency), simPub::timer_callback);
    // rclc_timer_init_default(&timer, pub_type_support, 100)
    // rclc_executor_add_timer(exec, &timer);
    //ESP_LOGI(TAG, "publisher initialization successful");
}


// Callback for handling RFID read events
void simPub::on_read_event(void *arg, esp_event_base_t base, int32_t event_id, void *data)
{
    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *)data;
    rc522_picc_t *picc = event->picc;

    if (picc->state != RC522_PICC_STATE_ACTIVE) {
        return;
    }
    

    rc522_picc_print(picc);
    char uid_str[RC522_PICC_UID_STR_BUFFER_SIZE_MAX];
    rc522_picc_uid_to_str(&picc->uid, uid_str, sizeof(uid_str));

    // Log the UID
    ESP_LOGI(TAG, "UID:  %s", uid_str);

    
    // Copy the UID to the message data and publish it
    memcpy(&current->msg.data, &picc->uid, 4);
    rcl_publish(&current->simple_publisher, &current->msg, NULL);
    
    
    // Log a warning if the card type is not supported
    if (!rc522_mifare_type_is_classic_compatible(picc->type)) {
        ESP_LOGW(TAG, "Card is not supported by this example");
        return;
    }

}
