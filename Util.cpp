//
// Created by shahrooz on 10/29/20.
//

#include "Util.h"
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <ncurses.h>
#include <math.h>

bool DEBUG_Subtitile = false;
bool DEBUG_VLC_interface = false;
bool DEBUG_Handler = false;

using namespace std;

int execute(const std::string &program, const std::vector<std::string> &args, std::string &output) {
  pid_t pid;
  int mypipe[2];

  char **argv = new char *[args.size() + 2];
  size_t i = 0;
  argv[i] = new char[program.size() + 1];
  strcpy(argv[i], program.c_str());
  i++;
  for (auto &arg: args) {
    argv[i] = new char[arg.size() + 1];
    strcpy(argv[i], arg.c_str());
    i++;
  }
  argv[i] = NULL;

  if (pipe(mypipe)) {
    cerr << "Pipe failed: " << errno << endl;
    exit(3);
  }

  do {
    pid = fork();
    if (pid == (pid_t) 0) { // Child
      close(mypipe[0]);
      setvbuf(stdout, NULL, _IONBF, 0);
      dup2(mypipe[1], STDOUT_FILENO);
      execvp(argv[0], argv);
      exit(4);
    } else {
      if (pid < (pid_t) 0) { // Fork failed
        if (errno != EAGAIN) {
          cerr << "Fork failed: " << pid << " " << errno << endl;
          close(mypipe[0]);
          close(mypipe[1]);
          for (size_t j = 0; j < i; j++) {
            delete[] argv[j];
          }
          delete[] argv;
          exit(3);
        }
      } else { // Parent
        close(mypipe[1]);
        for (size_t j = 0; j < i; j++) {
          delete[] argv[j];
        }
        delete[] argv;

        char buf[1024];
        ssize_t n_read;
        while ((n_read = read(mypipe[0], buf, 1024)) > 0) {
          output += string(buf, n_read);
        }
        close(mypipe[0]);
        int wstatus = 0;
        if (waitpid(pid, &wstatus, 0) != pid) {
          cerr << "error in waitpid(pid, &wstatus, 0)" << endl;
          return -1;
        }

        if (!WIFEXITED(wstatus)) {
          cerr << "Child with pid " << pid << " did not exit normally" << endl;
          return -1;
        }
        return WEXITSTATUS(wstatus);
      }
    }
  } while (true);
}

int execute(const std::string &program) {
  std::string output;
  return execute(program, output);
}

int execute(const std::string &program, const std::vector<std::string> &args) {
  std::string output;
  return execute(program, args, output);
}

int execute(const std::string &program, std::string &output) {
  std::vector<std::string> args;
  return execute(program, args, output);
}

void print_center_oneline(const std::string &text, int row) {
  int center_col = stdscr->_maxx / 2;
  size_t center_text = text.size() / 2;

  mvwprintw(stdscr, row, center_col - static_cast<int>(center_text), text.c_str());
}

size_t print_center(const std::string &text, size_t row) {
  size_t line_length = MAX_LINE_LENGTH;
  size_t line_number = 0;
  size_t end_pos = 0;
  size_t begin_pos = 0;
  while(begin_pos < text.size()) {
    if (begin_pos + line_length < text.size()) {
      end_pos = begin_pos + line_length;
      while (text[end_pos] != ' ') {
        end_pos--;
      }
    } else {
      end_pos = text.size();
    }
    print_center_oneline(text.substr(begin_pos, end_pos - begin_pos), static_cast<int>(static_cast<size_t>(row) + line_number));
    begin_pos = end_pos;
    if (begin_pos < text.size() && text[begin_pos] == ' ') {
      begin_pos++;
    }
    line_number++;
  }

  return line_number;
}
