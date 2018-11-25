package log_helper

import (
	"os"
)

var logger * logHelper

var default_file_max_size int = 50*1024*1024

type LogConf struct {
	log_path string
	prefix_file_name string
	log_Level int
	file_max_size int
}

func NewLogConf() *LogConf  {
	conf := new(LogConf)
	log_path, err := os.Getwd()
	if err != nil {
		log_path = "./"
	}

	conf.log_path = log_path

	conf.prefix_file_name = GetProcName()
	if conf.prefix_file_name == "" {
		conf.prefix_file_name = "test"
	}

	conf.log_Level = LOG_LEVEL_DEBUG
	conf.file_max_size = default_file_max_size

	return conf
}

func LOG_INIT(conf *LogConf) {
	logger = NewLogHelper(conf)
}

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
