/******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2025 Sam McDonald
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * provided to do so, subject to the following conditions:
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
 * 
 * -----------------------------------------------------------------------------
 * 
 * Author: Sam McDonald
 * Date: 2025-02-22
 * 
 * 
 *****************************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 

#include "sm_log.h"
 
namespace SamMcDonald {

    LogLevel Log::current_level = LogLevel::LINFO;

    void Log::set_log_level(LogLevel level) {
        current_level = level;
    }

    LogLevel Log::get_log_level() {
        return current_level;
    }

    void Log::log(LogLevel level, const std::string& message) {
        if (level <= current_level) {
            std::cout << "[ ";
            switch (level) {
                case LogLevel::LCRITICAL: std::cout << "\033[41m\033[97m" << std::setw(8) << std::left << "CRITICAL\033[0m"; break; 
                case LogLevel::LERROR: std::cout << "\033[91m" << std::setw(8) << std::left << "ERROR"; break;
                case LogLevel::LWARNING: std::cout << "\033[97m" << std::setw(8) << std::left << "WARNING"; break; 
                case LogLevel::LINFO: std::cout << "\033[34m" << std::setw(8) << std::left << "INFO"; break; 
                default: std::cout << "\033[90m" << std::setw(8) << std::left << "DEBUG"; break;
            }
            std::cout << "\033[0m";
            std::cout << " ] ";
            std::cout << std::setw(20) << std::left << message << std::endl;
        }

        std::cout << "\033[0m" << std::endl;
    }

    void Log::err(LogLevel level, const std::string& message) {
#ifdef DEBUG
        log(level, message);
#else
        std::cerr << message;
#endif        
    }
}