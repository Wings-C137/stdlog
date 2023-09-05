//
// Created by Wings on 8/17/23.
//
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <mutex>
#include <semaphore.h>

#ifndef _STD_LOG_
#define _STD_LOG_

class StdLog {
private:
    std::string fullname;
    std::string path = "./";
    struct tm *ptm = nullptr;
    FILE * logFile = nullptr;
    time_t secondsSince1970 = 0;
    char* filename = nullptr;
    sem_t sem;
    std::mutex mtx;
    uint8_t debugEnbaled = 0;
    static inline char* strCrt(char* timec)
    {
        timec[strlen(timec)-1]=0;
        return timec;
    }
public:
    StdLog()
    {
        ;
    }
    ~StdLog()
    {
        fclose(logFile);
    }
    int Open(const char* pathAndfilename, const char* openType, uint8_t debugFlag)
    {
        debugEnbaled = debugFlag;
        if (sem_init(&sem, 0, 1)) {
            fprintf(stderr, "Cannot init sem!\n");
            return -1;
        }
        filename = strrchr((char*)pathAndfilename, '/');
        filename = filename ? 1 + filename : (char*)pathAndfilename;
        memcpy((void*)&path[0], pathAndfilename, strlen(pathAndfilename)-strlen(filename));
        time(&secondsSince1970);
        ptm = localtime(&secondsSince1970);
        sprintf(&fullname[0], "%s%d-%d-%d_%s", &path[0], 1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday, filename);
        //printf("%s\n",&fullname[0]);
        logFile = fopen(&fullname[0], openType);
        if (logFile == nullptr)
        {
            return -1;
        }
        return 0;
    }
    int debug(const char *logs, ...)
    {
        if (debugEnbaled == 0) {
            return 0;
        }
        sem_wait(&sem);
        mtx.lock();
        std::string logs_tmp;
        va_list args;
        int i;
        va_start(args, logs);
        i = vsprintf(&logs_tmp[0], logs,args);
        va_end(args);
        time(&secondsSince1970);
        fprintf(logFile, "[%s] [Debug] %s", strCrt(ctime(&secondsSince1970)), &logs_tmp[0]);
        fflush(logFile);
        mtx.unlock();
        sem_post(&sem);
        return i;
    }
    int info(const char *logs, ...)
    {
        sem_wait(&sem);
        mtx.lock();
        std::string logs_tmp;
        va_list args;
        int i;
        va_start(args, logs);
        i = vsprintf(&logs_tmp[0], logs,args);
        va_end(args);
        time(&secondsSince1970);
        fprintf(logFile, "[%s] [Info] %s", strCrt(ctime(&secondsSince1970)), &logs_tmp[0]);
        fflush(logFile);
        mtx.unlock();
        sem_post(&sem);
        return i;
    }
    int warn(const char *logs, ...)
    {
        sem_wait(&sem);
        mtx.lock();
        std::string logs_tmp;
        va_list args;
        int i;
        va_start(args, logs);
        i = vsprintf(&logs_tmp[0], logs,args);
        va_end(args);
        time(&secondsSince1970);
        fprintf(logFile, "[%s] [Warning] %s", strCrt(ctime(&secondsSince1970)), &logs_tmp[0]);
        fflush(logFile);
        mtx.unlock();
        sem_post(&sem);
        return i;
    }
    int err(const char *logs, ...)
    {
        sem_wait(&sem);
        mtx.lock();
        std::string logs_tmp;
        va_list args;
        int i;
        va_start(args, logs);
        i = vsprintf(&logs_tmp[0], logs,args);
        va_end(args);
        time(&secondsSince1970);
        fprintf(logFile, "[%s] [Error] %s", strCrt(ctime(&secondsSince1970)), &logs_tmp[0]);
        fflush(logFile);
        mtx.unlock();
        sem_post(&sem);
        return i;
    }
};

#endif //_STD_LOG_
