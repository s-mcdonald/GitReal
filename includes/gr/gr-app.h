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

#include <string>
#include <vector>
#include <map>

#ifndef GIT_REAL_APP_H 
#define GIT_REAL_APP_H

namespace GitReal {

    struct AppData {
        std::string config_file_path = "./config.ini";
        std::string default_repo_path = "./";
    };

    const u_int8_t SUCCESS_SYSCALL = 0;

    enum class Action {
        IGNORE = 0,
        UPDATE_HEADERS = 5,
        UPDATE_ALL = 10,
    };

    struct Rgba
    {
        float                                                     r, g, b, a;
        constexpr Rgba()                                        : r(0.0f), g(0.0f), b(0.0f), a(0.0f)            { }
        constexpr Rgba(float _r, float _g, float _b, float _a)  : r(_r * _a), g(_g * _a), b(_b * _a), a(_a)     { }
    };

    // we store data in the config.ini file in the 
    // user path /home/<user>/.gitreal/.config.ini
    struct GitConfig {
        std::string root;               // location of git repo
    };

    struct Preferences {
        Rgba backgroundColor = Rgba(0.015f, 0.175f, 0.260f, 0.10f);
        std::string log_path;
    };

    struct GitState {
        BranchMeta current_branch;
        std::vector<BranchMeta> local_branches;
        std::vector<std::string> remote_branches;
        std::vector<std::string> remotes;
        std::map<std::string, bool> fetch_flags;
    };

    struct AppContext {
        GitConfig config;
        GitState git_state;
        Preferences preferences;
    };
}

#endif /* GIT_REAL_APP_H */
