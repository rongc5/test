#!/usr/bin/env python
#encoding=utf-8

from sklearn import feature_extraction
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer

import jieba
import jieba.analyse
import jieba.posseg as pseg
import optparse

__author__ = 'rong'

def log_write(filename, str, mode='a'):
    file = open(filename, mode)
    file.write(str)
    file.write('\n')
    file.flush()
    file.close()

def tfidf(parser, corpus = []):
    print len(corpus), 'tfidf'
    if not len(corpus):
        return

    vectorizer = CountVectorizer()
    X = vectorizer.fit_transform(corpus)
    print X

    transformer = TfidfTransformer()
    tfidf = transformer.fit_transform(X)
    word = vectorizer.get_feature_names()

    weight = tfidf.toarray()
    print word
    print len(word)
    #for j in range(len(word)):
        #print word[j]

    #for i in range(len(weight)):
        #for j in range(len(word)):
            #print weight[i][j]
            ## print '\n'

def do_search():
    parser = optparse.OptionParser()

    parser.add_option("-f", "--file",
                  action="store", dest="file",type="string", default="",
              help="files")

    parser.add_option("-o", "--out",
                  action="store", dest="out",type="string", default="",
              help="files")

    options, args = parser.parse_args()

    if not options.file:
        return

    corpus = []
    file = open(options.file)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        items = line.split('\t')

        cut_list = pseg.cut(items[1].decode('gb18030'), HMM=True)
        #print cut_list
         #jieba.analyse.set_stop_words(STOPWORDPATH)
         #   cut_list = jieba.analyse.extract_tags(items[i].decode('gb18030'), allowPOS=["ns", "n", "vn", "nr", "nt", "nz", "eng", "j", "nrt"])
         #   ##cut_list = jieba.analyse.textrank(items[i].decode('gb18030'), 2)
         #   for key in cut_list:
         #       if key is not None and key not  in remove_words:
         #           print items[i], key.encode('gb18030')
         #           if key.encode('gb18030') not in res_list:
         #               res_list.append(key.encode('gb18030'))
        res_str = ''
        for key, flag in cut_list:
            if key is not None and key not  in stopwords:
                res_str += key
                res_str += ' '
        #print res_str
        corpus.append(res_str)

    file.close()
    tfidf(parser, corpus)


STOPWORDPATH = '../stock/data/stopword'
stopwords = {}
def load_stopwords():
    file = open(STOPWORDPATH)
    while 1:
        line = file.readline().strip()
        if not line:
            break
        stopwords[line.strip('\r\n').decode('UTF-8')] = line.strip('\r\n').decode('UTF-8')
    file.close()

    return stopwords

if __name__ == '__main__':
    load_stopwords()
    do_search()
