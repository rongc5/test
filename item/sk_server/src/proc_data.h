#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__

#include "base_reload.h"
#include "sk_conf.h"

class ua_dict;
class strategy_dict;
class proc_data:public reload_inf
{
    public:
        ~proc_data();
        
        static proc_data * instance();

        int init(sk_conf *conf);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    public:
        reload_mgr<ua_dict> * _ua_dict;
        reload_mgr<strategy_dict> * _strategy_dict;

    private:
        static proc_data* _singleton;
        sk_conf * _conf;
};


#endif

