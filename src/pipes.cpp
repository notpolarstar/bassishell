/*
** EPITECH PROJECT, 2024
** aze
** File description:
** aze
*/

#include "bassishell.h"

void exec_cmd_no_fork(vector<string> words)
{
    if (see_if_pipes(words)) {
        handle_pipes(words);
        return;
    }
    if (see_if_redirections(words)) {
        handle_redirections(words);
        return;
    }
    if (builtins(words))
        return;
    int status = 0;
    pid_t pid;
    char **args = new char *[words.size() + 1];
    for (int i = 0; i < words.size(); i++) {
        args[i] = new char[words[i].length() + 1];
        strcpy(args[i], words[i].c_str());
    }
    args[words.size()] = nullptr;
    signal(SIGINT, SIG_DFL);
    execvp(args[0], args);
    cout << args[0] << ": Command not found." << endl;
    for (int i = 0; i < words.size() && args[0]; i++)
        delete[] args[i];
    delete[] args;
    exit(WEXITSTATUS(status));
}


int see_if_pipes(vector<string> words)
{
    for (const auto &word : words) {
        if (word == "|")
            return 1;
    }
    return 0;
}

int handle_pipes(vector<string> words)
{
    vector<string> words1;
    vector<string> words2;
    int i = 0;
    int j = 0;

    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        while (words[i] != "|" && i < words.size()) {
            words1.push_back(words[i]);
            i++;
        }
        signal(SIGINT, SIG_DFL);
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        exec_cmd_no_fork(words1);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        pid = fork();
        if (pid == 0) {
            while (words[i] != "|" && i < words.size()) {
                i++;
            }
            i++;
            while (i < words.size()) {
                words2.push_back(words[i]);
                i++;
            }
            signal(SIGINT, SIG_DFL);
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            exec_cmd_no_fork(words2);
            exit(0);
        } else {
            close(fd[0]);
            close(fd[1]);
            waitpid(pid, NULL, 0);
        }
    }
    return 0;
}
