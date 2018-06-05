#/usr/bin/env python
#encoding=utf-8

import jieba
import jieba.posseg
import jieba.analyse

if __name__ == '__main__':
    #seg_list = jieba.cut("来来到了北京", cut_all=True)
    #seg_list = jieba.cut_for_search("小明硕士毕业于中国科学院计算所，后在日本京都大学深造")
    #seg_list = jieba.cut_for_search("四川省")
    seg_list = jieba.cut_for_search("食品、饮料、烟草和家庭用品批发业")
    for key in seg_list:
        print key
    import jieba.posseg as pseg
    words =pseg.cut("四川省")
    for w in words:
        print w.word,w.flag
