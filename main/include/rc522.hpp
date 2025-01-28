#include <esp_log.h>
#include <esp_check.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "rc522.h"
#include "driver/rc522_spi.h"
#include "picc/rc522_mifare.h"
#include "rc522_picc.h"
#include "stdio.h"

#define RC522_SPI_BUS_GPIO_MISO    (38)
#define RC522_SPI_BUS_GPIO_MOSI    (37)
#define RC522_SPI_BUS_GPIO_SCLK    (36)
#define RC522_SPI_SCANNER_GPIO_SDA (35)
#define RC522_SCANNER_GPIO_RST     (GPIO_NUM_NC) // soft-reset


extern rc522_spi_config_t driver_config;

extern rc522_driver_handle_t driver;
extern rc522_handle_t scanner;

static void dump_block(uint8_t buffer[RC522_MIFARE_BLOCK_SIZE]);

extern "C" void on_picc_state_changed(void *arg, esp_event_base_t base, int32_t event_id, void *data);
