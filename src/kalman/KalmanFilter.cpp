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

#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float varVolt, float varProcess) {
    this->_varVolt = varVolt;
    this->_varProcess = varProcess;
}

float KalmanFilter::filter(float value) {
    this->_Pc = this->_Pn + this->_varProcess;
    this->_G = this->_Pc / (this->_Pc + this->_varVolt);
    this->_Pn = (1 - this->_G) * this->_Pc;
    this->_Xp = this->_Xe;
    this->_Zp = this->_Xp;
    this->_Xe = this->_G * (value - this->_Zp) + this->_Xp;

    return this->_Xe;
}
