idf_component_register(SRCS "rfid_pub.cpp" "main.cpp" "rc522.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp-idf-rc522 driver embed-ros micro_ros_espidf_component)