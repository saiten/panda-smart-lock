/**
 * registration_service.cpp
 */
#include "registration_service.hpp"
#include "bluefruit.h"
#include <SHA256.h>
#include <uECC.h>

namespace ble {

const uint16_t UUID16_SVC_REGISTRATION = 0x8D01;
// 9A234600-E549-4FE1-B5F3-A300529ED143
const uint8_t UUID128_CHR_REGIST_REGPUBKEY[] = {0x43, 0xD1, 0x9E, 0x52, 0x00, 0xA3, 0xF3, 0xB5,
                                                0xE1, 0x4F, 0x49, 0xE5, 0x00, 0x46, 0x23, 0x9A};
// A7EE741F-6544-4058-93CA-8F3BD84F211C
const uint8_t UUID128_CHR_REGIST_VERIFYSIGN[] = {0x1C, 0x21, 0x4F, 0xD8, 0x3B, 0x8F, 0xCA, 0x93,
                                                 0x58, 0x40, 0x44, 0x65, 0x1F, 0x74, 0xEE, 0xA7};

registration_service::registration_service()
    : BLEService(UUID16_SVC_REGISTRATION),
      register_pub_key_char(BLECharacteristic(UUID128_CHR_REGIST_REGPUBKEY)),
      verify_sign_char(BLECharacteristic(UUID128_CHR_REGIST_VERIFYSIGN)) {
}

err_t registration_service::begin() {
    VERIFY_STATUS(BLEService::begin());

    // register public key characteristic
    register_pub_key_char.setProperties(CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
    register_pub_key_char.setPermission(SECMODE_OPEN, SECMODE_OPEN);
    register_pub_key_char.setMaxLen(64);
    register_pub_key_char.setWriteCallback(registration_service::characteristic_write_callback, true);
    register_pub_key_char.setCccdWriteCallback(registration_service::characteristic_cccd_write_callback);

    VERIFY_STATUS(register_pub_key_char.begin());

    // verify sign characteristic
    verify_sign_char.setProperties(CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
    verify_sign_char.setPermission(SECMODE_OPEN, SECMODE_OPEN);
    verify_sign_char.setMaxLen(80);
    verify_sign_char.setWriteCallback(registration_service::characteristic_write_callback, true);
    verify_sign_char.setCccdWriteCallback(registration_service::characteristic_cccd_write_callback);

    VERIFY_STATUS(verify_sign_char.begin());

    return ERROR_NONE;
}

const char *registration_service::get_name() {
    return name;
}

const uint8_t *registration_service::get_tmp_public_key() {
    return tmp_public_key;
}

const uint8_t *registration_service::get_pin_code() {
    return pin_code;
}

void registration_service::set_confirm_register_callback(registration_service::confirm_register_callback_t callback) {
    confirm_register_callback = callback;
}

void registration_service::register_public_key(uint8_t *data, uint16_t len) {
    Serial.printf("register pub key write callback\n");

    Serial.printf("offset=%d, len=%d\n", tmp_public_key_offset, len);
    if(tmp_public_key_offset + len <= PUBLIC_KEY_SIZE) {
        memcpy(tmp_public_key + tmp_public_key_offset, data, len);
    } else {
        Serial.printf("public key overflow\n");
        tmp_public_key_offset = 0;
        register_pub_key_char.notify8(2);
        return;
    }

    tmp_public_key_offset += len;

    if(tmp_public_key_offset == PUBLIC_KEY_SIZE) {
        uECC_Curve curve = uECC_secp256r1();

        uint8_t retval = uECC_valid_public_key(tmp_public_key, curve) ? 0 : 1;
        Serial.printf("valid public key = %d\n", retval);

        tmp_public_key_offset = 0;
        register_pub_key_char.notify8(retval);
    }
}

void registration_service::verify_sign(uint8_t *data, uint16_t len) {
    Serial.printf("verify sign write callback\n");
    Serial.printf("length = %d\n", len);

    if(len <= SIGNATURE_SIZE) {
        Serial.printf("invalid data\n");
        verify_sign_char.notify8(2);
        return;
    }

    int salt_size = len - SIGNATURE_SIZE;

    Serial.printf("publickey = ");
    for(int i = 0; i < PUBLIC_KEY_SIZE; i++) {
        Serial.printf("%02x", tmp_public_key[i]);
    }
    Serial.printf("\n");

    Serial.printf("salt = ");
    for(int i = 0; i < salt_size; i++) {
        Serial.printf("%02x", data[i]);
    }
    Serial.printf("\n");

    Serial.printf("signature = ");
    for(int i = salt_size; i < salt_size + 64; i++) {
        Serial.printf("%02x", data[i]);
    }
    Serial.printf("\n");

    uint8_t hash[DIGEST_SIZE];
    SHA256 sha256 = SHA256();
    sha256.update(data, salt_size);
    sha256.finalize(hash, DIGEST_SIZE);

    Serial.printf("hash = ");
    for(int i = 0; i < DIGEST_SIZE; i++) {
        Serial.printf("%02x", hash[i]);
    }
    Serial.printf("\n");

    uECC_Curve curve = uECC_secp256r1();
    int retval = uECC_verify(tmp_public_key, hash, 32, data + salt_size, curve);
    Serial.printf("verify retval = %d\n", retval);
    if(retval) {
        // valid signature
        if(salt_size == 7) {
            memcpy(name, data, NAME_LENGTH - 1);
            name[NAME_LENGTH - 1] = '\0';

            for(int i = 0; i < 3; i++) {
                uint8_t val = *(data + 4 + i);
                pin_code[i * 2] = val & 0xF0 >> 4;
                pin_code[i * 2 + 1] = val & 0xF;
            }

            verify_sign_char.notify8(0);
        } else {
            verify_sign_char.notify8(1);
        }
    } else {
        verify_sign_char.notify8(1);
    }
}

void registration_service::characteristic_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len) {
    registration_service &service = (registration_service &)chr->parentService();
    if(&service.register_pub_key_char == chr) {
        service.register_public_key(data, len);
    } else if(&service.verify_sign_char == chr) {
        service.verify_sign(data, len);
    } else {
        Serial.printf("unknown write callback\n");
    }
}

void registration_service::characteristic_cccd_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value) {
    Serial.printf("cccd write callback value = %d\n", value);
    // registration_service &service = (registration_service &)chr->parentService();
}

} // namespace ble
