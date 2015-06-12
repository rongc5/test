#encoding=UTF-8
#!/usr/bin/python

import os, sys, logging, logging.handlers




format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s ->%(message)s'
handler = logging.handlers.RotatingFileHandler('test.log', 'a', 256*1024*1024, 0)
formatter = logging.Formatter(format)
handler.setFormatter(formatter)
logger = logging.getLogger('test')
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)



#log = logging.getLogger('test')
#fh = logging.FileHandler('test')


logger.info('first info message')
logger.debug("debug message")
logger.error("debug message")
