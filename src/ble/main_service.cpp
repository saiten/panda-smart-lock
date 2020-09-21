/**
 * main_service.cpp
 */
#include "main_service.hpp"
#include "Constant.h"
#include "bluefruit.h"
#include "key_manager.hpp"
#include <SHA256.h>
#include <uECC.h>

namespace ble {

const uint16_t UUID16_SVC_MAIN = 0x8D02;
// AFB54496-31DB-461B-8313-90A196CEA4D6
const uint8_t UUID128_CHR_MAIN_CHALLENGE[] = {0xD6, 0xA4, 0xCE, 0x96, 0xA1, 0x90, 0x13, 0x83,
                                              0x1B, 0x46, 0xDB, 0x31, 0x96, 0x44, 0xB5, 0xAF};
// E570EB24-CB0C-4A1D-851D-F4A524DDE786
const uint8_t UUID128_CHR_MAIN_OPERATION[] = {0x86, 0xE7, 0xDD, 0x24, 0xA5, 0xF4, 0x1D, 0x85,
                                              0x1D, 0x4A, 0x0C, 0xCB, 0x24, 0xEB, 0x70, 0xE5};

main_service::main_service()
    : BLEService(UUID16_SVC_MAIN),
      challenge_char(BLECharacteristic(UUID128_CHR_MAIN_CHALLENGE)),
      operation_char(BLECharacteristic(UUID128_CHR_MAIN_OPERATION)) {
}

err_t main_service::begin() {
    VERIFY_STATUS(BLEService::begin());

    // challenge characteristic
    challenge_char.setProperties(CHR_PROPS_READ);
    challenge_char.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    challenge_char.setFixedLen(CHALLENGE_SIZE);
    VERIFY_STATUS(challenge_char.begin());

    // write operation characteristic
    operation_char.setProperties(CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
    operation_char.setPermission(SECMODE_OPEN, SECMODE_OPEN);
    operation_char.setMaxLen(80);
    operation_char.setWriteCallback(main_service::operation_write_callback);
    operation_char.setCccdWriteCallback(main_service::cccd_write_callback);
    VERIFY_STATUS(operation_char.begin());

    return ERROR_NONE;
}

void main_service::update_challenge() {
    for(int i = 0; i < CHALLENGE_SIZE; i++) {
        int n = random(UINT8_MAX);
        challenge[i] = n;
    }
    Serial.printf("update challenge = 0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                  challenge[0], challenge[1], challenge[2], challenge[3], challenge[4], challenge[5], challenge[6], challenge[7]);

    //challenge_char.setFixedLen(CHALLENGE_SIZE);
    int16_t len = challenge_char.write(challenge, CHALLENGE_SIZE);
    Serial.printf("write %d bytes.\n", len);
}

void main_service::set_operation_callback(main_service::operation_callback_t callback) {
    operation_callback = callback;
}

void main_service::verify(const uint8_t *data, uint16_t len) {
    Serial.printf("verify operation length = %d\n", len);
    if(len <= SIGNATURE_SIZE) {
        Serial.printf("invalid data\n");
        operation_char.notify8(1);
        return;
    }
    int payload_size = len - SIGNATURE_SIZE;
    if(payload_size != NAME_LENGTH + 2 + CHALLENGE_SIZE) {
        Serial.printf("invalid data\n");
        operation_char.notify8(1);
        return;
    }

    char name[NAME_LENGTH + 1];
    memcpy(name, data, NAME_LENGTH);
    name[NAME_LENGTH] = '\0';

    uint8_t operation = *(data + NAME_LENGTH);
    int8_t rssi = *(data + NAME_LENGTH + 1);

    uint8_t receive_challenge[CHALLENGE_SIZE];
    memcpy(receive_challenge, data + NAME_LENGTH + 2, CHALLENGE_SIZE);

    Serial.printf("name = %s, rssi = %d, challenge = 0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                  name, rssi,
                  receive_challenge[0], receive_challenge[1], receive_challenge[2], receive_challenge[3],
                  receive_challenge[4], receive_challenge[5], receive_challenge[6], receive_challenge[7]);

    if(memcmp(challenge, receive_challenge, CHALLENGE_SIZE) != 0) {
        Serial.printf("invalid challeng\n");
        operation_char.notify8(1);
        return;
    }

    uint8_t public_key[PUBLIC_KEY_SIZE];
    auto manager = key_manager();
    if(manager.load(name, public_key) != ERROR_NONE) {
        Serial.printf("key not found\n");
        operation_char.notify8(2);
        return;
    }

    uint8_t hash[DIGEST_SIZE];
    SHA256 sha256 = SHA256();
    sha256.update(data, payload_size);
    sha256.finalize(hash, DIGEST_SIZE);

    uECC_Curve curve = uECC_secp256r1();
    int retval = uECC_verify(public_key, hash, 32, data + payload_size, curve);
    Serial.printf("verify retval = %d\n", retval);
    if(retval) {
        bool c = true;
        for(int i = 0; i < CHALLENGE_SIZE; i++) {
            c = c && (challenge[i] == receive_challenge[i]);
        }
        if(!c) {
            Serial.printf("invalid challenge response\n");
            operation_char.notify8(3);
            return;
        }

        Serial.printf("Operation receive success. operation = %d\n", operation);
        if(operation_callback) {
            operation_callback(operation);
        }
        operation_char.notify8(0);
    } else {
        Serial.printf("invalid signature\n");
        operation_char.notify8(1);
    }
}

void main_service::operation_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len) {
    main_service &service = (main_service &)chr->parentService();
    service.verify(data, len);
}

void main_service::cccd_write_callback(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value) {
    Serial.printf("cccd write callback value = %d\n", value);
}

} // namespace ble
