#include "log_thread.h"
#include "base_singleton.h"
#include "base_def.h"
#include "common_util.h"
#include "common_def.h"
#include "common_exception.h"

log_thread::log_thread(log_conf & conf):_conf(conf)
{
	log_thread_init();
}

log_thread::~log_thread()
{
    if (_epoll_events != NULL)
        delete [] _epoll_events;

    _queue[_current].clear();
    _queue[1- _current].clear();
}

void log_thread::log_write(const char * filename, const char *format, ...)
{
    log_thread * thread = base_singleton<log_thread>::get_instance();
    if (!thread) {
        return;
    }

    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    std::shared_ptr<log_msg> lmsg(new log_msg());
    if (lmsg) {
        lmsg->_buf = new std::vector<char>(vsnprintf(NULL, 0, format, args1));
        va_end(args1);

        vsnprintf(lmsg->_buf->data(), lmsg->_buf->size(), format, args2);
        va_end(args2);
        
        lmsg->_fname.append(filename);
        thread->put_msg(lmsg);
    }
}

void log_thread::log_write(LogType type, const char *format, ...)
{

    log_thread * thread = base_singleton<log_thread>::get_instance();
    if (!thread) {
        return;
    }

    if (type > thread->get_log_conf().type){
        return;
    }

    char log_common_tmp[SIZE_LEN_64];
    get_timestr_millSecond(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT);
    uint32_t prefix_len = thread->_proc_name.size() + strlen(log_common_tmp) + SIZE_LEN_16;
    
    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    std::shared_ptr<log_msg> lmsg(new log_msg());
    if (lmsg) {
        lmsg->_buf = new std::vector<char>(prefix_len + vsnprintf(NULL, 0, format, args1));
        va_end(args1);

        uint32_t ret = snprintf(lmsg->_buf->data(), lmsg->_buf->size(), "[%s]:[%s] ", 
                thread->_proc_name.c_str(), log_common_tmp);
        vsnprintf(lmsg->_buf->data() + ret, lmsg->_buf->size() - ret, format, args2);
        va_end(args2);
        
        lmsg->_type = type;
        thread->put_msg(lmsg);
    }
}

void log_thread::put_msg(std::shared_ptr<log_msg> & p_msg)
{
    int idle = 1 - _current;
    std::lock_guard<std::mutex> lck (_mutex[idle]);
    _queue[idle].push_back(p_msg);
    write(_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));
}

void log_thread::handle_msg(std::shared_ptr<log_msg> & p_msg)
{
    FILE * fp = NULL;
    if (p_msg->_fname.empty())
    {
        check_to_renmae(_log_name[p_msg->_type]._name, _conf.file_max_size);
        fp = fopen(_log_name[p_msg->_type]._name, "a");
    }
    else
    {
        if (p_msg->_fname[0] == '/')
        {
            fp = fopen(p_msg->_fname.c_str(), "a");
        }
        else
        {
            std::string path;
            path.append(_conf.log_path);
            path.append("/");
            path.append(p_msg->_fname);
            fp = fopen(path.c_str(), "a");
        }
    }

    if (!fp){
        return;
    }   

    fprintf(fp, "%s\n", p_msg->_buf->data());
    fclose(fp);
}

void log_thread::get_file_name(LogType type, char dest[], size_t dest_len)
{
	if (!dest || !dest_len){
        return;
    }   

    switch (type)
    {   
        case LOGFATAL:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "ft");
            break;
        case LOGWARNING:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "wn");
            break;
        case LOGNOTICE:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "nt");
            break;
        case LOGTRACE:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "tc");
            break;
        case LOGDEBUG:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "db");
            break;
        default:
            break;
    }   

    return;
}

void log_thread::check_to_renmae(const char *filename, int max_size)
{
	  if (!filename){
        return;
    }

    char tmp[SIZE_LEN_64];
    char path[SIZE_LEN_128];
    struct stat statBuf;
    stat(filename, &statBuf);

    if (max_size && statBuf.st_size >= max_size){
        get_timestr(tmp, sizeof(tmp), "%Y%m%d%H%M%S");
        snprintf(path, sizeof(path), "%s.%s", filename, tmp);
        rename(filename, path);
    }
}

void log_thread::log_thread_init()
{
	 if (_conf.log_path[0] != '0') {
            char buf[SIZE_LEN_256];
            snprintf(buf, sizeof(buf), "mkdir -p %s", _conf.log_path);
            system(buf);         
     }


	int i = LOGFATAL;
	for (; i<LOGSIZE; i++) {
		get_file_name((LogType)i, _log_name[i]._name, sizeof(_log_name[i]._name));
	}

    _epoll_fd = epoll_create(DAFAULT_EPOLL_SIZE);
    if (_epoll_fd == -1)
    {       
        THROW_COMMON_EXCEPT("epoll_create fail " << strerror(errno));
    }

    _epoll_size = SIZE_LEN_16;
    _epoll_events = new epoll_event[_epoll_size];

    int fd[2];
    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
    if (ret < 0) {
        return;
    }

    _channelid = fd[1];
    _current = 0;

    struct epoll_event tmpEvent;
    memset(&tmpEvent, 0, sizeof(epoll_event));
    tmpEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    tmpEvent.data.fd = fd[0];
    ret = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd[0], &tmpEvent);
    if (ret != 0) {
        THROW_COMMON_EXCEPT("add to epoll fail " << strerror(errno));
    }
    
    char tmp_buff[SIZE_LEN_128];
    get_proc_name(tmp_buff, sizeof(tmp_buff));
    _proc_name.append(tmp_buff);
}

void * log_thread::run()
{
    while (get_run_flag()) {
        obj_process();
    }

    return NULL;
}

int log_thread::RECV(int fd, void *buf, size_t len)
{
    int ret = recv(fd, buf, len, MSG_DONTWAIT);

    if (ret == 0)
    {
        THROW_COMMON_EXCEPT("the client close the socket(" << fd << ")");
    }

    else if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            THROW_COMMON_EXCEPT("this socket occur fatal error " << strerror(errno));
        }

        ret = 0;
    }

    return ret;
}

size_t log_thread::process_recv_buf(const char *buf, const size_t len)
{
    size_t k = len /sizeof(CHANNEL_MSG_TAG);
    size_t i = 0, m = 0;
    bool flag = true;
    std::deque<std::shared_ptr<log_msg> >::iterator it;
    for (; m < 2; m++) {
        {
            std::lock_guard<std::mutex> lck (_mutex[_current]);
            for (it = _queue[_current].begin(); it != _queue[_current].end() && i < k;) {
                handle_msg(*it);
                it = _queue[_current].erase(it);
                i++;
            }

            if (_queue[_current].begin() == _queue[_current].end()){
                _current = 1 - _current;
                flag = false;
            }
        }

        if (flag){
            break;
        }
    }
    k =  i * sizeof(CHANNEL_MSG_TAG);

    return k;
}

void log_thread::set_type(LogType type)
{
    _conf.type = type;
}


void log_thread::obj_process()
{
    int  nfds = ::epoll_wait(_epoll_fd, _epoll_events, _epoll_size, DEFAULT_EPOLL_WAITE);
    if (-1 == nfds){
        return;
    }

    char buf[SIZE_LEN_2048];
    ssize_t ret = 0;
    size_t _recv_buf_len = _recv_buf.length();
    
    for (int i =0; i < nfds; i++) {
        if (_epoll_events[i].events & EPOLLIN) {
            ret = RECV(_epoll_events[i].data.fd, buf, sizeof(buf));
            if (ret) {
                _recv_buf.append(buf, ret);  
                _recv_buf_len += ret;
            }
        }
    }
    
    if (_recv_buf_len > 0) {
    	size_t p_ret = process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);

      if (p_ret > 0 && p_ret <= _recv_buf_len)
        {
            _recv_buf.erase(0, p_ret);
        }
    }
}

const log_conf & log_thread::get_log_conf()
{
    return _conf;
} 
