#include "worker_thread.h"
#include "log_helper.h"
#include "rsingle_data_process.h"
#include "proc_data.h"
#include "rquotation_data_process.h"
#include "sk_def.h"
#include "rmain_funds_data_process.h"


void worker_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    if (p_msg == nullptr)
        return;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    switch(p_msg->_msg_op)
    {
        case NORMAL_MSG_REQ_QUOTATION:
            {
                auto p=std::dynamic_pointer_cast<req_msg>(p_msg);
                if (!p || !p->_id_vec)
                {
                    return;
                }

                if (!strategy->is_req_quotation_from_file)
                {
                    rquotation_data_process::gen_net_obj(p->_id_vec, get_net_container(), p->_headers);
                }
                else
                {
                    rquotation_data_process::load_from_curl(p->_id_vec, get_net_container(), p->_headers, _curl);
                }
            }
            break;
        case NORMAL_MSG_REQ_SINGLE:
            {
                auto p=std::dynamic_pointer_cast<req_msg>(p_msg);
                if (!p || !p->_id_vec)
                {
                    return;
                }

                if (!strategy->is_req_single_from_file) 
                {
                    rsingle_data_process::gen_net_obj(*p->_id_vec->begin(), get_net_container(), p->_headers);
                }
                else
                {
                    rsingle_data_process::load_from_curl(*p->_id_vec->begin(), get_net_container(), p->_headers, _curl);
                }
            }
            break;
        case NORMAL_MSG_REQ_MAIN_FUNDS:
            {
                auto p=std::dynamic_pointer_cast<req_msg>(p_msg);
                if (!p || !p->_id_vec)
                {
                    return;
                }

                if (!strategy->is_req_main_funds_from_file) 
                {
                    rmain_funds_data_process::gen_net_obj(*p->_id_vec->begin(), get_net_container(), p->_headers);
                }
                else
                {
                    rmain_funds_data_process::load_from_curl(*p->_id_vec->begin(), get_net_container(), p->_headers, _curl);
                }
            }
            break;
    }

}

void worker_thread::put_req_msg(std::shared_ptr<req_msg> & p_msg, int index)
{
    proc_data* p_data = proc_data::instance();
    std::vector<base_net_thread *> * req_thread = p_data->get_thread("worker_thread"); 
    if (!req_thread)
        return;

    if (!req_thread->size())
    {
        return;
    }
    index = index % req_thread->size();
    LOG_DEBUG("index: %d, size:%d", index, req_thread->size());

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = req_thread->at(index)->get_thread_index();


    std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(p_msg);
    base_net_thread::put_obj_msg(id, ng);
}

curl_req & worker_thread::get_curl()
{
    return _curl;
}
