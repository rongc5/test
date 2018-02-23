#ifndef __BASE_RELOAD_H_
#define __BASE_RELOAD_H_

#include "base_def.h"


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
class reload_mgr 
{
    public:
        reload_mgr(reload_inf * T1, reload_inf *T2);
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
        reload_inf* current(); 

        void dump();

        int destroy();

    private:
        reload_inf * _objects[2];
        int16_t _curr;
};


#endif
