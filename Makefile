CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=

RM=rm
RMFLAGS=-rf

TARGET=build/gerador
OBJS=obj/lexico.o obj/main.o obj/sintatico.o

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(TARGET)

run:	$(TARGET)
	./build/gerador entrada.afd

$(TARGET):	$(OBJS)
		$(CXX) -o $(TARGET) $(OBJS) $(LDXXFLAGS)

obj/lexico.o: header/lexico.h src/lexico.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/lexico.cpp

obj/sintatico.o: header/sintatico.h header/tp.h src/sintatico.cpp obj/lexico.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/sintatico.cpp

obj/main.o: header/tp.h src/main.cpp obj/sintatico.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp
