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
#include "HeatingService.h"

HeatingService::HeatingService() : RelayService(PIN_RELAY_HEATING) {
    double Kp = 2, Ki = 0.2, Kd = 0.5;
    this->_pid = new PID(&this->_input, &this->_output, &this->_setpoint, Kp, Ki, Kd, DIRECT);
    this->_pid->SetOutputLimits(0, 100);
    this->_pid->SetMode(AUTOMATIC);
}

void HeatingService::update() {
    this->_pid->Compute();

    if ((millis() - this->_lastUpdateTime >= HEATING_UPDATE_INTERVAL) || this->_lastUpdateTime == 0) {
        this->internalUpdate();
        this->_lastUpdateTime = millis();
    }
}

void HeatingService::internalUpdate() {
    int32_t mode = App->getSettingsService()->getHeatingMode();

    if (mode == MODE_DISABLE) {
        App->getSerial()->println("Heating: Manual disabled");
        this->disable();
    } else if (mode == MODE_ENABLE) {
        App->getSerial()->println("Heating: Manual enabled");
        this->enable();
    } else if (mode == MODE_AUTO) {
        this->_setpoint = App->getSettingsService()->getMaintainTemperature();
        this->_input = App->getMaintainTemperatureService()->getValue();

        if (this->_output > 50) {
            this->enable();
            App->getSerial()->println("Heating: Auto enabled");
        } else {
            this->disable();
            App->getSerial()->println("Heating: Auto disabled");
        }
    } else {
        App->getSerial()->print("Heating: Unknown mode: ");
        App->getSerial()->println(mode);
    }
}
