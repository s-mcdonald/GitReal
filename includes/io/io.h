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

#include <iomanip>
#include <git2.h>
#include <unordered_map>
#include <algorithm>

#include "io.h"
#include "gr-types.h"

 #ifndef GIT_REAL_IO_H 
 #define GIT_REAL_IO_H

 
 namespace GitReal {

    enum class ConsoleColor {
        RED = 0,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        DULL_WHITE,
        BRIGHT_WHITE,
        GREY,
        DEFAULT
    };
    
    class Console {
        public:
            explicit Console();

            ~Console();

            void print_repo_info(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options);

            void print_repo_info_tree(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options);

            void print_repo_info_simple(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options);

            constexpr Console& operator<<(const GitReal::Analyze::BranchInfo& b_info);

            constexpr Console& set_fg(ConsoleColor color);

            constexpr Console& set_bg(ConsoleColor color);

            constexpr Console& set_bg_color(ConsoleColor color);

            constexpr Console& reset_color();

            constexpr Console& operator--();

        private:
            void print_branch(const GitReal::Analyze::BranchInfo& b_info);
            void print_branch(const GitReal::Analyze::BranchInfo& b_info, InputOptions options);
            constexpr std::string get_foreground_color(ConsoleColor color);
            constexpr std::string get_background_color(ConsoleColor color);
    };
}

#endif /*  #define GIT_REAL_IO_H */