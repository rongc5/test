#ifndef __COMMON_CONFIG_H__
#define __COMMON_CONFIG_H__

class common_config
{
    public:
        common_config();
        ~common_config();

    private:
        map<string, map<string, string> > _config_map;
};


#endif
