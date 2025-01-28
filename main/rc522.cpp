#include "rc522.hpp"
#include "esp_mac.h"

#define TAG "RFID"

spi_bus_config_t reader_config = {
        .mosi_io_num = RC522_SPI_BUS_GPIO_MOSI,
        .miso_io_num = RC522_SPI_BUS_GPIO_MISO,
        .sclk_io_num = RC522_SPI_BUS_GPIO_SCLK,
};

rc522_spi_config_t driver_config = {
    .host_id = SPI3_HOST,
    .bus_config = &reader_config,
    .dev_config = {
        .spics_io_num = RC522_SPI_SCANNER_GPIO_SDA,
    },
    .rst_io_num = RC522_SCANNER_GPIO_RST,
};

// Driver handle
rc522_driver_handle_t driver;


extern void dump_block(uint8_t buffer[RC522_MIFARE_BLOCK_SIZE])
{
    for (uint8_t i = 0; i < RC522_MIFARE_BLOCK_SIZE; i++) {
        //esp_log_write(ESP_LOG_INFO, "%02" RC522_X " ", buffer[i]);
    }
    //esp_log_write(ESP_LOG_INFO,  "\n");
}


extern "C" void on_picc_state_changed(void *arg, esp_event_base_t base, int32_t event_id, void *data)
{
    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *)data;
    rc522_picc_t *picc = event->picc;

    if (picc->state != RC522_PICC_STATE_ACTIVE) {
        return;
    }
    rc522_picc_print(picc);
    char uid_str[RC522_PICC_UID_STR_BUFFER_SIZE_MAX];
    rc522_picc_uid_to_str(&picc->uid, uid_str, sizeof(uid_str));
    

    if (!rc522_mifare_type_is_classic_compatible(picc->type)) {
        ESP_LOGW( "RFID", "Card is not supported by this example");
        return;
    }

}


