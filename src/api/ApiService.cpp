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

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "../AppService.h"
#include "ApiService.h"

ApiService::ApiService() {}

void ApiService::update() {
    if (this->_lastUpdateTime == 0 || millis() - this->_lastUpdateTime > 300000) {
        this->internalUpdate();
        this->_lastUpdateTime = millis();
    }
}

void ApiService::internalUpdate() {
    this->sendStats();
    this->updateSettings();
}

void ApiService::updateSettings() {
    App->getSerial()->println("Call update settings");

    WiFiClient wifiClient;
    HTTPClient client;

    char url[1000] = "";
    sprintf(url, "%s/aquariums/%d/settings",
            App->getSettingsService()->getApiAddress().c_str(),
            App->getSettingsService()->getApiAquariumId()
    );

    App->getSerial()->print("Url: ");
    App->getSerial()->println((String) url);

    client.begin(wifiClient, (String)url);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + App->getSettingsService()->getApiToken());

    int httpResponseCode = client.GET();

    App->getSerial()->print("httpResponseCode: ");
    App->getSerial()->println(httpResponseCode);

    if (httpResponseCode == 200) {
        String response = client.getString();

        DynamicJsonDocument json_document(500);
        DeserializationError err = deserializeJson(json_document, response);
        json_document.shrinkToFit();
        JsonObject root = json_document.as<JsonObject>();

        App->getSettingsService()->setLightMode(root["lighting"]);
        App->getSettingsService()->setHeatingMode(root["heating"]);
        App->getSettingsService()->setAerationMode(root["aeration"]);
        App->getSettingsService()->setFilteringMode(root["filtering"]);
        App->getSettingsService()->setStartLighting(root["startLighting"]);
        App->getSettingsService()->setEndLighting(root["endLighting"]);
        App->getSettingsService()->setMaintainTemperature(root["maintainTemperature"]);
    }

    client.end();
}

void ApiService::sendStats() {
    App->getSerial()->println("Call send stats");

    WiFiClient wifiClient;
    HTTPClient client;

    char url[1000] = "";
    sprintf(url, "%s/aquariums/%d/stats", App->getSettingsService()->getApiAddress().c_str(),
            App->getSettingsService()->getApiAquariumId());

    App->getSerial()->print("Url: ");
    App->getSerial()->println((String) url);

    char data[1000];
    DynamicJsonDocument json_document(500);
    JsonObject obj = json_document.to<JsonObject>();

    obj["heating"] = (int) App->getHeatingService()->isEnabled();
    obj["aeration"] = (int) App->getAerationService()->isEnabled();
    obj["lighting"] = (int) App->getLightService()->isEnabled();
    obj["filtering"] = (int) App->getFilterService()->isEnabled();

    if (!isnan(App->getMaintainTemperatureService()->getValue())) {
        obj["maintainTemperature"] = App->getMaintainTemperatureService()->getValue();
    } else {
        obj["maintainTemperature"] = nullptr;
    }

    if (!isnan(App->getOuterTemperatureService()->getValue())) {
        obj["outerTemperature"] = App->getOuterTemperatureService()->getValue();
    } else {
        obj["outerTemperature"] = nullptr;
    }

    json_document.shrinkToFit();
    serializeJson(json_document, data);

    client.begin(wifiClient, (String) url);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + App->getSettingsService()->getApiToken());

    int httpResponseCode = client.POST((String) data);

    App->getSerial()->print("httpResponseCode: ");
    App->getSerial()->println(httpResponseCode);

    if (httpResponseCode > 0 && httpResponseCode != 200) {
        String response = client.getString();

        App->getSerial()->print("Response: ");
        App->getSerial()->println(response);
    }

    client.end();
}
