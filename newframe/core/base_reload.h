#ifndef __BASE_RELOAD_H__
#define __BASE_RELOAD_H__
#include "log_helper.h"

class ReloadInf {
    public:
        virtual ~ReloadInf() {}
        virtual int Init() = 0;
        virtual int Load(const char* path, const char* fname) = 0;
        virtual int Reload(const char* path, const char* fname) = 0;
        virtual int Dump(const char* path, const char* fname) = 0;
        virtual int Destroy() = 0;
};

template<typename ClassT>
class ReloadMgr {
    public:
        ReloadMgr() {}
        ~ReloadMgr() {}

        int Init(){
            objects_[0].Init();
            objects_[1].Init();
            return 0;
        }

        int Load(const char* path,const char* fname) {
            SetPathFile(path,fname);
            if( objects_[0].Load(path_,file_) == 0 ) {
                struct stat st;
                stat(fullpath_, &st);
                last_load_ = st.st_mtime;
                curr_ = 0;
                return 0;
            }

            WARNING_LOG("load data failed, %s/%s,%d", path_,file_);
            return -1;
        }

        int Reload(const char* path,const char* fname) {
            SetPathFile(path,fname);
            struct stat st;
            if ( stat(fullpath_, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != last_load_ ) {
                DEBUG_LOG("reload thread check time. file time[%lu],last load time[%lu]\n",st.st_mtime,last_load_);
                if ( objects_[1-curr_].Reload(path_, file_) == 0 ) {
                    last_load_ = st.st_mtime;
                    curr_ = 1- curr_;
                    return 0;
                } else {
                    WARNING_LOG("reload data failed, %s/%s,%d", path_,file_,curr_);
                    return -1;
                }
            }

            return 0;
        }

        ClassT* Current() {
            if( curr_ == 0 || curr_ == 1){
                return &(objects_[curr_]);
            }

            return NULL;
        }

        void Dump(const char* path,const char* fname){
            ClassT* obj = Current();
            obj->Dump(path,fname);
        }

        int Destroy(){
            objects_[0].Destroy();
            objects_[1].Destroy();
            return 0;
        }

    private:
        void SetPathFile(const char* path,const char* fname) {
            snprintf(path_,sizeof(path_),"%s",path);
            snprintf(file_,sizeof(file_),"%s",fname);
            snprintf(fullpath_, sizeof(fullpath_), "%s/%s", path_, file_);
        }
    private:
        char path_[PATH_MAX_LEN];
        char file_[FILE_MAX_LEN];
        char fullpath_[FULLPATH_MAX_LEN];
        ClassT objects_[2];
        int16_t curr_;
        time_t last_load_;
};

#endif
