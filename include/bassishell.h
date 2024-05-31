/*
** EPITECH PROJECT, 2024
** aze
** File description:
** aze
*/

#ifndef aze
#define aze

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

vector<string> str_2_wordarray(string str, string delim);
int builtins(vector<string> words);
int exec_cmd(vector<string> words);
void exec_cmd_no_fork(vector<string> words);
int handle_pipes(vector<string> words);
int see_if_pipes(vector<string> words);
void redirect_right(vector<string> words, vector<string> words2);
void redirect_left(vector<string> words, vector<string> words2);
void redirect_double_right(vector<string> words, vector<string> words2);
void redirect_double_left(vector<string> words, vector<string> words2);
int see_if_redirections(vector<string> words);
int handle_redirections(vector<string> words);

#endif /* !aze */
