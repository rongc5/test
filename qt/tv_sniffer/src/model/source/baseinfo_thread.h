#ifndef __BASETHREAD_H_
#define __BASETHREAD_H_
#include "data_struct.h"
#include <pthread.h>

class BaseThread
{
public:
		/**
		 * @brief 对外提供获取基本信息接口
		 *
		 * @param info 将获取的基本信息存储与改指针指向地址内
		 */
    void getBasic(BasicInfo * info);
    void start();
    BaseThread();
    ~BaseThread();
private:
	/**
	 * @brief 解析网络相关信息，
	 *
	 * @param buf 由用户传入的网络信息buf
	 */
    void getNetworkName();
    void getLocalIp();
	/**
	 * @brief 获取网络相关信息， 并存入buf
	 *
	 * @param buf 存储获取的网络信息
	 * @param size buf的大小
	 */
    static void * getNetworkInfo(void * arg);
    /**
	 * @brief 解析buf中haier电视机相关信息
	 *
	 * @param buf 由用户传入的用户信息
	 */
    void prase_haier_info(char *buf);
	/**
	 * @brief 获取用户信息并存入buf
	 *
	 * @param buf 存储用户信息
	 * @param size buf的大小
	 */
    void getTvMacIp();
    void getHaierInfo(char *buf, size_t size);
    int prase_str(char *buf, const char *name, char *save);
    static void * getHaierTvInfo(void *arg);
    int getHaierTvInfobuf(char *buf, size_t size, char *net_name);
private:
	/**
	 * @brief 用户基本信息存储
	 */
    pthread_t Local_tid;
    pthread_t Tv_tid;

    BasicInfo base;
    pthread_mutex_t Local_lock;
    pthread_mutex_t Tv_lock;
};

#endif

