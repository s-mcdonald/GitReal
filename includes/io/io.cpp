#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <git2.h>

#include "io.h"
#include "gr-types.h"

namespace GitReal {

    Console::Console() 
    {

    }

    Console::~Console()
    {

    }
       
    void Console::print_repo_info(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options)
    {
        if (options.simple_list_mode == false) {
            print_repo_info_tree(repo_info, options);
            return;
        }

        print_repo_info_simple(repo_info, options);
    }

    void Console::print_repo_info_tree(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options)
    {        
        for (const auto& remote : repo_info.remote_branches) {
            const auto& branch_metas = remote.second;
            set_fg(GitReal::ConsoleColor::BRIGHT_WHITE);
            std::cout << " " << remote.first << std::endl;

            for (const auto& b_info : branch_metas) {
                print_branch(b_info, options);
            }

            std::cout << std::endl; 
        }

        set_fg(GitReal::ConsoleColor::BRIGHT_WHITE);
        std::cout << " = Untracked =" << std::endl; 
        for (const auto& b_info : repo_info.local_branches) {
            print_branch(b_info, options);
        }

        reset_color();

        std::cout << std::endl;
    }

    void Console::print_repo_info_simple(const GitReal::Analyze::RepositoryInfo& repo_info, const InputOptions options)
    {        
        for (const auto& b_info : repo_info.local_branches) {
            print_branch(b_info, options);
        }

        for (const auto& remote : repo_info.remote_branches) {
            const auto& branch_metas = remote.second;
            for (const auto& b_info : branch_metas) {
                print_branch(b_info, options);
            }
        }

        reset_color();
    }

    constexpr Console& Console::operator<<(const GitReal::Analyze::BranchInfo& b_info) {
        print_branch(b_info);
        return *this;
    }

    constexpr Console& Console::operator--() {
        std::cout << get_foreground_color(ConsoleColor::DEFAULT) << get_background_color(ConsoleColor::DEFAULT);
        return *this;
    }

    constexpr Console& Console::set_fg(ConsoleColor color) {
        std::cout << get_foreground_color(color);
        return *this;
    }

    constexpr Console& Console::set_bg(ConsoleColor color) {
        std::cout << get_background_color(color);
        return *this;
    }

    constexpr Console& Console::set_bg_color(ConsoleColor color) {
        std::cout << get_background_color(color);
        return *this;
    }
    
    constexpr Console& Console::reset_color() {
        --(*this);
        return *this;
    }

    // private
    void Console::print_branch(const GitReal::Analyze::BranchInfo& b_info) {

        reset_color();

        std::cout << "  ";

        set_fg(GitReal::ConsoleColor::WHITE);

        if (b_info.is_active && b_info.is_worktree_branch == false) {
            set_fg(GitReal::ConsoleColor::GREEN);
        }
        
        if (b_info.is_active && b_info.is_porcelain == false) {
            set_fg(GitReal::ConsoleColor::RED);
        }

        std::cout << " " << b_info.name << " ";

        reset_color();

        std::cout << "" << std::endl;
    }

    void Console::print_branch(const GitReal::Analyze::BranchInfo& b_info, InputOptions options) {

        std::cout << "  ";

        set_fg(GitReal::ConsoleColor::WHITE);

        if (b_info.is_active && b_info.is_worktree_branch == false) {
            set_fg(GitReal::ConsoleColor::GREEN);
        }
        
        if (b_info.is_active && b_info.is_porcelain == false) {
            set_fg(GitReal::ConsoleColor::RED);
        }

        std::cout << " " << b_info.name << " ";

        if (options.show_wip && b_info.is_wip == true) {
            set_bg_color(GitReal::ConsoleColor::BLUE);
            std::cout << "(w)";
            reset_color();
            std::cout << " ";
        }

        reset_color();

        if (options.show_tracked && b_info.remote_tracked_branch_name != "") {
            set_fg(GitReal::ConsoleColor::DULL_WHITE);
            std::cout << "->";
            std::cout << " " << b_info.remote_tracked_branch_name << " ";
            reset_color();
        }

        if (options.show_worktrees && b_info.is_active && b_info.is_porcelain == false) {
            std::cout << " 🔴";
        }

        // if (options.show_worktrees && b_info.is_active == true && b_info.is_worktree_branch == false) {
        //     std::cout << " ⭐";
        // }

        // if (options.show_worktrees && b_info.is_worktree_branch == true) {
        //     std::cout << "🌳";
        // }

        if (options.show_worktrees && b_info.is_worktree_branch) {
            std::cout << "🌳";
        }

        std::cout << std::endl;
    }

    constexpr std::string Console::get_foreground_color(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[91m";
            //case ConsoleColor::RED: return "\033[31m";
            case ConsoleColor::GREEN: return "\033[32m";
            case ConsoleColor::YELLOW: return "\033[33m";
            case ConsoleColor::BLUE: return "\033[34m";
            case ConsoleColor::MAGENTA: return "\033[35m";
            case ConsoleColor::CYAN: return "\033[36m";
            case ConsoleColor::WHITE: return "\033[80m";
            case ConsoleColor::DULL_WHITE: return "\033[90m";
            case ConsoleColor::BRIGHT_WHITE: return "\033[97m";
            case ConsoleColor::GREY: return "\033[90m";
            case ConsoleColor::DEFAULT:
            default: return "\033[0m"; // Default to reset
        }
    }

    constexpr std::string Console::get_background_color(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[41m";
            case ConsoleColor::GREEN: return "\033[42m";
            case ConsoleColor::YELLOW: return "\033[43m";
            case ConsoleColor::BLUE: return "\033[44m";
            case ConsoleColor::MAGENTA: return "\033[45m";
            case ConsoleColor::CYAN: return "\033[46m";
            case ConsoleColor::WHITE: return "\033[47m";
            case ConsoleColor::DULL_WHITE: return "\033[100m";
            case ConsoleColor::BRIGHT_WHITE: return "\033[107m";
            case ConsoleColor::GREY: return "\033[100m";
            case ConsoleColor::DEFAULT:
            default: return "\033[0m"; // Default to reset
        }
    }
}