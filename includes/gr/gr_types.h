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

#ifndef GIT_REAL_TYPES_H 
#define GIT_REAL_TYPES_H


#include <iostream>
#include <git2.h>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <string.h>
#include <vector>
#include <string>
#include <utility> 

struct InputOptions {
    bool show_wip = false;
    bool show_help = false;
    bool show_worktrees = false;
    bool simple_list_mode = false;
    bool show_tracked = false;
};


namespace GitReal::Analyze {

    typedef std::unordered_map<std::string,std::string> WorkTreeMap;

    typedef struct {
        std::string name;
        std::string remote; // origin | upstream | nullptre | etc
        std::string remote_tracked_branch_name; // actual remote branch name
        bool is_wip = false;
        bool is_porcelain = true;
        bool is_active = false;
        bool is_checked_out = false;
        bool is_head = false;
        bool is_worktree_branch = false;
    } BranchInfo;
    
    struct RepositoryInfo {
        std::string path;
        std::unordered_map<std::string, std::vector<BranchInfo>> remote_branches;
        std::vector<BranchInfo> local_branches;
        std::vector<std::string> remotes;
    };

    struct WorkTreeAssociation {
        std::string branch_name;
        std::string path;
    };

    typedef std::vector<WorkTreeAssociation> WorkTreeListNew;
}


#endif /* GIT_REAL_TYPES_H */

