#ifndef __COMMON_OBJ_CONTAINER_H__
#define __COMMON_OBJ_CONTAINER_H__

#include "common_def.h"
#include "common_epoll.h"
#include "base_net_container.h"


class common_obj_container:public base_net_container
{
    public:
        common_obj_container()
        {
            base_net_container::init();
        }

        virtual ~common_obj_container()
        {
        }

        void push_net_obj(base_net_obj *p_obj)
        {
            _obj_net_map.insert(make_pair(p_obj->get_id(), p_obj));
        }
        virtual base_net_obj* find(const ObjId * obj_id)
        {
            base_net_obj *p_obj = NULL;
            map<ObjId, base_net_obj*>::iterator itr =  _obj_net_map.find(*obj_id);
            if (itr != _obj_net_map.end())
                p_obj = itr->second;
            return p_obj;
        }

        virtual bool erase(ObjId *obj_id)
        {
            if (!obj_id){
                return false;
            }
            bool ret = false;
            base_net_obj *p_obj = NULL;
            map<ObjId, base_net_obj*>::iterator itr =  _obj_net_map.find(*obj_id);
            if (itr != _obj_net_map.end())
            {
                p_obj = itr->second;
                if (p_obj != NULL)
                {
                    _obj_net_map.erase(p_obj->get_id());
                    p_obj->destroy();	    
                }

                ret = true;
            }
            return ret;
        }

        virtual void put_msg(pass_msg* p_msg)
        {
            base_net_obj * net_obj = find(&p_msg->_obj_id);
            if (!net_obj) {
                REC_OBJ<pass_msg> rec(p_msg);
                return;
            }

            net_obj->process_s(p_msg);
        }

        virtual void obj_process()
        {   
            map<ObjId, base_net_obj*> exp_list;
            for (map<ObjId, base_net_obj*>::iterator tmp_itr = _obj_net_map.begin();tmp_itr != _obj_net_map.end(); )
            {
                int32_t ret = 0;
                try
                {
                    ret = tmp_itr->second->real_net_process();            
                }
                catch(CMyCommonException &e)
                {
                    exp_list.insert(make_pair(tmp_itr->first,tmp_itr->second));	
                }
                catch(std::exception &e)
                {
                    exp_list.insert(make_pair(tmp_itr->first,tmp_itr->second));	
                }

                if (ret == -1) //空的对象删除之
                {
                    map<ObjId, base_net_obj*>::iterator aa_itr = tmp_itr;
                    ++tmp_itr;
                    _obj_net_map.erase(aa_itr);
                }
                else
                {
                    ++tmp_itr;
                }
            }


            _p_epoll->epoll_wait(exp_list);
            if (exp_list.size() != 0)
            {
                for (map<ObjId, base_net_obj*>::iterator itr = exp_list.begin(); itr != exp_list.end(); ++itr)
                {         	
                    _obj_net_map.erase(itr->second->get_id());
                    itr->second->destroy();
                }
            }       
        }

    protected:
        map<ObjId, base_net_obj*> _obj_net_map;
};

#endif

