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


#include <iostream>
#include <git2.h>

#include "gr-config.h"

using namespace std;

namespace GitReal {

    Configurator::Configurator(git_repository *repo) 
    {
        if (git_repository_config(&m_git_config, repo) != 0) {
            std::cerr << "Failed to get repository config." << std::endl;
            return;
        }

        m_repo = &repo;
        m_init = true;
    }

    Configurator::~Configurator() 
    {
        git_config_free(m_git_config);
    }

    // public:
    bool Configurator::config_key_exists(const char* key) {

        bool exist = false;

        const char *value = nullptr;

        git_config_get_string(&value, m_git_config, key);

        if (value != nullptr) {
            exist = true;
        }

        delete value;

        return exist;
    }

    bool Configurator::get_bool_config(const char* gitreal_config_key) {

        bool default_value = false;

        const char *config_key = "gitreal.";
        int *out_val = nullptr;

        std::string full_config_key = std::string(config_key) + gitreal_config_key;

        if (git_config_get_bool(out_val, m_git_config, full_config_key.c_str()) != 0) {
            return default_value;
        }

        return out_val;
    }

    void Configurator::set_bool_config(const char* gitreal_config_key, bool value) {

        const char *config_key = "gitreal.";

        std::string full_config_key = std::string(config_key) + gitreal_config_key;

        if (false == config_key_exists(full_config_key.c_str())) {
            if (git_config_set_bool(m_git_config, full_config_key.c_str(), value) != 0) {
                std::cerr << "Failed to set config." << std::endl;
                return;
            }
            std::cout << "Set config to '" << value << "'." << std::endl;
            return ;
        } 
    }

    void Configurator::set_str_config(const char* gitreal_config_key, const char* value) {

        const char *config_key = "gitreal.";

        std::string full_config_key = std::string(config_key) + gitreal_config_key;

        if (git_config_set_string(m_git_config, full_config_key.c_str(), value) != 0) {
            std::cerr << "Failed to set config." << std::endl;
            return;
        }
        std::cout << "Set config to '" << value << "'." << std::endl;
        return;
    }

    const char* Configurator::get_str_config(const char* gitreal_config_key) {

        const char *config_key = "gitreal.";

        const char* out_val = nullptr;

        std::string full_config_key = std::string(config_key) + gitreal_config_key;

        std::cerr << "The key: " << full_config_key << std::endl;

        if (git_config_get_string(&out_val, m_git_config, full_config_key.c_str()) != 0) {
            std::cerr << "Failed to get config." << std::endl;
            return out_val;
        }

        return out_val;
    }
}
