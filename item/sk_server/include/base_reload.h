#ifndef __BASE_RELOAD_H_
#define __BASE_RELOAD_H_

#include "base_def.h"
#include "log_helper.h"


/**
 * @brief 需要提供Load和Reload功能的类型，需要继承此接口。
 * 用ReloadMgr管理时需要调用Load和Reload方法
 * */
class reload_inf {
public:
    virtual ~reload_inf() {}
    virtual int init(const char * path) = 0;
    virtual int load() = 0;
    virtual int reload() = 0;
    virtual int dump() = 0;
    virtual int destroy() = 0;
};

/**
 *@brief 重载管理，判断是否需要重载
 */
template<typename ClassT>
class reload_mgr {
public:
    reload_mgr() {}
    ~reload_mgr() {}

    int init(const char * path){
        _objects[0].init(path);
        _objects[1].init(path);
        set_path(path);
        return 0;
    }

    /**
     *@brief 加载配置
     */
    int load() {
        if( _objects[0].load() == 0 ) {
            struct stat st;
            stat(_fullpath, &st);
            last_load_ = st.st_mtime;
            _curr = 0;
            return 0;
        }

        LOG_WARNING("load data failed, %s", _fullpath);
        return -1;
    }

    /**
     * @brief 重载配置
     */
     //检查当前配置文件的修改时间是否和上次记录的时间一致， 不一致重新加载, 并更新记录的文件修改时间，以及curr_ 值
    int reload() {
        struct stat st;
        if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load) {
            LOG_DEBUG("reload thread check time. file time[%lu],last load time[%lu]\n",st.st_mtime,_last_load);
            if ( _objects[1 - _curr].reload() == 0 ) {
                _last_load = st.st_mtime;
                _curr = 1 - _curr;
                return 0;
            } else {
                LOG_WARNING("reload data failed, %s,%d", _fullpath, _curr);
                return -1;
            }
        }

        return 0;
    }
	//因为每一个ReloadMgr 模板对象， 里边具有两个版本类成员， 其中只有一个是当前可用的， curr_
	//就是记录当前可用的模板类对象， 在Load 和 Reload 中均会更新改标志值, 改方法返回当前有效的
	//模板类对象
    ClassT* current() {
        if( _curr == 0 || _curr == 1){
            return &(_objects[_curr]);
        }

        return NULL;
    }

    void dump(){
        ClassT* obj = current();
        obj->dump(path,fname);
    }

    int destroy(){
    	_objects[0].destroy();
		_objects[1].destroy();
    	return 0;
    }
private:
    /**设置路径和文件名*/
    void set_path (const char* path) {
        snprintf(_fullpath, sizeof(_fullpath), "%s", path);
    }
private:
    char _fullpath[SIZE_LEN_512];
    ClassT _objects[2];
    int16_t _curr;
    time_t _last_load;
};


#endif
