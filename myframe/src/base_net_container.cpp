#include "common_epoll.h"
#include "base_net_container.h"
//#include "net_obj.h"


namespace MZFRAME {


    base_net_container::base_net_container()
    {
        _p_epoll = NULL;
    }
    base_net_container::~base_net_container()
    {
        if (_p_epoll != NULL)
            delete _p_epoll;
    }


    common_epoll * base_net_container::get_epoll()
    {
        return _p_epoll;
    }

    void base_net_container::init(const uint32_t epoll_size)
    {
        _p_epoll = new common_epoll();
        _p_epoll->init(epoll_size);
    }

}


