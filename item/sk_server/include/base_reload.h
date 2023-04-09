#ifndef __BASE_RELOAD_H_
#define __BASE_RELOAD_H_

#include "base_def.h"

/**
 * @brief ��Ҫ�ṩLoad��Reload���ܵ����ͣ���Ҫ�̳д˽ӿڡ�
 * ��ReloadMgr����ʱ��Ҫ����Load��Reload����
 * */
class reload_inf 
{
    public:
        virtual ~reload_inf() {}
        virtual int load() = 0;
        virtual int reload() = 0;
        virtual bool need_reload()=0;
        virtual int dump() = 0;
        virtual int destroy() = 0;
};

/**
 *@brief ���ع����ж��Ƿ���Ҫ����
 */
template<typename T>
class reload_mgr: public reload_inf
{
    public:
        reload_mgr(T * T1, T *T2);
        virtual ~reload_mgr();

        /**
         *@brief ��������
         */
        int load(); 

        /**
         * @brief ��������
         */
        //��鵱ǰ�����ļ����޸�ʱ���Ƿ���ϴμ�¼��ʱ��һ�£� ��һ�����¼���, �����¼�¼���ļ��޸�ʱ�䣬�Լ�curr_ ֵ
        int reload(); 
        //��Ϊÿһ��ReloadMgr ģ����� ��߾��������汾���Ա�� ����ֻ��һ���ǵ�ǰ���õģ� curr_
        //���Ǽ�¼��ǰ���õ�ģ������� ��Load �� Reload �о�����¸ı�־ֵ, �ķ������ص�ǰ��Ч��
        //ģ�������
        T* current(); 

        bool need_reload();

        int dump();

        int destroy();

        T* idle(); 

    private:
        T * _objects[2];
        int16_t _curr;
};

template<typename T>
reload_mgr<T>::reload_mgr(T * T1, T *T2) 
{
    _objects[0] = T1;
    _objects[1] = T2;
    _curr = 0;
}

template<typename T>
reload_mgr<T>::~reload_mgr() 
{
    if (_objects[0])
    {
        _objects[0]->destroy();
        delete _objects[0];
        _objects[0] = NULL;
    }

    if (_objects[1])
    {
        _objects[1]->destroy();
        delete _objects[1];
        _objects[1] = NULL;
    }
}

/**
 *@brief ��������
 */
template<typename T>
int reload_mgr<T>::load() 
{
    if( _objects[_curr]->load() == 0 ) 
    {
        return 0;
    }

    return -1;
}

template<typename T>
bool reload_mgr<T>::need_reload()
{
    return current()->need_reload();
}


/**
 * @brief ��������
 */
//��鵱ǰ�����ļ����޸�ʱ���Ƿ���ϴμ�¼��ʱ��һ�£� ��һ�����¼���, �����¼�¼���ļ��޸�ʱ�䣬�Լ�curr_ ֵ
template<typename T>
int reload_mgr<T>::reload() 
{
    if ( _objects[1 - _curr]->reload() == 0 ) 
    {
        _curr = 1 - _curr;
        return 0;
    } else 
    {
        PDEBUG("reload data failed,%d", _curr);
        return -1;
    }

    return 0;
}
//��Ϊÿһ��ReloadMgr ģ����� ��߾��������汾���Ա�� ����ֻ��һ���ǵ�ǰ���õģ� curr_
//���Ǽ�¼��ǰ���õ�ģ������� ��Load �� Reload �о�����¸ı�־ֵ, �ķ������ص�ǰ��Ч��
//ģ�������
template<typename T>
T* reload_mgr<T>::current() {
    if( _curr == 0 || _curr == 1){
        return (_objects[_curr]);
    }

    return NULL;
}

template<typename T>
int reload_mgr<T>::dump()
{
    reload_inf* obj = current();
    return obj->dump();
}

template<typename T>
int reload_mgr<T>::destroy()
{
    if (_objects[0])
        _objects[0]->destroy();

    if (_objects[1])
        _objects[1]->destroy();

    return 0;
}

template<typename T>
T* reload_mgr<T>::idle()
{
    if( _curr == 0 || _curr == 1){
        return (_objects[1 - _curr]);
    }

    return NULL;
}

#endif
