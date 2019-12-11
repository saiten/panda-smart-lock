/**
 * registration_service.hpp
 */
#pragma once

#include "constant.h"
#include <bluefruit.h>

namespace ble {

class registration_service : public BLEService {
  public:
    typedef void (*confirm_register_callback_t)(registration_service *, const char *, const uint8_t *);

    registration_service();
    err_t begin(void);

    void set_confirm_register_callback(confirm_register_callback_t callback);

    void save();
    void abort();

    const char *get_name();
    const uint8_t *get_tmp_public_key();
    const uint8_t *get_pin_code();

  private:
    BLECharacteristic register_pub_key_char;
    BLECharacteristic verify_sign_char;
    uint8_t tmp_public_key[PUBLIC_KEY_SIZE];
    uint8_t tmp_public_key_offset = 0;
    uint8_t pin_code[PIN_CODE_LENGTH];
    char name[NAME_LENGTH + 1];

    confirm_register_callback_t confirm_register_callback;

    void register_public_key(uint8_t *data, uint16_t len);
    void verify_sign(uint8_t *data, uint16_t len);

    static void characteristic_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
    static void characteristic_cccd_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value);
};
} // namespace ble
