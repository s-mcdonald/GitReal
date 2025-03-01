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
 * Class Name: BranchMetaInspector
 * 
 * Author: Sam McDonald
 * Date: 2025-02-22
 * 
 * Description:
 * 
 * This class provides access to the git repository meta data about a 
 * branch or branches using the below public members.
 * 
 *      ###  std::vector<BranchMeta> fetch_all_local_branch_meta();
 *
 *      ###  BranchMeta fetch_current_branch_meta();
 * 
 *****************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <git2.h>

#ifndef GIT_REAL_BRANCH_META_H 
#define GIT_REAL_BRANCH_META_H

struct BranchMeta {
    std::string name = "";
    std::string remote_name = "";
    std::string remote_branch_name = "";
    bool has_wip = false;
    bool is_porcelain = true;
    bool is_current = false;
};

namespace GitReal 
{

    class BranchMetaInspector 
    {
public:
        explicit BranchMetaInspector(
            git_repository *repo, 
            git_branch_iterator *iter
        );

        ~BranchMetaInspector();
        
        std::vector<BranchMeta> fetch_all_local_branch_meta();
        std::vector<BranchMeta> fetch_all_local_branch_meta(bool with_wip);

        BranchMeta fetch_current_branch_meta();

private:
        std::string _get_branch_remote(git_repository *repo, const std::string &branch_name);
        std::vector<std::string> _list_branches(git_branch_t branch_flags); 
        std::string _clean_remote_name(const char *remote_ref);
        BranchMeta _load_branch(std::string branch_name);
        BranchMeta _load_branch_with_wip(std::string branch_name);

        bool _is_porcelain(std::string* branch_name);
        bool _is_current(std::string* branch_name);
        bool _has_wip(std::string* branch_name);
        
        git_branch_iterator *iter;
        git_repository *repo;
    };

}


#endif /* GIT_REAL_BRANCH_META_H */
