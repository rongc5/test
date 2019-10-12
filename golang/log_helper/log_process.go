package log_helper

import (
	"os"
	"time"
	"fmt"
	"errors"
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
	log_path string
	prefix_file_name string

	log_Level int
	file_max_size int
	m_mu sync.Mutex

	log_name map[int]string
}

type LogConf struct {
	log_path string
	prefix_file_name string
	log_Level int
	file_max_size int
}

func defaultNew() *logHelper  {
	return &logHelper{
		log_path:*logD,
		prefix_file_name:"",
		m_FileHandle:nil,
		log_Level:0,
		log_name:make(map[int]string),
		m_Depth:defaultDepth,
		m_MaxLogFileNum:(*maxFileNum),
		m_MaxLogDataNum:(*maxFileCap),
	}
}

func NewLogHelper(conf *LogConf) *logHelper {

	logger := defaultNew()

	if level < LOG_LEVEL_FATAL || level > LOG_LEVEL_DEBUG {
		fmt.Println("level is invailed")
	}

	logger.log_Level = conf.level
	logger.
	logger.log_name[LOG_LEVEL_FATAL] = logger.getLogFileName(LOG_LEVEL_FATAL)
	logger.log_name[LOG_LEVEL_DEBUG] = logger.getLogFileName(LOG_LEVEL_DEBUG)
	logger.log_name[LOG_LEVEL_WARNING] = logger.getLogFileName(LOG_LEVEL_WARNING)
	logger.log_name[LOG_LEVEL_NOTICE] = logger.getLogFileName(LOG_LEVEL_NOTICE)
	logger.log_name[LOG_LEVEL_TRACE] = logger.getLogFileName(LOG_LEVEL_TRACE)




	err := logger.obtainLofFile()
	if err != nil{
		fmt.Println(ObtainFileFail)
	}
	return logger
}

func (this *logHelper)checkToRenmae(level int)  {
	oldFileName := this.GetLoggerFileName(level)
	_,fileSize := GetFileByteSize(oldFileName)

	now := time.Now().Format("20181105180405")

	if this.file_max_size > 0 && int(fileSize) > this.file_max_size {

		newFileName := fmt.Sprintf("%s.%s", oldFileName, now)

		os.Rename(oldFileName,newFileName)
	}
}

func (this *logHelper)logWrite(level int,logMsg string)  {
	this.m_mu.Lock()
	defer this.m_mu.Unlock()
	//超时或者超过大小

	this.checkToRenmae(level)

	flag := GetLoggerLevel(level)

	_,file,line,ok := runtime.Caller(0)
	if ok == false{
		fmt.Println(GetLineNumFail)
	}
	name := path.Base(file)
	time := time.Now().Format("2018-11-05 18:04:05.000")
	_,err := Write(this.m_FileHandle,fmt.Sprintf("%s %s [%s:%d] %s\n",time,flag,name,line,logMsg))
	if err != nil {
		fmt.Println(WriteLogInfoFail,err.Error())
	}
}

func (this *logHelper)DEBUG(format string,args ...interface{})  {
	if LOG_LEVEL_DEBUG < this.m_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_DEBUG,fmt.Sprintf(format,args...))
}

func (this *logHelper)FATAL(format string,args ...interface{})  {
	if LOG_LEVEL_FATAL < this.m_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_FATAL,fmt.Sprintf(format,args...))
}

func (this *logHelper)WARNING(format string,args ...interface{})  {
	if LOG_LEVEL_WARNING < this.m_Level{
		return
	}
	this.LogWrite(LOG_LEVEL_WARNING,fmt.Sprintf(format,args...))
}

func (this *logHelper)NOTICE(format string,args ...interface{})  {
	if LOG_LEVEL_NOTICE < this.m_Level {
		return
	}
	this.LogWrite(LOG_LEVEL_NOTICE,fmt.Sprintf(format,args...))
}

func (this *logHelper)TRACE(format string,args ...interface{})  {
	if LOG_LEVEL_TRACE < this.m_Level{
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

func (this *logHelper)getLogFileName(level int)string {
	switch level{
	case LOG_LEVEL_DEBUG:
		return fmt.Sprintf("%s/%s.%s", this.log_path, this.prefix_file_name, "db")
	case LOG_LEVEL_FATAL:
		return fmt.Sprintf("%s/%s.%s", this.log_path, this.prefix_file_name, "ft")
	case LOG_LEVEL_WARNING:
		return fmt.Sprintf("%s/%s.%s", this.log_path, this.prefix_file_name, "wn")
	case LOG_LEVEL_NOTICE:
		return fmt.Sprintf("%s/%s.%s", this.log_path, this.prefix_file_name, "nt")
	case LOG_LEVEL_TRACE:
		return fmt.Sprintf("%s/%s.%s", this.log_path, this.prefix_file_name, "tr")
	default:
		return ""
	}
}
func (this *stLogger)obtainLofFile() error  {
	fileDir := this.m_FileDir
	//文件夹为空
	if fileDir == ""{
		fmt.Println(ArgsInvaild)
		os.Exit(1)
	}

	//时间文件夹
	destFilePath := fmt.Sprintf("%s%d%d%d",logDir,time.Now().Year(),time.Now().Month(),
		time.Now().Day())
	flag,err := IsExist(destFilePath)
	if err != nil{
		fmt.Println(ArgsInvaild)
	}
	if !flag {
		os.MkdirAll(destFilePath,os.ModeDir)
	}
	//文件夹存在,直接以创建的方式打开文件
	destFilePath =destFilePath+"/"
	logFilePath:=fmt.Sprintf("%s%s_%d_%d%d%d%s",destFilePath,"log",1,time.Now().Year(),time.Now().Month(),
		time.Now().Day(),".log")

	_,fileSize := GetFileByteSize(logFilePath)
	if flag && fileSize > int64(this.m_MaxLogDataNum){
		this.RenameTooBigFile()
	}
	fileHandle,err := os.OpenFile(logFilePath,os.O_CREATE | os.O_APPEND | os.O_RDWR, os.ModePerm)
	if err != nil{
		fmt.Println( OpenFileFail,err.Error())
	}

	this.m_FileHandle = fileHandle
	this.m_FileName = logFilePath
	//设置下次创建文件的时间
	time.Unix(time.Now().Unix(),0).Format("2006-01-02")
	nextDay := time.Unix(time.Now().Unix() + (24 * 3600),0)
	nextDay = time.Date(nextDay.Year(),nextDay.Month(),nextDay.Day(),0,0,0,
		0,nextDay.Location())
	this.m_nexDay = nextDay


	return nil
}