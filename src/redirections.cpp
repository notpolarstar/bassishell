/*
** EPITECH PROJECT, 2024
** aze
** File description:
** aze
*/

#include "bassishell.h"

int see_if_redirections(vector<string> words)
{
    for (const auto &word : words) {
        if (word == ">" || word == ">>" || word == "<" || word == "<<")
            return 1;
    }
    return 0;
}

void redirect_right(vector<string> words, vector<string> words2)
{
    int fd = open(words2[0].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int save_stdout = dup(1);

    dup2(fd, 1);
    exec_cmd(words);
    dup2(save_stdout, 1);
    close(fd);
}

void redirect_double_right(vector<string> words, vector<string> words2)
{
    int fd = open(words2[0].c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
    int save_stdout = dup(1);

    dup2(fd, 1);
    exec_cmd(words);
    dup2(save_stdout, 1);
    close(fd);
}

void redirect_left(vector<string> words, vector<string> words2)
{
    int fd = open(words2[0].c_str(), O_RDONLY);
    int save_stdin = dup(0);

    dup2(fd, 0);
    exec_cmd(words);
    dup2(save_stdin, 0);
    close(fd);
}

void redirect_double_left(vector<string> words, vector<string> words2)
{
    FILE *tmp = tmpfile();
    string end = words2[0];
    string line;
    int save_stdin = dup(0);

    while (1) {
        cout << "> ";
        getline(cin, line);
        if (line == end)
            break;
        fputs(line.c_str(), tmp);
        fputs("\n", tmp);
    }
    fseek(tmp, 0, SEEK_SET);
    dup2(fileno(tmp), 0);
    exec_cmd(words);
    dup2(save_stdin, 0);
    fclose(tmp);
}

int handle_redirections(vector<string> words)
{
    vector<string> words1;
    vector<string> words2;
    int i = 0;
    int j = 0;

    while (words[i] != ">" && words[i] != ">>" && words[i] != "<" && words[i] != "<<") {
        words1.push_back(words[i]);
        i++;
    }
    j = i;
    i++;
    while (i < words.size()) {
        words2.push_back(words[i]);
        i++;
    }
    if (words[j] == ">" && words2.size())
        redirect_right(words1, words2);
    if (words[j] == ">>" && words2.size())
        redirect_double_right(words1, words2);
    if (words[j] == "<" && words2.size())
        redirect_left(words1, words2);
    if (words[j] == "<<" && words2.size())
        redirect_double_left(words1, words2);
    return 0;
}
