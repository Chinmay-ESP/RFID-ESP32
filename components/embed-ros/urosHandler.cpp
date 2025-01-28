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


#include "urosHandler.hpp"

#include <freertos/task.h>
#include <esp_log.h>


#ifdef UROS_USB_SUPPORT

#include <tinyusb.h>
#include <tusb_cdc_acm.h>
#include <tusb_console.h>

#include <driver/uart.h>
#include <driver/gpio.h>


static uart_port_t uart_port = UART_NUM_0;

void tinyusb_rx_callback(int itf, cdcacm_event_t *event);
bool esp32_usb_open(struct uxrCustomTransport * transport);
bool esp32_usb_close(struct uxrCustomTransport * transport);
size_t esp32_usb_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t esp32_usb_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);


#endif

#include <rmw_microxrcedds_c/config.h>
#include <uros_network_interfaces.h>
#include <rmw_microros/rmw_microros.h>
#include <rmw_microros/custom_transport.h>


#define TAG "urosHandler"


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc);vTaskDelete(NULL);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}


QueueHandle_t urosHandler::m_urosAccess;

bool isUsbInit = false;
bool isUrosInterfaceInit = false;




urosHandler::urosHandler(std::string nodeName){

    // create mutex for acccess to micro ros methods
    if(!m_urosAccess) m_urosAccess = xSemaphoreCreateMutex();

    if(!isUrosInterfaceInit) {
#ifndef UROS_USB_SUPPORT
        // initialize wifi
        ESP_ERROR_CHECK(uros_network_interface_initialize());
#else
    // setup custom transport for usb here
    rmw_uros_set_custom_transport(
        true,
        (void *) this,
        esp32_usb_open,
        esp32_usb_close,
        esp32_usb_write,
        esp32_usb_read );
#endif

    isUrosInterfaceInit = true;
    };

    allocator = rcl_get_default_allocator();

	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
	RCCHECK(rcl_init_options_init(&init_options, allocator));

	rmw_options = rcl_init_options_get_rmw_init_options(&init_options);
	

    bool waitAutoDiscovery = true;
    rmw_ret_t ret = 0;
    while (waitAutoDiscovery)
    {   

#ifndef UROS_USB_SUPPORT
        ret = rmw_uros_discover_agent(rmw_options);
#endif

        ret = rmw_uros_ping_agent(1000, 5);
        if(ret == RMW_RET_OK) waitAutoDiscovery = false;
        else if(ret == RMW_RET_TIMEOUT) waitAutoDiscovery = true;

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    rmw_uros_sync_session(1000);

	RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

	// create node
	RCCHECK(rclc_node_init_default(&node, nodeName.c_str(), "", &support));

}

void urosHandler::addThreadExecutor(std::vector<urosElement*> elements, int CPUID){

    std::vector<urosElement*>* elems = new std::vector<urosElement*>(elements);

    elems->at(0)->node = &node;
    elems->at(0)->support = &support;
    elems->at(0)->alloc = &allocator;

    TaskHandle_t handle;
    xTaskCreatePinnedToCore(executorTask, "uros-exec", 4096, elems, 2, &handle, CPUID);
}



void urosHandler::executorTask(void *param){
    std::vector<urosElement*>* elems = (std::vector<urosElement*>*)param;

    rclc_support_t* support = elems->at(0)->support;
    rcl_allocator_t* alloc = elems->at(0)->alloc;
    rcl_node_t* node = elems->at(0)->node;

    
    // hold mutex while calling uros methods
    xSemaphoreTake(m_urosAccess, portMAX_DELAY);
    rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
	RCCHECK(rclc_executor_init(&executor, &support->context, 6, alloc));

    for(auto* elem : *elems){
        elem->support = support;
        elem->exec = &executor;
        elem->alloc = alloc;
        elem->node = node;

        elem->init();
    } 

    xSemaphoreGive(m_urosAccess);
    
    rclc_executor_spin(&executor);
    // rclc_executor_spin_period(&executor, 1000);
    vTaskDelete(NULL);
}



#ifdef UROS_USB_SUPPORT

// bool esp32_usb_open(uxrCustomTransport *transport){ 
//     if(isUsbInit) return isUsbInit;
//     ESP_LOGI(TAG, "USB initialization successful");

//     const tinyusb_config_t tusb_cfg = {
//         .device_descriptor = NULL,
//         .string_descriptor = NULL,
//         .external_phy = false,
// #if (TUD_OPT_HIGH_SPEED)
//         .fs_configuration_descriptor = NULL,
//         .hs_configuration_descriptor = NULL,
//         .qualifier_descriptor = NULL,
// #endif
//     };

//     ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

//     tinyusb_config_cdcacm_t uros_acm = {
//         .usb_dev = TINYUSB_USBDEV_0,
//         .cdc_port = TINYUSB_CDC_ACM_1,
//         .callback_rx = tinyusb_rx_callback
//     };

//     tinyusb_config_cdcacm_t logger_acm = {
//         .usb_dev = TINYUSB_USBDEV_0,
//         .cdc_port = TINYUSB_CDC_ACM_0,
//     }; 

//     ESP_ERROR_CHECK(tusb_cdc_acm_init(&uros_acm));
//     ESP_ERROR_CHECK(tusb_cdc_acm_init(&logger_acm));

//     ESP_LOGI(TAG, "USB initialization DONE");
//     esp_tusb_init_console(TINYUSB_CDC_ACM_0); // log to usb
//     return isUsbInit = true;
// };



// size_t esp32_usb_write(uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *err)
// {
//     tinyusb_cdcacm_write_queue((tinyusb_cdcacm_itf_t) TINYUSB_CDC_ACM_1, buf, len);
//     tinyusb_cdcacm_write_flush((tinyusb_cdcacm_itf_t) TINYUSB_CDC_ACM_1, 0);
//     return len;
// }


// void tinyusb_rx_callback(int itf, cdcacm_event_t *event){}


// size_t esp32_usb_read(uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *err){
//     size_t rx_size = 0;
//     esp_err_t ret = tinyusb_cdcacm_read(TINYUSB_CDC_ACM_1, buf, len, &rx_size);
//     return rx_size;
// }

// bool esp32_usb_close(struct uxrCustomTransport * transport){
//     return true;
// }





#define UART_TXD  (GPIO_NUM_43)
#define UART_RXD  (GPIO_NUM_44)


// #define UART_TXD  (CONFIG_MICROROS_UART_TXD)
// #define UART_RXD  (CONFIG_MICROROS_UART_RXD)
#define UART_RTS  (-1)
#define UART_CTS  (-1)

// --- micro-ROS Transports ---
#define UART_BUFFER_SIZE (512)

bool esp32_usb_open(struct uxrCustomTransport * transport){
    // uart_port_t uart_port = *(uart_port_t*) transport->args;

    uart_config_t uart_config = {
        .baud_rate = 460800,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    if (uart_param_config(uart_port, &uart_config) == ESP_FAIL) {
        return false;
    }
    if (uart_set_pin(uart_port, UART_TXD, UART_RXD, UART_RTS, UART_CTS) == ESP_FAIL) {
        return false;
    }
    if (uart_driver_install(uart_port, UART_BUFFER_SIZE * 2, 0, 0, NULL, 0) == ESP_FAIL) {
        return false;
    }

    return true;
}

bool esp32_usb_close(struct uxrCustomTransport * transport){
    // uart_port_t uart_port = *(uart_port_t*) transport->args;

    return uart_driver_delete(uart_port) == ESP_OK;
}

size_t esp32_usb_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err){
    // uart_port_t uart_port = *(uart_port_t*) transport->args;
    const int txBytes = uart_write_bytes(uart_port, (const char*) buf, len);
    return txBytes;
}

size_t esp32_usb_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err){
    // uart_port_t uart_port = *(uart_port_t*) transport->args;
    const int rxBytes = uart_read_bytes(uart_port, buf, len, timeout / portTICK_PERIOD_MS);
    return rxBytes;
}

#endif
