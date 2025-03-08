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

#include <iostream>
#include <git2.h>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <string.h>
#include <string_view>
#include <vector>
#include <string>
#include <utility> 
#include <filesystem>

#include "gr_types.h"
#include "gr_repo_state.h"

namespace GitReal::Analyze {

    RepositoryInfo get_repository_info(git_repository* repo, git_branch_iterator* iter)
    {
        git_strarray remotes;
        std::unordered_map<std::string, std::vector<BranchInfo>> remote_branches;
        std::vector<BranchInfo> local_branches;

        //
        // 1. Get a list of remotes
        //
        auto remote_list = gr_git_remotes_fetch_new_list(repo);
        remote_branches.reserve(remote_list.size());

        // 
        // 2. Get the full worktree list, this will come in handy later
        //
        WorkTreeMap worktrees = list_worktrees_new(repo);

        // 
        // 3. get all the branch Names
        //
        auto branch_infos = get_local_branch_info_list(repo, iter, worktrees);

        for (auto& branch_info : branch_infos)  
        {
            if (branch_info.remote == "") 
                local_branches.emplace_back(branch_info);
            else 
                remote_branches[branch_info.remote].emplace_back(branch_info);
        }

        RepositoryInfo repo_info;
        repo_info.local_branches = std::move(local_branches);
        repo_info.remote_branches = std::move(remote_branches);
        repo_info.path = ""; // get_repository_full_path(repo);
        repo_info.remotes = std::move(remote_list);

        return repo_info;
    };

    /**
     * @brief Create a WorkTreeMap of current WT in the repo
     */
    WorkTreeMap list_worktrees_new(git_repository* repo) {

        WorkTreeMap map;

        const auto worktree_names = get_worktree_names(repo);
    
        for (auto wt_name : worktree_names)
        {
            git_worktree* worktree = nullptr;
            const char* name = wt_name.c_str();
    
            // Lookup the worktree by name
            if (git_worktree_lookup(&worktree, repo, name) != 0) {
                // Failed to lookup worktree
                continue;
            }

            const char* worktree_path = git_worktree_path(worktree);
    
            std::string branch_name;
            git_repository* worktree_repo = nullptr;

            if (git_repository_open(&worktree_repo, worktree_path) == 0) {
                git_reference* head_ref = nullptr;
                if (git_repository_head(&head_ref, worktree_repo) == 0) {
                    if (git_reference_is_branch(head_ref)) {
                        const char* ref_name = git_reference_name(head_ref);
                        branch_name = std::string(ref_name).substr(11); // Remove "refs/heads/"
                    } else {
                        branch_name = "DETACHED";
                    }
                    git_reference_free(head_ref);
                } else {
                    branch_name = "UNKNOWN";
                }
    
                git_repository_free(worktree_repo);
            } else {
                branch_name = "UNKNOWN";
            }

            WorkTreeAssociation worktree_info;
            worktree_info.branch_name = std::move(branch_name);
            worktree_info.path = worktree_path;

            map.emplace(branch_name, worktree_path);
    
            git_worktree_free(worktree);
        }
    
        return map;
    }

    /**
     * @brief Creates a vector of BranchInfo from given input
     */
    std::vector<BranchInfo> get_local_branch_info_list(git_repository* repo, git_branch_iterator* iter, WorkTreeMap wt_map) {

        std::vector<BranchInfo> branch_infos;
        git_branch_t branch_flags = GIT_BRANCH_LOCAL;
        git_reference *branch_ref = nullptr;

        while (git_branch_next(&branch_ref, &branch_flags, iter) == 0) {

            const char* branch_name = nullptr;
            if (git_branch_name(&branch_name, branch_ref) != 0) {
                //  Failed to get branch name
                git_reference_free(branch_ref);
                continue;
            }

            BranchInfo branch_info = create_branch_info(repo, branch_ref, branch_name);

            branch_infos.push_back(std::move(branch_info));
            git_reference_free(branch_ref);
        }

        // Apply worktree branch status
        for (auto& branch_info : branch_infos) {
            auto path_to_workspace = wt_map.find(branch_info.name);
            if (path_to_workspace != wt_map.end() && !path_to_workspace->second.empty()) {
                branch_info.is_worktree_branch = true;
            }
        }
        
        return branch_infos;
    }

    /**
     * @brief Create BranchInfo from given input
     */
    inline BranchInfo create_branch_info(git_repository* repo, git_reference* branch_ref, const char* branch_name) {
        BranchInfo branch_info;
        branch_info.name = branch_name;
        
        auto x =  gr_git_branch_remote_name(branch_ref);
        branch_info.remote = gr_clean_remote_name(x); // gets origin | upstream etc..
        branch_info.remote_tracked_branch_name = gr_git_branch_remote_branch_name(branch_ref);

        branch_info.is_wip = git_branch_has_wip(branch_ref);
        branch_info.is_head = git_branch_is_head(branch_ref);
        branch_info.is_checked_out = git_branch_is_checked_out(branch_ref);
        branch_info.is_active = branch_info.is_head && branch_info.is_checked_out;
        branch_info.is_worktree_branch = false; // update this later
        branch_info.is_porcelain = git_is_porcelain(repo);
        return branch_info;
    }

    /**
     * @brief Create a simple vector of Worktree branch names.
     */
    std::vector<std::string> get_worktree_names(git_repository* repo) {
        git_strarray worktree_list;
        std::vector<std::string> list;
    
        if (git_worktree_list(&worktree_list, repo) != 0) {
            return list;
        }
    
        list.reserve(worktree_list.count);
        list.assign(worktree_list.strings, worktree_list.strings + worktree_list.count);
    
        git_strarray_dispose(&worktree_list);
    
        return list;
    }

    /**
     * @brief WIP commits is a commit on the HEAD where commit message == "WIP"
     */
    bool git_branch_has_wip(git_reference *branch_ref) {

        git_commit *latest_commit = nullptr;
        bool has_wip = false;

        git_object *branch_obj = nullptr;
        if (git_reference_peel(&branch_obj, branch_ref, GIT_OBJECT_COMMIT) != 0) {
            return false;
        }

        latest_commit = (git_commit*) branch_obj;

        const char *commit_message = git_commit_message(latest_commit);
        
        if (commit_message != nullptr)
        {
            if (
                std::string_view(commit_message).find("WIP") != std::string_view::npos || 
                std::string_view(commit_message).find("wip") != std::string_view::npos
            ) has_wip = true;   
        }

        git_commit_free(latest_commit);

        return has_wip;   
    }

    /**
     * @brief Similar to git status --porcelain, true if clean.
     */
    bool git_is_porcelain(git_repository* repo) {

        bool is_clean = true;
        git_status_list* status_list = nullptr;

        if (git_status_list_new(&status_list, repo, NULL) < 0) {
            // Failed to retrieve, We could log this..
            return is_clean;
        }
    
        // Get the number of status entries
        size_t status_count = git_status_list_entrycount(status_list);

        for (size_t i = 0; i < status_count; ++i) {
            // dont free this entry.
            const git_status_entry* status_entry = git_status_byindex(status_list, i);
            if (
                status_entry->status == GIT_STATUS_CURRENT ||
                status_entry->status == GIT_STATUS_IGNORED) {
                continue;
            }

            is_clean = false;

            break;
        }
    
        git_status_list_free(status_list);

        return is_clean;
    }

    /**
     * @return true if the current branch is active to the repo
     */
    inline bool git_is_active(git_repository* repo, git_reference *branch_ref) {

        bool checked_out = git_branch_is_checked_out(branch_ref);

        bool at_head = git_branch_is_head(branch_ref);

        return checked_out && at_head;
    }

    /**
     * Get the remote name for a given branch. Will return "local" if no remote.
     */
    std::string gr_git_branch_remote_name(git_reference *branch_ref) {

        git_reference *upstream_ref = nullptr;
        const char *remote_name = "local"; 

        if (git_branch_upstream(&upstream_ref, branch_ref) == 0 && upstream_ref != nullptr) {
            if ( const char* r_n = git_reference_name(upstream_ref)) {
                remote_name = r_n;
            }
    
            git_reference_free(upstream_ref);  
        }
    
        return remote_name;
    }

    std::string gr_git_branch_remote_branch_name(git_reference *branch_ref) {
        git_reference *upstream_ref = nullptr;
        std::string result = "";
    
        if (git_branch_upstream(&upstream_ref, branch_ref) == 0 && upstream_ref != nullptr) {
            if (const char* ref_name = git_reference_name(upstream_ref)) {
                std::string_view ref(ref_name);
                constexpr std::string_view prefix = "refs/remotes/";
                
                if (ref.starts_with(prefix)) {
                    result = std::string(ref.substr(prefix.length()));
                } else {
                    result = std::string(ref);
                }
            }
            
            git_reference_free(upstream_ref);
        }
    
        return result;
    }

    std::string gr_clean_remote_name(std::string& ref_str) {
        std::stringstream ss(ref_str);
        std::string part;
        std::vector<std::string> parts;
        
        while (std::getline(ss, part, '/')) {
            parts.push_back(part);
        }
        
        if (parts.size() > 2) {
            return parts[2]; // Return the std::string directly
        }
        
        return "";  
    }

    std::vector<std::string> gr_git_remotes_fetch_new_list(git_repository* repo) {
        std::vector<std::string> v_remotes;
        git_strarray remotes;
    
        if (git_remote_list(&remotes, repo) != 0) {
            return v_remotes;
        }
    
        v_remotes = git_strarray_to_vector(remotes);
        git_strarray_dispose(&remotes);
    
        return v_remotes;
    }

    /**
     * Copy git_strarray to vector
     */
    inline std::vector<std::string> git_strarray_to_vector(const git_strarray& strarray) {
        std::vector<std::string> vec;        
        vec.reserve(strarray.count);
        vec.assign(strarray.strings, strarray.strings + strarray.count);
        return vec;
    }
}
