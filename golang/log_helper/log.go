package log_helper

var logger = NewLogHelper(0)

func  LOG_DEBUG(format string,args ...interface{})  {
	logger.DEBUG(format,args...)
}

func LOG_FATAL(format string,args ...interface{})  {
	logger.FATAL(format,args...)
}

func LOG_WARNING(format string,args ...interface{})  {
	logger.WARNING(format,args...)
}

func LOG_NOTICE(format string,args ...interface{})  {
	logger.NOTICE(format,args...)
}

func LOG_TRACE(format string,args ...interface{})  {
	logger.TRACE(format,args...)
}
