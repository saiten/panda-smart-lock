/**
 * main_service.hpp
 */
#pragma once

#include <bluefruit.h>

namespace ble {

#define CHALLENGE_SIZE 8

class main_service : public BLEService {
  public:
    typedef void (*operation_callback_t)(uint8_t operation);

    main_service();
    err_t begin(void);

    void update_challenge();
    void set_operation_callback(operation_callback_t callback);

  private:
    BLECharacteristic challenge_char;
    BLECharacteristic operation_char;
    uint8_t challenge[CHALLENGE_SIZE];

    operation_callback_t operation_callback;

    void verify(const uint8_t *data, uint16_t len);

    static void operation_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
    static void cccd_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value);
};
} // namespace ble
