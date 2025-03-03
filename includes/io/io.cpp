#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <iomanip>
#include <git2.h>
#include <unordered_map>
#include <algorithm>

#include <chrono>
#include <ctime>

#include "io.h"
#include "git-branch_meta.h"

namespace GitReal {

    Console::Console() 
    {

    }

    void Console::print_title(const char* title)
    {
        set_color(GitReal::ConsoleColor::MAGENTA);
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;
        std::cout << " //  " << title << std::endl;
        std::cout << " " << std::endl;            
        std::cout << " " << std::endl;
        reset_color();
    }

    void Console::cout(const char* value)
    {
        std::cout << value;
    }
        
    void Console::print_simple_list(const std::vector<BranchMeta>& branch_meta_v) {
 
        for (const auto& b_meta : branch_meta_v) {
            std::cout << "  ";

            if (b_meta.is_current) {
                set_color(GitReal::ConsoleColor::GREEN);
            }
            std::cout << b_meta.name;
            reset_color();
            std::cout << "" << std::endl;
        }

        std::cout << std::endl; 
          
    }

    /**
     * This will tajke the vector and structure the output for
     * display in terminal.
     */
    void Console::print_branch_tree(const std::vector<BranchMeta>& branch_meta_v, bool p) {

        std::unordered_map<std::string, std::vector<BranchMeta>> remote_branches;
        std::vector<BranchMeta> local_branches;

        for (const auto& b_meta : branch_meta_v) {
            const std::string remote = b_meta.remote_name == "" ? "local": b_meta.remote_name; // fix this
            if (remote == "local") {
                local_branches.push_back(b_meta);
            } else {
                remote_branches[remote].push_back(b_meta);
            }
        }

        for (const auto& remote : remote_branches) {
            const auto branch_metas = remote.second;

            set_color(GitReal::ConsoleColor::WHITE);

            std::cout << " " << remote.first << std::endl;
            reset_color();

            for (const auto& b_meta : branch_metas) {
                std::cout << "  ";

                set_color(GitReal::ConsoleColor::GREY);
                if (b_meta.is_current) {
                    set_color(GitReal::ConsoleColor::GREEN);
                }
                if (b_meta.is_porcelain == false) {
                    set_color(GitReal::ConsoleColor::RED);
                }
                std::cout << " " << b_meta.name << " ";
                
                if (b_meta.has_wip == true && true == p) {
                    set_color(GitReal::ConsoleColor::WHITE);
                    set_bg_color(GitReal::ConsoleColor::BLUE);
                    std::cout << " W ";
                    reset_color();
                }

                reset_color();
                std::cout << "" << std::endl;
            }

            std::cout << std::endl; 
        }

        if (local_branches.size() > 0) {
            set_color(GitReal::ConsoleColor::WHITE);
            std::cout << " " << "local" << std::endl;
            reset_color();
        }

        for (const auto& b_meta : local_branches) {
            std::cout << "  ";

            set_color(GitReal::ConsoleColor::GREY);

            if (b_meta.is_current) {
                set_color(GitReal::ConsoleColor::GREEN);
            }
            if (b_meta.is_porcelain == false) {
                set_color(GitReal::ConsoleColor::RED);
            }
            std::cout << " " << b_meta.name << " ";
            
            if (b_meta.has_wip == true) {
                set_color(GitReal::ConsoleColor::WHITE);
                set_bg_color(GitReal::ConsoleColor::BLUE);
                std::cout << " W ";
                reset_color();
            }

            std::cout << "" << std::endl;
            std::cout << std::endl; 
        }
        reset_color();
        std::cout << std::endl;
    }  

    Console& Console::operator<<(const BranchMeta& value) {
        std::cout << "Branch Details ----- " << std::endl;
        std::cout << "Branch : " << value.name << std::endl;
        std::cout << "WIP    : " <<(value.has_wip ? "Yes" : "No") << std::endl;
        std::cout << "Clean  : " <<(value.is_porcelain ? "Yes" : "No") << std::endl;
        return *this;
    }

    Console& Console::operator<<(const std::vector<BranchMeta>& branch_meta_v) {
        print_branch_tree(branch_meta_v, false);
        return *this;
    }

    Console& Console::operator--() {
        std::cout << get_foreground_color(ConsoleColor::DEFAULT) << get_background_color(ConsoleColor::DEFAULT);
        return *this;
    }

    Console& Console::set_color(ConsoleColor color) {
        std::cout << get_foreground_color(color);
        return *this;
    }

    Console& Console::set_bg_color(ConsoleColor color) {
        std::cout << get_background_color(color);
        return *this;
    }
    
    Console& Console::reset_color() {
        --(*this);
        return *this;
    }

    std::string Console::get_foreground_color(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[31m";
            case ConsoleColor::GREEN: return "\033[32m";
            case ConsoleColor::YELLOW: return "\033[33m";
            case ConsoleColor::BLUE: return "\033[34m";
            case ConsoleColor::MAGENTA: return "\033[35m";
            case ConsoleColor::CYAN: return "\033[36m";
            case ConsoleColor::WHITE: return "\033[37m";
            case ConsoleColor::GREY: return "\033[90m";
            case ConsoleColor::DEFAULT:
            default: return "\033[0m"; // Default to reset
        }
    }

    std::string Console::get_background_color(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[41m";
            case ConsoleColor::GREEN: return "\033[42m";
            case ConsoleColor::YELLOW: return "\033[43m";
            case ConsoleColor::BLUE: return "\033[44m";
            case ConsoleColor::MAGENTA: return "\033[45m";
            case ConsoleColor::CYAN: return "\033[46m";
            case ConsoleColor::WHITE: return "\033[47m";
            case ConsoleColor::GREY: return "\033[100m";
            case ConsoleColor::DEFAULT:
            default: return "\033[0m"; // Default to reset
        }
    }
}