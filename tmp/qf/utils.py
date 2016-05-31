#!/usr/bin/env python
#coding=utf-8



def trimStr(str):
    str = str.replace(" ", "")
    str = str.replace("\r", "")
    str = str.replace("\t", "")
    str = str.replace("\n", "")
    return str

def cmp2str(str1, str2):
    str1 = trimStr(str1)
    str2 = trimStr(str2)

    return id(str1) == id(str2)

if __name__ == '__main__':
    str = '''if (id == -1){
                printf("%d\\n", id);
                        return ;
                        }

                            printf("id:%d, %d\\n", id, a[id]);'''
    #str = str.replace("\n", "\\n")
    print str


