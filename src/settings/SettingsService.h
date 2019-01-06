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

#ifndef AQUA_CONTROLLER_SETTINGSSERVICE_H
#define AQUA_CONTROLLER_SETTINGSSERVICE_H

#include <EEPROM.h>
#include <WString.h>

enum {
    MODE_AUTO = 1,
    MODE_ENABLE = 2,
    MODE_DISABLE = 3,
};

enum {
    ADDRESS_AERATION_MODE = 0x01,
    ADDRESS_FILTERING_MODE = 0x02,
    ADDRESS_HEATING_MODE = 0x03,
    ADDRESS_LIGHT_MODE = 0x04,
    ADDRESS_MAINTAIN_TEMPERATURE = 0x05,
    ADDRESS_START_LIGHTING = 0x06,
    ADDRESS_END_LIGHTING = 0x08,
    ADDRESS_AQUARIUM_ID = 0x09,
    ADDRESS_WIFI_SSID = 0x10,
    ADDRESS_WIFI_PASSWORD = 0x30,
    ADDRESS_API_ADDRESS = 0x80,
    ADDRESS_API_TOKEN = 0x120,
    ADDRESS_MAX = 0x200,
};

class SettingsService {
public:
    SettingsService();

    ~SettingsService();

    void setLightMode(int8_t mode);

    void setHeatingMode(int8_t mode);

    void setAerationMode(int8_t mode);

    void setFilteringMode(int8_t mode);

    void setWifiAPSSID(const char *ssid);

    void setWifiAPPassword(const char *password);

    void setMaintainTemperature(int8_t temperature);

    void setStartLighting(int16_t time);

    void setEndLighting(int16_t time);

    void setApiAddress(const char *address);

    void setApiToken(const char *token);

    void setApiAquariumId(int8_t id);

    int8_t getLightMode();

    int8_t getHeatingMode();

    int8_t getAerationMode();

    int8_t getFilteringMode();

    String getWifiAPSSID();

    String getWifiAPPassword();

    int8_t getMaintainTemperature();

    int16_t getStartLighting();

    int16_t getEndLighting();

    String getApiAddress();

    String getApiToken();

    int8_t getApiAquariumId();

protected:
    EEPROMClass *_eeprom;
};

#endif //AQUA_CONTROLLER_SETTINGSSERVICE_H
