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

#include <string>
#include <vector>
#include <git2.h>
#include "gr-types.h"

#ifndef GIT_REAL_REPO_STATE_H 
#define GIT_REAL_REPO_STATE_H

namespace GitReal::Analyze
{
    RepositoryInfo get_repository_info(git_repository* repo, git_branch_iterator* iter);

    WorkTreeMap list_worktrees_new(git_repository* repo);

    std::vector<BranchInfo> get_local_branch_info_list(git_repository* repo, git_branch_iterator* iter, WorkTreeMap wt_map);

    std::vector<std::string> get_worktree_names(git_repository* repo);

    bool git_branch_has_wip(git_reference *branch_ref);

    bool git_is_porcelain(git_repository* repo);

    inline bool git_is_active(git_repository* repo, git_reference *branch_ref);

    inline BranchInfo create_branch_info(git_repository* repo, git_reference* branch_ref, const char* branch_name);

    std::string gr_git_branch_remote_name(git_reference *branch_ref);

    std::string gr_git_branch_remote_branch_name(git_reference *branch_ref);

    inline std::vector<std::string> git_strarray_to_vector(const git_strarray& strarray);

    std::string gr_clean_remote_name(std::string& ref_str);

    std::vector<std::string> gr_git_remotes_fetch_new_list(git_repository* repo);
}


#endif /* GIT_REAL_REPO_STATE_H */
