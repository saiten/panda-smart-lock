/**
 * constant.h
 */
#pragma once

// OLED FeatherWing buttons map to different pins depending on board:
#define BUTTON_A 31
#define BUTTON_B 30
#define BUTTON_C 27

#define TASK_ID_SPLASH 0x01
#define TASK_ID_MENU 0x02
#define TASK_ID_ADV 0x03
#define TASK_ID_REGISTER 0x04

#define MAX_PATH_LEN 32
#define BASE_DIR "/panda/"
#define PUB_KEY_DIR BASE_DIR "/keys"

#define REGISTER_MODE_FILE_PATH BASE_DIR "/register"
#define PUB_KEY_FILE_PATH PUB_KEY_DIR "/%s"

#define PUBLIC_KEY_SIZE 64
#define SIGNATURE_SIZE 64
#define DIGEST_SIZE 32

#define PIN_CODE_LENGTH 6
#define NAME_LENGTH (4 + 1)
