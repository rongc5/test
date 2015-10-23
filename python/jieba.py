#/usr/bin/env python
#encoding=utf-8

import jieba
import jieba.posseg
import jieba.analyse

if __name__ == '__main__':
    #seg_list = jieba.cut("来来到了北京", cut_all=True)
    seg_list = jieba.cut_for_search("小明硕士毕业于中国科学院计算所，后在日本京都大学深造")
    #print seg_list
