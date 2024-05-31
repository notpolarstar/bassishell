/*
** EPITECH PROJECT, 2024
** aze
** File description:
** ze
*/

#include "bassishell.h"

vector<string> str_2_wordarray(string str, string delim)
{
    vector<string> words;
    char *token = NULL;
    char *cstr = new char[str.length() + 1];
    char *cstr_delim = new char[delim.length() + 1];

    strcpy(cstr, str.c_str());
    strcpy(cstr_delim, delim.c_str());
    token = strtok(cstr, cstr_delim);
    while (token != NULL) {
        words.push_back(token);
        token = strtok(NULL, cstr_delim);
    }
    delete[] cstr;
    delete[] cstr_delim;
    return words;
}

int builtins(vector<string> words)
{
    if (words[0] == "cd") {
        if (words.size() < 2)
            words.push_back(getenv("HOME"));
        if (words[1] == "-")
            words[1] = getenv("OLDPWD");
        if (chdir(words[1].c_str()) == -1)
            cout << words[1] << ": No such file or directory" << endl;
        setenv("OLDPWD", getenv("PWD"), 1);
        char *pwd = getcwd(NULL, 0);
        setenv("PWD", pwd, 1);
        free(pwd);
        return 1;
    }
    if (words[0] == "setenv") {
        if (words.size() < 2) {
            for (char **env = environ; *env; ++env)
                cout << *env << endl;
            return 1;
        }
        if (words.size() == 2) {
            words.push_back("");
        }
        setenv(words[1].c_str(), words[2].c_str(), 1);
        return 1;
    }
    if (words[0] == "unsetenv") {
        unsetenv(words[1].c_str());
        return 1;
    }
    if (words[0] == "env") {
        for (char **env = environ; *env; ++env)
            cout << *env << endl;
        return 1;
    }
    if (words[0] == "builtins") {
        cout << "cd\tenv\tsetenv\tunsetenv\texit" << endl;
        return 1;
    }
    if (words[0] == "exit") {
        return 1;
    }
    return 0;
}

int exec_cmd(vector<string> words)
{
    int status = 0;

    if (see_if_pipes(words)) {
        status = handle_pipes(words);
        return status;
    }
    if (see_if_redirections(words)) {
        status = handle_redirections(words);
        return status;
    }
    if (builtins(words))
        return status;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        char **args = new char *[words.size() + 1];
        for (int i = 0; i < words.size(); i++) {
            args[i] = new char[words[i].length() + 1];
            strcpy(args[i], words[i].c_str());
        }
        args[words.size()] = NULL;
        signal(SIGINT, SIG_DFL);
        execvp(args[0], args);
        cout << args[0] << ": Command not found." << endl;
        for (int i = 0; i < words.size() && args[0]; i++)
            delete[] args[i];
        delete[] args;
        exit(WEXITSTATUS(status));
    } else {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            cout << strsignal(WTERMSIG(status)) << endl;
    }
    return WEXITSTATUS(status);
}

string add_spaces_between_operators(string words)
{
    string operators = "|><";
    string new_words = "";

    for (int i = 0; i < words.size(); i++) {
        if (operators.find(words[i]) != string::npos) {
            if (i > 0 && operators.find(words[i - 1]) == string::npos)
                new_words += " ";
            new_words += words[i];
            if (i + 1 < words.size() && operators.find(words[i + 1]) == string::npos)
                new_words += " ";
        } else {
            new_words += words[i];
        }
    }
    return new_words;
}

int main_loop(string words, int status)
{
    vector <string> args;

    if (words.empty())
        return status;
    words = add_spaces_between_operators(words);
    args = str_2_wordarray(words, " ");
    status = exec_cmd(args);
    return status;
}

void print_prompt(void)
{
    char *user = getenv("USER");
    char *dir = getenv("PWD");

    cout << "\e[1;31m[" << user << "\e[1;33m@\e[1;32m" << dir << "] " << "\e[0m";
}

void signal_handler(int signum)
{
    cout << endl;
    print_prompt();
    cout << flush;
}

int main(void)
{
    string input;
    vector<string> lines = {"", ""};
    int status = 0;

    signal(SIGINT, signal_handler);
    while ((lines.size() == 0 || lines[0] != "exit") && cin.good()) {
        print_prompt();
        getline(cin, input);
        if (input.empty())
            continue;
        lines = str_2_wordarray(input, ";");
        for (int i = 0; i < lines.size(); i++)
            status = main_loop(lines[i], status);
    }
    if (cin.eof())
        cout << "exit" << endl;
    return status;
}
