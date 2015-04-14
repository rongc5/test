
#include <QThread>
#include "imodel.h"
#include "start_thread.h"


CStartThread::CStartThread (IModel * model)
    : m_model (model)
{

}
 void CStartThread::run()
{
    m_model->start_capture_impl();
}