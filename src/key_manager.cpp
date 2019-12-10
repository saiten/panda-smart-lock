/**
 * key_manager.cpp
 */
#include "key_manager.hpp"

#include "constant.h"
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

namespace LittleFS = Adafruit_LittleFS_Namespace;

err_t key_manager::load(const char *name, uint8_t *public_key) {
    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, PUB_KEY_FILE_PATH, name);

    if(InternalFS.exists(path)) {
        LittleFS::File file(path, LittleFS::FILE_O_READ, InternalFS);
        VERIFY(file);
        file.readBytes(public_key, key_size);
        file.close();
        return ERROR_NONE;
    }
    return -1;
}

err_t key_manager::save(const char *name, const uint8_t *public_key) {
    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, PUB_KEY_FILE_PATH, name);

    if(InternalFS.exists(path)) {
        InternalFS.remove(path);
    }

    LittleFS::File file(path, LittleFS::FILE_O_WRITE, InternalFS);
    VERIFY(file);
    file.write(public_key, key_size);
    file.close();
    return ERROR_NONE;
}

util::list<String> *key_manager::get_list() {
    LittleFS::File dir(PUB_KEY_DIR, LittleFS::FILE_O_READ, InternalFS);
    LittleFS::File file(InternalFS);

    auto *list = new util::list<String>();
    while((file = dir.openNextFile(LittleFS::FILE_O_READ))) {
        if(!file.isDirectory()) {
            list->add(String(file.name()));
        }
        file.close();
    }
    file.close();
    dir.close();

    return list;
}