
#ifndef _START_THREAD_H_
#define _START_THREAD_H_

class QThread;
class IModel;

class CStartThread : public QThread
{
public:
    CStartThread (IModel * model);
protected:
    virtual void run();
private:
    IModel * m_model;
};



#endif