#include"tvsnifferlog.h"

#if 0
LoggerPtr logger_name(string name){
	LoggerPtr logger_root(Logger::getLogger(name));
	return logger_root;
}
void log_path(string str){
	PropertyConfigurator::configure(str);
}

int LOG(LoggerPtr name,string Level,char *buf,const char* fmt, ...){
#endif
int LOG(string Level,char *buf,const char* fmt, ...){
	log4cxx::PropertyConfigurator::configure("/home/haier/tv_sniffer/src/model/source/log4cxx.properties");
	LoggerPtr name(Logger::getLogger("world"));
	int strlen;
	va_list ap;
	va_start(ap,fmt);
	strlen = vsprintf(buf,fmt,ap);
	va_end(ap);
	cout << "Level: " << Level << endl;
	if(Level == "DEBUG")
	{
		LOG_DEBUG(name,buf);
	}
	else if(Level ==  "INFO")
	{
		LOG_INFO(name,buf);
	}
	else if(Level == "WARN")
	{
		LOG_WARN(name,buf);
	}
	else if(Level == "ERROR")
	{
		LOG_ERROR(name,buf);
	}
	else if(Level == "FATAL")
	{
		LOG_FATAL(name,buf);
	}
	else
	{
		LOG_DEFAULT(name,buf);
	}

	return strlen;
}


void LOG_DEBUG(LoggerPtr name,char *buf){
        LOG4CXX_DEBUG(name,buf);
}
void LOG_INFO(LoggerPtr name,char *buf){
        LOG4CXX_INFO(name,buf);
}
void LOG_WARN(LoggerPtr name,char *buf){
        LOG4CXX_WARN(name,buf);
}
void LOG_ERROR(LoggerPtr name,char *buf){
        LOG4CXX_ERROR(name,buf);
}
void LOG_FATAL(LoggerPtr name,char *buf){
        LOG4CXX_FATAL(name,buf);
}
void LOG_DEFAULT(LoggerPtr name,char *buf){
        LOG4CXX_DEBUG(name,buf);
}
