package log_helper

import (
	"os"
	"time"
	"fmt"
	"runtime"
	"sync"
	"path"
)


const(
	LOG_LEVEL_FATAL = iota
	LOG_LEVEL_WARNING
	LOG_LEVEL_NOTICE
	LOG_LEVEL_TRACE
	LOG_LEVEL_DEBUG
)

type logHelper struct{
	log_Level int
	file_max_size int
	mutex sync.Mutex
	log_name map[int]string
}

func NewLogHelper(conf *LogConf) *logHelper {

	logger := new(logHelper)

	if conf.log_Level < LOG_LEVEL_FATAL || conf.log_Level > LOG_LEVEL_DEBUG {
		fmt.Println("level is invailed")
	}

	logger.log_Level = conf.log_Level
	logger.file_max_size = conf.file_max_size
	logger.log_name = make(map[int]string)
	logger.log_name[LOG_LEVEL_FATAL] = logger.getLogFileName(conf.log_path, conf.prefix_file_name, LOG_LEVEL_FATAL)
	logger.log_name[LOG_LEVEL_DEBUG] = logger.getLogFileName(conf.log_path, conf.prefix_file_name,LOG_LEVEL_DEBUG)
	logger.log_name[LOG_LEVEL_WARNING] = logger.getLogFileName(conf.log_path, conf.prefix_file_name,LOG_LEVEL_WARNING)
	logger.log_name[LOG_LEVEL_NOTICE] = logger.getLogFileName(conf.log_path, conf.prefix_file_name,LOG_LEVEL_NOTICE)
	logger.log_name[LOG_LEVEL_TRACE] = logger.getLogFileName(conf.log_path, conf.prefix_file_name,LOG_LEVEL_TRACE)

	os.MkdirAll(conf.log_path, 0755)

	return logger
}

func (this *logHelper)checkToRenmae(level int)  {
	oldFileName := this.log_name[level]
	_,fileSize := GetFileByteSize(oldFileName)

	now := time.Now().Format("20181105180405")

	if this.file_max_size > 0 && int(fileSize) > this.file_max_size {

		newFileName := fmt.Sprintf("%s.%s", oldFileName, now)

		os.Rename(oldFileName,newFileName)
	}
}

func (this *logHelper)LogWrite(level int,logMsg string)  {
	this.mutex.Lock()
	defer this.mutex.Unlock()

	this.checkToRenmae(level)

	flag := this.getLoggerLevel(level)

	_,file,line,ok := runtime.Caller(0)
	if ok == false{
		fmt.Println("get line num fail")
	}
	name := path.Base(file)
	time := time.Now().Format("2018-11-05 18:04:05.000")

	fliename := this.log_name[level]

	fd,_:=os.OpenFile(fliename,os.O_WRONLY|os.O_CREATE | os.O_APPEND,0644)
	defer fd.Close()

	_,err := fd.WriteString(fmt.Sprintf("%s %s [%s:%d] %s\n",time,flag,name,line,logMsg))
	if err != nil {
		fmt.Println("write log fail",err.Error())
	}
}

func (this *logHelper)DEBUG(format string,args ...interface{})  {
	if LOG_LEVEL_DEBUG > this.log_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_DEBUG,fmt.Sprintf(format,args...))
}

func (this *logHelper)FATAL(format string,args ...interface{})  {
	if LOG_LEVEL_FATAL > this.log_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_FATAL,fmt.Sprintf(format,args...))
}

func (this *logHelper)WARNING(format string,args ...interface{})  {
	if LOG_LEVEL_WARNING > this.log_Level{
		return
	}
	this.LogWrite(LOG_LEVEL_WARNING,fmt.Sprintf(format,args...))
}

func (this *logHelper)NOTICE(format string,args ...interface{})  {
	if LOG_LEVEL_NOTICE > this.log_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_NOTICE,fmt.Sprintf(format,args...))
}

func (this *logHelper)TRACE(format string,args ...interface{})  {
	if LOG_LEVEL_TRACE > this.log_Level{
		return
	}
	this.LogWrite(LOG_LEVEL_TRACE,fmt.Sprintf(format,args...))
}

func (this *logHelper)getLoggerLevel(level int)string  {
	switch level{
	case LOG_LEVEL_DEBUG:
		return "[DEBUG]:"
	case LOG_LEVEL_FATAL:
		return "[FATAL]:"
	case LOG_LEVEL_WARNING:
		return "[WARNING]:"
	case LOG_LEVEL_NOTICE:
		return "[NOTICE]:"
	case LOG_LEVEL_TRACE:
		return "[TRACE]:"
	default:
		return ""
	}
}

func (this *logHelper)getLogFileName(log_path string, prefix_file_name string, level int)string {
	switch level{
	case LOG_LEVEL_DEBUG:
		return fmt.Sprintf("%s/%s.%s", log_path, prefix_file_name, "db")
	case LOG_LEVEL_FATAL:
		return fmt.Sprintf("%s/%s.%s", log_path, prefix_file_name, "ft")
	case LOG_LEVEL_WARNING:
		return fmt.Sprintf("%s/%s.%s", log_path, prefix_file_name, "wn")
	case LOG_LEVEL_NOTICE:
		return fmt.Sprintf("%s/%s.%s", log_path, prefix_file_name, "nt")
	case LOG_LEVEL_TRACE:
		return fmt.Sprintf("%s/%s.%s", log_path, prefix_file_name, "tr")
	default:
		return ""
	}
}

func IsFile(filename string) bool  {
	fhandler, err := os.Stat(filename);
	if(! (err == nil || os.IsExist(err)) ) {
		return false
	}else if (fhandler.IsDir()){
		return false
	}
	return true
}

func GetFileByteSize(filename string) (bool,int64) {
	if (! IsFile(filename)) {
		return false,0
	}
	fhandler, _ := os.Stat(filename);
	return true,fhandler.Size()
}


func GetProcName() string  {
	exec_file := fmt.Sprintf("/proc/%d/exe", os.Getpid())
	proc_name, err := os.Readlink(exec_file)
	if err != nil {
		return proc_name
	}

	proc_name = path.Base(proc_name)

	return proc_name
}
