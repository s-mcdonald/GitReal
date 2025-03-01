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
        setColor(GitReal::ConsoleColor::MAGENTA);
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;
        std::cout << " //  " << title << std::endl;
        std::cout << " " << std::endl;            
        std::cout << " " << std::endl;
        resetColor();
    }

    void Console::cout(const char* value)
    {
        std::cout << value;
    }
        
    void Console::print_simple_list(const std::vector<BranchMeta>& branch_meta_v) {
 
        // // Output each branch under the corresponding remote
        for (const auto& b_meta : branch_meta_v) {
            std::cout << "  ";

            if (b_meta.is_current) {
                setColor(GitReal::ConsoleColor::GREEN);
            }
            std::cout << b_meta.name;
            resetColor();
            std::cout << "" << std::endl;
        }

        std::cout << std::endl; 
          
    }

    /**
     * This will tajke the vector and structure the output for
     * display in terminal.
     */
    void Console::print_branch_tree(const std::vector<BranchMeta>& branch_meta_v, bool p) {

        std::unordered_map<std::string, std::vector<BranchMeta>> grouped_branches;

        for (const auto& b_meta : branch_meta_v) {
            const std::string remote = b_meta.remote_name == "" ? "local": b_meta.remote_name; // fix this
            grouped_branches[remote].push_back(b_meta);
        }

        for (const auto& remote : grouped_branches) {

            const auto remote_name = remote.first;
            const auto branch_metas = remote.second;

            setColor(GitReal::ConsoleColor::WHITE);

            std::cout << " " << remote_name << std::endl;
            resetColor();


            // // Output each branch under the corresponding remote
            for (const auto& b_meta : branch_metas) {
                std::cout << "  ";

                if (b_meta.is_current) {
                    setColor(GitReal::ConsoleColor::GREEN);
                }
                if (b_meta.is_porcelain == false) {
                    setColor(GitReal::ConsoleColor::RED);
                }
                std::cout << " " << b_meta.name << " ";
                
                if (b_meta.has_wip == false && true == p) {
                    setColor(GitReal::ConsoleColor::WHITE);
                    setBgColor(GitReal::ConsoleColor::BLUE);
                    std::cout << "[w]";
                }

                resetColor();

                std::cout << "" << std::endl;

            }

            std::cout << std::endl; 
        }
    }
    
    Console& Console::setColor(ConsoleColor color) {
        std::cout <<  getForegroundColor(color);
        return *this;
    }

    Console& Console::setBgColor(ConsoleColor color) {
        std::cout <<  getBackgroundColor(color);
        return *this;
    }
    
    Console& Console::resetColor() {
        std::cout << "\033[0m";  // ANSI reset code
        return *this;
    }

    std::string Console::getForegroundColor(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[31m";
            case ConsoleColor::GREEN: return "\033[32m";
            case ConsoleColor::YELLOW: return "\033[33m";
            case ConsoleColor::BLUE: return "\033[34m";
            case ConsoleColor::MAGENTA: return "\033[35m";
            case ConsoleColor::CYAN: return "\033[36m";
            case ConsoleColor::WHITE: return "\033[37m";
            default: return "\033[0m"; // Default to reset
        }
    }

    std::string Console::getBackgroundColor(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[41m";
            case ConsoleColor::GREEN: return "\033[42m";
            case ConsoleColor::YELLOW: return "\033[43m";
            case ConsoleColor::BLUE: return "\033[44m";
            case ConsoleColor::MAGENTA: return "\033[45m";
            case ConsoleColor::CYAN: return "\033[46m";
            case ConsoleColor::WHITE: return "\033[47m";
            default: return "\033[0m"; // Default to reset
        }
    }
}