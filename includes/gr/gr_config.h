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
 *****************************************************************************/

#pragma once

#include <git2.h>

#ifndef GIT_REAL_CONFIG_H 
#define GIT_REAL_CONFIG_H

namespace GitReal {

    class Configurator {
public:
        explicit Configurator(
            git_repository *repo
        );

        ~Configurator();

        bool config_key_exists(const char *key);

        bool get_bool_config(const char* gitreal_config_key);

        void set_bool_config(const char* gitreal_config_key, bool value);

        const char* get_str_config(const char* gitreal_config_key);

        void set_str_config(const char* gitreal_config_key, const char* value);

private:
        git_repository** m_repo = nullptr; 
        git_config* m_git_config = nullptr; 
        bool m_init = false; 
    };
}

#endif /* GIT_REAL_CONFIG_H */
