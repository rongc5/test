#ifndef __BASE_RELOAD_H_
#define __BASE_RELOAD_H_

#include "base_def.h"
#include "log_helper.h"


/**
 * @brief 需要提供Load和Reload功能的类型，需要继承此接口。
 * 用ReloadMgr管理时需要调用Load和Reload方法
 * */
class reload_inf 
{
    public:
        virtual ~reload_inf() {}
        virtual int load() = 0;
        virtual int reload() = 0;
        virtual bool need_reload()=0;
        virtual int dump() = 0;
        virtual int destroy() = 0;
};

/**
 *@brief 重载管理，判断是否需要重载
 */
template<typename T>
class reload_mgr 
{
    public:
        reload_mgr(T * T1, T *T2);
        ~reload_mgr();

        /**
         *@brief 加载配置
         */
        int load(); 

        /**
         * @brief 重载配置
         */
        //检查当前配置文件的修改时间是否和上次记录的时间一致， 不一致重新加载, 并更新记录的文件修改时间，以及curr_ 值
        int reload(); 
        //因为每一个ReloadMgr 模板对象， 里边具有两个版本类成员， 其中只有一个是当前可用的， curr_
        //就是记录当前可用的模板类对象， 在Load 和 Reload 中均会更新改标志值, 改方法返回当前有效的
        //模板类对象
        T* current(); 

        bool need_reload();

        void dump();

        int destroy();

        T* idle(); 

    private:
        T * _objects[2];
        int16_t _curr;
};

template<typename T>
reload_mgr<T>::reload_mgr(T * T1, T *T2) 
{
    _objects[0] = T1;
    _objects[1] = T2;
    _curr = 0;
}

template<typename T>
reload_mgr<T>::~reload_mgr() 
{
    if (_objects[0])
    {
        _objects[0]->destroy();
        delete _objects[0];
        _objects[0] = NULL;
    }

    if (_objects[1])
    {
        _objects[1]->destroy();
        delete _objects[1];
        _objects[1] = NULL;
    }
}

/**
 *@brief 加载配置
 */
template<typename T>
int reload_mgr<T>::load() 
{
    if( _objects[0]->load() == 0 ) 
    {
        _curr = 0;
        return 0;
    }

    return -1;
}

template<typename T>
bool reload_mgr<T>::need_reload()
{
    return current()->need_reload();
}


/**
 * @brief 重载配置
 */
//检查当前配置文件的修改时间是否和上次记录的时间一致， 不一致重新加载, 并更新记录的文件修改时间，以及curr_ 值
template<typename T>
int reload_mgr<T>::reload() 
{
    if ( _objects[1 - _curr]->reload() == 0 ) 
    {
        _curr = 1 - _curr;
        return 0;
    } else 
    {
        LOG_WARNING("reload data failed,%d", _curr);
        return -1;
    }

    return 0;
}
//因为每一个ReloadMgr 模板对象， 里边具有两个版本类成员， 其中只有一个是当前可用的， curr_
//就是记录当前可用的模板类对象， 在Load 和 Reload 中均会更新改标志值, 改方法返回当前有效的
//模板类对象
template<typename T>
T* reload_mgr<T>::current() {
    if( _curr == 0 || _curr == 1){
        return (_objects[_curr]);
    }

    return NULL;
}

template<typename T>
void reload_mgr<T>::dump()
{
    reload_inf* obj = current();
    obj->dump();
}

template<typename T>
int reload_mgr<T>::destroy()
{
    if (_objects[0])
        _objects[0]->destroy();

    if (_objects[1])
        _objects[1]->destroy();

    return 0;
}

template<typename T>
T* reload_mgr<T>::idle()
{
    if( _curr == 0 || _curr == 1){
        return (_objects[1 - _curr]);
    }

    return NULL;
}

#endif
