#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__

#include "base_reload.h"

class ua_dict;
class proc_data:public reload_inf
{
    public:
        ~proc_data();
        
        static proc_data * instance();

        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    public:
        ua_dict * _ua_dict;

    private:
        static proc_data* _singleton;
};


#endif

