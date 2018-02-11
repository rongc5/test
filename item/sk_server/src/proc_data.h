#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__


class ua_dict;
class proc_data
{
    public:
        ~proc_data();
        
        static proc_data * instance();

    public:
        ua_dict * _ua_dict;

    private:
        static proc_data* _singleton;
};


#endif

