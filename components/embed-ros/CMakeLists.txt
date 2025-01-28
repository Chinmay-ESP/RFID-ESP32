

list(APPEND EMBED_ROS_SRCS
    "urosHandler.cpp"
    "tfPub.cpp"
    "TfSource.cpp"
    "poseTracker.cpp"
)


set(EMBED_ROS_USB_SUPPORT TRUE)


if(EMBED_ROS_USB_SUPPORT)

message(STATUS "building EMBED_ROS with usb support")

idf_component_register(SRCS ${EMBED_ROS_SRCS}
        INCLUDE_DIRS "include"
        REQUIRES qmd micro_ros_espidf_component espressif__esp-dsp espressif__esp_tinyusb)

else()

message(STATUS "building EMBED_ROS without usb support")


idf_component_register(SRCS ${EMBED_ROS_SRCS}
                    INCLUDE_DIRS "include"
                    REQUIRES qmd micro_ros_espidf_component espressif__esp-dsp)
endif()

