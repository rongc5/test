#include "base_reload.h"
#include "log_helper.h"


reload_mgr::reload_mgr(reload_inf * T1, reload_inf *T2) 
{
    _objects[0] = T1;
    _objects[1] = T2;
    _curr = 0;
}

reload_mgr::~reload_mgr() 
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
int reload_mgr::load() 
{
    if( _objects[0]->load() == 0 ) 
    {
        _curr = 0;
        return 0;
    }

    return -1;
}

/**
 * @brief 重载配置
 */
//检查当前配置文件的修改时间是否和上次记录的时间一致， 不一致重新加载, 并更新记录的文件修改时间，以及curr_ 值
int reload_mgr::reload() 
{
    if (current()->need_reload()) 
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
    }

    return 0;
}
//因为每一个ReloadMgr 模板对象， 里边具有两个版本类成员， 其中只有一个是当前可用的， curr_
//就是记录当前可用的模板类对象， 在Load 和 Reload 中均会更新改标志值, 改方法返回当前有效的
//模板类对象
reload_inf* reload_mgr::current() {
    if( _curr == 0 || _curr == 1){
        return (_objects[_curr]);
    }

    return NULL;
}

void reload_mgr::dump()
{
    reload_inf* obj = current();
    obj->dump();
}

int reload_mgr::destroy()
{
    if (_objects[0])
        _objects[0]->destroy();

    if (_objects[1])
        _objects[1]->destroy();

    return 0;
}


