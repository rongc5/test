#coding=utf-8
#!/usr/bin/python

from reportlab.pdfgen import canvas

def hello(c):
    c.drawString(50,50,"Hello World")


if __name__ == '__main__':
    c = canvas.Canvas("hello.pdf")
    hello(c)
    c.showPage()
    c.save()


