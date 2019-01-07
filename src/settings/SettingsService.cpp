/**
 * MIT License
 *
 * Copyright (c) 2018-2019 Kravchenko Artyom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../AppService.h"
#include "SettingsService.h"

SettingsService::SettingsService() {
    this->_eeprom = new EEPROMClass();
    this->_eeprom->begin(ADDRESS_MAX);
}

SettingsService::~SettingsService() {
}

void SettingsService::setAerationMode(int8_t mode) {
    this->_eeprom->write(ADDRESS_AERATION_MODE, mode);
    this->_eeprom->commit();
}

void SettingsService::setFilteringMode(int8_t mode) {
    this->_eeprom->write(ADDRESS_FILTERING_MODE, mode);
    this->_eeprom->commit();
}

void SettingsService::setHeatingMode(int8_t mode) {
    this->_eeprom->write(ADDRESS_HEATING_MODE, mode);
    this->_eeprom->commit();
}

void SettingsService::setLightMode(int8_t mode) {
    this->_eeprom->write(ADDRESS_LIGHT_MODE, mode);
    this->_eeprom->commit();
}

void SettingsService::setWifiAPSSID(const char *ssid) {
    int8_t i = 0;

    while (true) {
        this->_eeprom->write(ADDRESS_WIFI_SSID + i, *ssid);

        if (*ssid == '\0') {
            break;
        }

        ++ssid;
        ++i;
    }

    this->_eeprom->commit();
}

void SettingsService::setWifiAPPassword(const char *password) {
    int8_t i = 0;

    while (true) {
        this->_eeprom->write(ADDRESS_WIFI_PASSWORD + i, *password);

        if (*password == '\0') {
            break;
        }

        ++password;
        ++i;
    }

    this->_eeprom->commit();
}

void SettingsService::setMaintainTemperature(int8_t temperature) {
    this->_eeprom->write(ADDRESS_MAINTAIN_TEMPERATURE, temperature);
    this->_eeprom->commit();
}

void SettingsService::setStartLighting(int16_t time) {
    this->_eeprom->put(ADDRESS_START_LIGHTING, time);
    this->_eeprom->commit();
}

void SettingsService::setEndLighting(int16_t time) {
    this->_eeprom->put(ADDRESS_END_LIGHTING, time);
    this->_eeprom->commit();
}

void SettingsService::setApiAddress(const char *address) {
    int8_t i = 0;

    while (true) {
        this->_eeprom->write(ADDRESS_API_ADDRESS + i, *address);

        if (*address == '\0') {
            break;
        }

        ++address;
        ++i;
    }

    this->_eeprom->commit();
}

void SettingsService::setApiToken(const char *token) {
    int8_t i = 0;

    while (true) {
        this->_eeprom->write(ADDRESS_API_TOKEN + i, *token);

        if (*token == '\0') {
            break;
        }

        ++token;
        ++i;
    }

    this->_eeprom->commit();
}

void SettingsService::setApiAquariumId(int8_t id) {
    this->_eeprom->write(ADDRESS_AQUARIUM_ID, id);
    this->_eeprom->commit();
}

int8_t SettingsService::getAerationMode() {
    int8_t mode = this->_eeprom->read(ADDRESS_AERATION_MODE);

    switch (mode) {
        case MODE_AUTO:
        case MODE_ENABLE:
        case MODE_DISABLE:
            return mode;
        default:
            this->setAerationMode(MODE_AUTO);
            return MODE_AUTO;
    }
}

int8_t SettingsService::getLightMode() {
    int8_t mode = this->_eeprom->read(ADDRESS_LIGHT_MODE);

    switch (mode) {
        case MODE_AUTO:
        case MODE_ENABLE:
        case MODE_DISABLE:
            return mode;
        default:
            this->setLightMode(MODE_AUTO);
            return MODE_AUTO;
    }
}

int8_t SettingsService::getHeatingMode() {
    int8_t mode = this->_eeprom->read(ADDRESS_HEATING_MODE);

    switch (mode) {
        case MODE_AUTO:
        case MODE_ENABLE:
        case MODE_DISABLE:
            return mode;
        default:
            this->setHeatingMode(MODE_AUTO);
            return MODE_AUTO;
    }
}

int8_t SettingsService::getFilteringMode() {
    int8_t mode = this->_eeprom->read(ADDRESS_FILTERING_MODE);

    switch (mode) {
        case MODE_AUTO:
        case MODE_ENABLE:
        case MODE_DISABLE:
            return mode;
        default:
            this->setFilteringMode(MODE_AUTO);
            return MODE_AUTO;
    }
}

String SettingsService::getWifiAPSSID() {
    String string = "";

    for (int32_t i = ADDRESS_WIFI_SSID; i <= ADDRESS_WIFI_PASSWORD; i++) {
        char byte = this->_eeprom->read(i);

        if (byte != '\0') {
            string += byte;
        } else {
            break;
        }
    }

    return string;
}

String SettingsService::getWifiAPPassword() {
    String string = "";

    for (int32_t i = ADDRESS_WIFI_PASSWORD; i <= ADDRESS_API_ADDRESS; i++) {
        char byte = this->_eeprom->read(i);

        if (byte != '\0') {
            string += byte;
        } else {
            break;
        }
    }

    return string;
}

int8_t SettingsService::getMaintainTemperature() {
    return this->_eeprom->read(ADDRESS_MAINTAIN_TEMPERATURE) ?: 25;
}

int16_t SettingsService::getStartLighting() {
    int16_t time;
    time = this->_eeprom->get(ADDRESS_START_LIGHTING, time);

    return time ?: 900;
}

int16_t SettingsService::getEndLighting() {
    int16_t time;
    time = this->_eeprom->get(ADDRESS_END_LIGHTING, time);

    return time ?: 2100;
}

String SettingsService::getApiAddress() {
    String string = "";

    for (int32_t i = ADDRESS_API_ADDRESS; i <= ADDRESS_API_TOKEN; i++) {
        char byte = this->_eeprom->read(i);

        if (byte != '\0') {
            string += byte;
        } else {
            break;
        }
    }

    return string;
}

String SettingsService::getApiToken() {
    String string = "";

    for (int32_t i = ADDRESS_API_TOKEN; i <= ADDRESS_MAX; i++) {
        char byte = this->_eeprom->read(i);

        if (byte != '\0') {
            string += byte;
        } else {
            break;
        }
    }

    return string;
}

int8_t SettingsService::getApiAquariumId() {
    return this->_eeprom->read(ADDRESS_AQUARIUM_ID);
}
