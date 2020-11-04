#include "skhttp_res_thread.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "log_helper.h"
#include "base_connect.h"
#include "skhttp_res_data_process.h"
#include "proc_data.h"

skhttp_res_thread::skhttp_res_thread()
{
    //reg_for_date();
}

void skhttp_res_thread::reg_for_date()
{
    proc_data* p_data = proc_data::instance();
    std::vector<base_net_thread *> * req_thread = p_data->get_thread("req_thread"); 
    if (!req_thread)
        return;

    std::shared_ptr<reg_date_msg>  net_obj(new reg_date_msg);
    net_obj->_msg_op = NORMAL_MSG_REG_FOR_DATE_INFO;

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = (*req_thread->begin())->get_thread_index();

    net_obj->_thread_index = get_thread_index();

    std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);

    base_net_thread::put_obj_msg(id, ng);
}

void skhttp_res_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    if (!p_msg) 
        return;

    LOG_DEBUG("recive msg %d",p_msg->_msg_op);   
    switch(p_msg->_msg_op)
    {
        case NORMAL_MSG_CONNECT:
        {
            std::shared_ptr<content_msg> p=std::dynamic_pointer_cast<content_msg>(p_msg);
            if (p)
            {
                skhttp_res_data_process::gen_listen_obj(p->fd, _base_container);
            }
        }
        break;
        //case NORMAL_MSG_UPDATE_DATE_INFO:
        //{
            //std::shared_ptr<date_msg> p=std::dynamic_pointer_cast<date_msg>(p_msg);
            //if (p)
            //{
                //_req_date = p->req_date;
                //_trade_date = p->_trade_date;
            //}
        //}
        //break;
    }
}




