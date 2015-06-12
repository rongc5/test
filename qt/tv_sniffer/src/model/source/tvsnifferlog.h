#ifndef TVSNIFFERLOG_H
#define TVSNIFFERLOG_H

#include<log4cxx/logger.h>
#include<log4cxx/basicconfigurator.h>
#include<log4cxx/helpers/exception.h>
#include<log4cxx/propertyconfigurator.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

#define DEBUG "DEBUG"
#define INFO "INFO"
#define WARN "WARN"
#define ERROR "ERROR"
#define FALAT "FALAT"
#define DEFAULT "DEFAULT"

//LoggerPtr logger_name(string name);

//void log_path(string str);

//int LOG(LoggerPtr name,string Level,char *buf,const char* fmt, ...);
int LOG(string Level,char *buf,const char* fmt, ...);
void LOG_DEBUG(LoggerPtr name,char *buf);
void LOG_INFO(LoggerPtr name,char *buf);
void LOG_WARN(LoggerPtr name,char *buf);
void LOG_ERROR(LoggerPtr name,char *buf);
void LOG_FATAL(LoggerPtr name,char *buf);
void LOG_DEFAULT(LoggerPtr name,char *buf);



#endif
