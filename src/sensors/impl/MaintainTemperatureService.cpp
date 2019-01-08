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

#include "../../defines.h"
#include "../../AppService.h"
#include "MaintainTemperatureService.h"

MaintainTemperatureService::MaintainTemperatureService() : Sensor(PIN_MAINTAIN_TEMPERATURE) {
    this->_filter = new KalmanFilter(0.5, 0.05);

    this->_bus = new OneWire(this->getPin());
    this->_sensors = new DallasTemperature(this->_bus);

    this->_sensors->begin();

    this->_devicesCount = this->_sensors->getDeviceCount();

    if (this->_devicesCount > 0) {
        App->getSerial()->print("Found ");
        App->getSerial()->print(this->_devicesCount);
        App->getSerial()->println(" sensors.");
    } else {
        App->getSerial()->println("Not found D18B20 sensors.");
    }
}

void MaintainTemperatureService::internalUpdate() {
    if (this->_devicesCount) {
        this->_sensors->requestTemperatures();

        float val = 0;

        for (int i = 0; i < this->_devicesCount; ++i) {
            val += this->_sensors->getTempCByIndex(i);
        }

        val /= this->_devicesCount;

        this->_value = this->_filter->filter(val);
    } else {
        this->_value = NAN;
    }
}

