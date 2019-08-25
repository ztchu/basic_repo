#!/usr/bin/env python
# This script is used for managing thirdparty for git reporitory.
# Only python3 is supported.

# Execute this script need gitpython module.
# You can execute "pip install gitpython" to install it.
# https://gitpython.readthedocs.io/en/stable/
# https://programtalk.com/python-examples/git.repo.base.Repo.clone_from/


# If you want to manage your thirdparty library by this script,
# you should create a deps file named "deps.json" in your src root directory.
# In deps file, you can set different commit id for different platform.
# The format of deps file is like this:
#"src/thirdparty/jsoncpp":[
#    {
#        "repository":"git@github.com:ztchu/jsoncpp.git",
#        "commit_id":"9249878229e03a594eb6276c8f0ed6f9c21cb51f",
#        "checkout_condition":"linux"
#    },
#    {
#        "repository":"git@github.com:ztchu/jsoncpp.git",
#        "commit_id":"43fd41d1fc31bfaa9606004fc89aa2b7b33d74c6",
#        "checkout_condition":"win"
#    }
#]

# The value that can be assigned to "checkout_condition":
# linux, win, all

# To sync your thirdparty library, you just should do:
# python gtool.py --sync


import argparse
import git
import json
import os
import platform
import sys
import threading
from concurrent.futures import ThreadPoolExecutor

deps_file_name = "deps.json"


def GitCloneOneRepo(dst_dir, git_path, commit_id):
    relative_path = dst_dir.replace("src", ".", 1)
    if not os.path.isdir(relative_path):
        os.makedirs(relative_path)

    try:
        repo = git.Repo(relative_path)
        remote_repo = repo.remote()
        if remote_repo.url != git_path:
            remote_repo.set_url(git_path, remote_repo.url)
        
        head = repo.head
        if str(head.commit) != commit_id:
            remote_repo.fetch()
            git_cmd = repo.git
            git_cmd.checkout(commit_id)
        else:
            print("The repository update nothing: " + remote_repo.url)
    except git.exc.InvalidGitRepositoryError:
        print("Start to clone  ----->>>>>  " + git_path)
        repo = git.Repo.clone_from(url = git_path, to_path = relative_path)
        git_cmd = repo.git
        git_cmd.checkout(commit_id)

def SyncThirdParty():
    with open(deps_file_name) as deps_file:
        load_dict = json.load(deps_file)
        max_worker_number = 4
        if len(load_dict) < max_worker_number:
            max_worker_number = len(load_dict)
        task_pool = ThreadPoolExecutor(max_workers = max_worker_number)
        for val in load_dict:
            for platform_info in load_dict[val]:
                if (sys.platform.find(platform_info["checkout_condition"]) != -1) \
                    or (platform_info["checkout_condition"] == "all"): 
                    task_pool.submit(GitCloneOneRepo, val, platform_info["repository"], \
                        platform_info["commit_id"])
        task_pool.shutdown()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--sync", action = "store_true", help = " Sync your third party library.")
    args = parser.parse_args()
    if args.sync:
        SyncThirdParty()

if __name__ == "__main__":
    sys.exit(main())