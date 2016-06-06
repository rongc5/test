JAVAC=javac
sources = $(wildcard *.java)
classes = $(sources:.java=.class)

all:$(class)

clean:
    rm -f *.class
%.class:%.java
    $(JAVAC) $<
