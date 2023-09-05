#include <iostream>
#include <csignal>
#include <cstring>
#include <getopt.h>
#include <fstream>
#include <thread>
#include <unistd.h>
#include "../universal/stdlog.h"
using namespace std;
//Function usages!
static void usage(char *programName)
{
    fprintf(stderr,
            "usage: %s [options]\n"
            " -h help    print usage\n",
            programName);
}
//Interrupt handle function!
void interruptHandler(int nSigal)
{
    std::cout << "receved terminal signal" << std::endl;
}
//Signal handle function!
void signalHandler()
{
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);
}

//Main function!
int main(int argc, char* argv[]) {
    //handle user input options!
    int opt = 0;
    char* programName = nullptr;
    programName = strrchr(argv[0], '/');
    programName = programName ? 1 + programName : argv[0];
    while ((opt = getopt(argc, argv, "d:h")) != EOF)
    {
        switch (opt)
        {
            case 'h':
                usage(programName);
                return 0;
            default:
                usage(programName);
                return -1;
        }
    }
    //logger init!
    StdLog syslog;
    syslog.Open("/tmp/stdlog_demo.log", "w+", 0);
    syslog.info("System initing!\n");
    //Signal handle function!
    signalHandler();
    syslog.info("Signal handler register complete!\n");
    return 0;
}
