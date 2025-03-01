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


#include <iostream>
#include <git2.h>
#include <algorithm>
#include <unordered_map>
#include <sstream>

#include "git-branch_meta.h"

using namespace std;

namespace GitReal {

    BranchMetaInspector::BranchMetaInspector(
            git_repository *repo,
            git_branch_iterator *iter) 
    {
        this->repo = repo;
        this->iter = iter;
    }


    BranchMetaInspector::~BranchMetaInspector()
    {
        delete this->repo;
        delete this->iter;
    }


    // public:
    std::vector<BranchMeta> BranchMetaInspector::fetch_all_local_branch_meta() {
        std::vector<BranchMeta> branches;
        for (const auto &branch_name : _list_branches(GIT_BRANCH_LOCAL)) {
            BranchMeta br_data = _load_branch(branch_name);
            branches.push_back(br_data);
        }
        return branches;
    }

    std::vector<BranchMeta> BranchMetaInspector::fetch_all_local_branch_meta(bool with_wip) {

        if (with_wip == false) 
            return fetch_all_local_branch_meta();

        std::vector<BranchMeta> branches;
        for (const auto &branch_name : _list_branches(GIT_BRANCH_LOCAL)) {
            BranchMeta br_data = _load_branch_with_wip(branch_name);
            branches.push_back(br_data);
        }
        return branches;
    }

    BranchMeta BranchMetaInspector::fetch_current_branch_meta() {
        BranchMeta branch_meta;

        git_reference *head_ref = nullptr;
        if (git_repository_head(&head_ref, this->repo) == 0) {
            const char* branch_name = nullptr;

            if (git_branch_name(&branch_name, head_ref) == 0 ) {
                branch_meta = _load_branch(branch_name);
            }
        }

        git_reference_free(head_ref);
        // if not found make that clear and handle exceptions

        return branch_meta;
    }

    // private:
    std::string BranchMetaInspector::_get_branch_remote(git_repository *repo, const std::string &branch_name) {
        git_reference *branch_ref = nullptr;
        git_reference *upstream_ref = nullptr;
        const char *remote_name = "local";  // Default to "local" if no remote is tracked
    
        if (git_reference_lookup(&branch_ref, repo, ("refs/heads/" + branch_name).c_str()) != 0) {
            std::cerr << "Branch " << branch_name << " not found." << std::endl;
            return remote_name;
        }

        if (git_branch_upstream(&upstream_ref, branch_ref) == 0 && upstream_ref != nullptr) {
            if ( const char* r_n = git_reference_name(upstream_ref)) {
                remote_name = r_n;
            }
    
            git_reference_free(upstream_ref);  
        }
    
        git_reference_free(branch_ref);
        return remote_name;
    }

    std::vector<std::string> BranchMetaInspector::_list_branches(git_branch_t branch_flags) {
        std::vector<std::string> branches;
        git_reference *branch_ref = nullptr;
        while (git_branch_next(&branch_ref, &branch_flags, this->iter) == 0) {
            const char *branch_name;
    
            if (git_branch_name(&branch_name, branch_ref) != 0) {
                std::cerr << "Failed to get branch name." << std::endl;
                continue;
            }
    
            branches.push_back(branch_name);
        }
    
        git_reference_free(branch_ref);
    
        return branches;
    }

    std::string BranchMetaInspector::_clean_remote_name(const char *remote_ref) {
        std::string ref_str(remote_ref);
        
        // Split the string using '/'
        std::stringstream ss(ref_str);
        std::string part;
        std::vector<std::string> parts;
        
        while (std::getline(ss, part, '/')) {
            parts.push_back(part);
        }
        
        if (parts.size() > 2) {
            return parts[2].c_str(); 
        }
        
        return "";  
    }

    BranchMeta BranchMetaInspector::_load_branch(std::string branch_name)
    {
        BranchMeta br_data;
        br_data.name = branch_name;
        br_data.remote_branch_name = _get_branch_remote(this->repo, branch_name);
        br_data.remote_name = _clean_remote_name(br_data.remote_branch_name.c_str());
        br_data.is_current = _is_current(&branch_name);
        br_data.has_wip = false;
        br_data.is_porcelain = (br_data.is_current) ? _is_porcelain(&branch_name) : true;

        return br_data;
    }

    
    BranchMeta BranchMetaInspector::_load_branch_with_wip(std::string branch_name)
    {
        BranchMeta br_data;
        br_data.name = branch_name;
        br_data.remote_branch_name = _get_branch_remote(this->repo, branch_name);
        br_data.remote_name = _clean_remote_name(br_data.remote_branch_name.c_str());
        br_data.is_current = _is_current(&branch_name);
        br_data.has_wip = _has_wip(&branch_name);
        br_data.is_porcelain = (br_data.is_current) ? _is_porcelain(&branch_name) : true;

        return br_data;
    }

    bool BranchMetaInspector::_is_porcelain(std::string* branch_name) {

        bool is_clean = true;
        git_status_list *status_list = nullptr;

        // Get the status list
        if (git_status_list_new(&status_list, repo, NULL) < 0) {
            std::cerr << "Failed to get repository status." << std::endl;
            return is_clean;
        }
    
        // Get the number of status entries
        size_t status_count = git_status_list_entrycount(status_list);

        if (0 != status_count) {
            for (size_t i = 0; i < status_count; ++i) {
                const git_status_entry *status_entry = git_status_byindex(status_list, i);
                if (
                    status_entry->status == GIT_STATUS_CURRENT ||
                    status_entry->status == GIT_STATUS_IGNORED) {
                    continue;
                }

                is_clean = false;
            }
        }
    
        git_status_list_free(status_list);

        return is_clean;
    }

    bool BranchMetaInspector::_is_current(std::string* branch_name) {
        git_reference *branch_ref = nullptr;
    
        bool is_current = false;

        if (git_reference_lookup(&branch_ref, this->repo, ("refs/heads/" + *branch_name).c_str()) != 0) {
            std::cerr << "Failed to find branch: " << *branch_name << std::endl;
            return false;
        }

        is_current = git_branch_is_checked_out(branch_ref);

        return is_current;
    }

    bool BranchMetaInspector::_has_wip(std::string* branch_name) {

        git_reference *branch_ref = nullptr;
        git_commit *latest_commit = nullptr;
        bool has_wip = false;

        if (git_reference_lookup(&branch_ref, this->repo, ("refs/heads/" + *branch_name).c_str()) != 0) {
            std::cerr << "Failed to find branch: " << *branch_name << std::endl;
            return false;
        }

        git_object *branch_obj = nullptr;
        if (git_reference_peel(&branch_obj, branch_ref, GIT_OBJECT_COMMIT) != 0) {
            std::cerr << "Failed to get latest commit for branch: " << *branch_name << std::endl;
            git_reference_free(branch_ref);
            return false;
        }

        latest_commit = (git_commit *)branch_obj;

        const char *commit_message = git_commit_message(latest_commit);
        
        if (commit_message != nullptr && (std::string(commit_message).find("WIP"))) {
            has_wip = true;
        }

        git_reference_free(branch_ref);
        

        return has_wip;   
    
    }
}
