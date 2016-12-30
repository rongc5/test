 #include "net_obj.h"
 #include "common_epoll.h"
 #include "base_net_container.h"
 void NET_OBJ::get_peer_addr(sockaddr_in &addr)
 {
     addr = _peer_addr;
 }
  
 void base_net_obj::add_event(const int32_t event)
 {
     _epoll_event = _epoll_event | event;
     _p_epoll->mod_from_epoll(this);
 }
 
 void base_net_obj::del_event(const int32_t event)
 {
     _epoll_event = _epoll_event & (~event);
     _p_epoll->mod_from_epoll(this);
 }
 
 void base_net_obj::set_net_container(base_net_container *p_net_container)
 {
 	_p_net_container = p_net_container;
 	_id_str = p_net_container->get_id_str();
	_p_epoll = p_net_container->get_epoll();
	_p_epoll->add_to_epoll(this);
}
