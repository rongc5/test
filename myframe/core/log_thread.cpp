#include "log_thread.h"
#include "base_singleton.h"
#include "base_def.h"
#include "common_util.h"
#include "common_def.h"
#include "common_exception.h"

log_thread::log_thread()
{
}

log_thread::~log_thread()
{
    if (_epoll_events != NULL)
        delete [] _epoll_events;

    _queue[_current].clear();
    _queue[1- _current].clear();
}

void log_thread::init(const char * path)
{
    log_thread * thread = base_singleton<log_thread>::get_instance_ex();
    thread->log_thread_init(path);

    thread->start();
}


void log_thread::log_write(const char * filename, const char *format, ...)
{
    log_thread * thread = base_singleton<log_thread>::get_instance_ex();
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

bool log_thread::check_type(LogType type)
{
     log_conf * conf = _rlog_conf->current();
    if (conf && type <= conf->type)
    {
        return true;
    }

    return false;
}

void log_thread::log_write(LogType type, const char *format, ...)
{

    log_thread * thread = base_singleton<log_thread>::get_instance_ex();
    if (!thread) {
        return;
    }

    if (!thread->check_type(type)){
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
    log_conf * conf = _rlog_conf->current();

    std::lock_guard<std::mutex> lck (_mutex);
    p_msg->_logid = get_logid();

    if (conf->model == LOGLOCAL) {
        handle_msg(p_msg); 
    } else {
        int idle = 1 - _current;
        _queue[idle].push_back(p_msg);
    }

    //write(_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));
    send(_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG), MSG_DONTWAIT);
}

void log_thread::handle_msg(std::shared_ptr<log_msg> & p_msg)
{
    FILE * fp = NULL;
    
    log_conf * conf = _rlog_conf->current();

    if (p_msg->_fname.empty() && conf)
    {
        check_to_renmae(conf->_log_name[p_msg->_type].c_str(), conf->file_max_size);
        fp = fopen(conf->_log_name[p_msg->_type].c_str(), "a");
        if (!fp){
            return;
        }   

        fprintf(fp, "[log_id:%" PRIu64 "]%s\n", p_msg->_logid, p_msg->_buf->data());
    }
    else
    {
        fp = fopen(p_msg->_fname.c_str(), "a");
        if (!fp){
            return;
        }   

        fprintf(fp, "%s\n", p_msg->_buf->data());
    }


    fclose(fp);
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

void log_thread::log_thread_init(const char * path)
{
    if (!path) {
        return ;
    }

    _rlog_conf = new reload_mgr<log_conf>(new log_conf(path), new log_conf(path));
    log_conf_init();

    _epoll_fd = epoll_create(DAFAULT_EPOLL_SIZE);
    if (_epoll_fd == -1)
    {       
        THROW_COMMON_EXCEPT("epoll_create fail " << strError(errno).c_str());
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
        THROW_COMMON_EXCEPT("add to epoll fail " << strError(errno).c_str());
    }
    
    char tmp_buff[SIZE_LEN_128];
    get_proc_name(tmp_buff, sizeof(tmp_buff));
    _proc_name.append(tmp_buff);
}

void log_thread::log_conf_init()
{
    if (_rlog_conf) {
        _rlog_conf->reload();
    }

    log_conf * conf = _rlog_conf->current();

	 if (!conf->log_path.empty()) {
            char buf[SIZE_LEN_512];
            snprintf(buf, sizeof(buf), "mkdir -p %s", conf->log_path.c_str());
            system(buf);         
     }
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
            THROW_COMMON_EXCEPT("this socket occur fatal error " << strError(errno).c_str());
        }

        ret = 0;
    }

    return ret;
}

size_t log_thread::process_recv_buf(const char *buf, const size_t len)
{
    //size_t k = len /sizeof(CHANNEL_MSG_TAG);
    //size_t i = 0;
    std::deque<std::shared_ptr<log_msg> >::iterator it;

    {
        std::deque<std::shared_ptr<log_msg> > & queue = _queue[_current];
        if (queue.begin() == queue.end()){
            std::lock_guard<std::mutex> lck (_mutex);
            _current = 1 - _current;
        }
    }

    //for (it = _queue[_current].begin(); it != _queue[_current].end() && i < k;) {
    std::deque<std::shared_ptr<log_msg> > & queue = _queue[_current];
    for (it = queue.begin(); it != queue.end();) {
        handle_msg(*it);
        it = queue.erase(it);
        //i++;
    }


    //k =  i * sizeof(CHANNEL_MSG_TAG);

    //return k;
    return len;
}

void log_thread::obj_process()
{
    int  nfds = ::epoll_wait(_epoll_fd, _epoll_events, _epoll_size, DEFAULT_EPOLL_WAITE);
    if (-1 == nfds){
        //PDEBUG("errno: %s\n", strError(errno).c_str());
        return;
    }

    char buf[SIZE_LEN_2048];
    ssize_t ret = 0;
    //size_t _recv_buf_len = _recv_buf.length();
    
    for (int i =0; i < nfds; i++) {
        if (_epoll_events[i].events & EPOLLIN) {
            ret = RECV(_epoll_events[i].data.fd, buf, sizeof(buf));
            //if (ret) {
                //_recv_buf.append(buf, ret);  
                //_recv_buf_len += ret;
            //}

            //if (_recv_buf_len > 0) {
                //size_t p_ret = process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);

                //if (p_ret > 0 && p_ret <= _recv_buf_len)
                //{
                    //_recv_buf.erase(0, p_ret);
                //}
            //}
        }
    }
    
    process_recv_buf(buf, ret);

    if (_rlog_conf->need_reload()) {
        log_conf_init();
    }
}

uint64_t log_thread::get_logid()
{
    if (log_global_id < LOG_ID_MIN)
    {
        log_global_id = LOG_ID_MIN;
    }

    return log_global_id++;
}

uint64_t log_thread::log_global_id = 0;
