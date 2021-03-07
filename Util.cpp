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

bool DEBUG_Subtitile = false;
bool DEBUG_VLC_interface = false;
bool DEBUG_Handler = false;

using namespace std;

std::string execute(const std::string& program, const std::vector<std::string>& args){
    pid_t pid;
    int mypipe[2];
    string ret;

//    char** argv = new char*[args.size() + 1];
    char* argv[1024];
    int i = 0;
    argv[i] = new char[program.size() + 1];
    strcpy(argv[i], program.c_str());
    i++;
    for(auto& arg: args){
        argv[i] = new char[arg.size() + 1];
        strcpy(argv[i], arg.c_str());
        i++;
    }
    argv[i] = NULL;

    if(pipe(mypipe)){
        cerr << "Pipe failed: " << errno << endl;
        exit(3);
    }

    pid = fork();
    if(pid == (pid_t)0){ // Child
        close(mypipe[0]);
        setvbuf(stdout, NULL, _IONBF, 0);
        dup2(mypipe[1], STDOUT_FILENO);
        execvp(argv[0], argv);
        exit(4);
    }
    else{
        // Cleaning up
        close (mypipe[1]);
        for(size_t j = 0; j < i; j++){
            delete[] argv[j];
        }
//        delete[] argv;

        if(pid < (pid_t)0){ // Fork failed
            cerr << "Fork failed: " << errno << endl;
            close(mypipe[0]);
            exit(3);
        }
        else{ // Parent
            char buf[1024];
            ssize_t n_read;
            while((n_read = read(mypipe[0], buf, 1024)) > 0){
                ret += string(buf, n_read);
            }
            close(mypipe[0]);
            return ret;
        }
    }
}

