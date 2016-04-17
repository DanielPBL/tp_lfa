CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=

RM=rm
RMFLAGS=-rf

TARGET=build/gerador
OBJS=obj/lexico.o obj/sintatico.o obj/alfabeto.o obj/globals.o obj/estado.o obj/transicao.o obj/afd.o obj/main.o 

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(TARGET)

run:	$(TARGET)
	./build/gerador entrada.afd
	for i in $$(ls | grep '.dot$$'); do dot -Tpdf -o $$(echo $$i | sed -e 's/.dot//').pdf $$i; done

$(TARGET):	$(OBJS)
		$(CXX) -o $(TARGET) $(OBJS) $(LDXXFLAGS)

obj/lexico.o: header/lexico.h src/lexico.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/lexico.cpp

obj/alfabeto.o: header/alfabeto.h src/alfabeto.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/alfabeto.cpp

obj/globals.o: header/globals.h obj/alfabeto.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/globals.cpp

obj/estado.o: header/estado.h src/estado.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/estado.cpp

obj/transicao.o: header/transicao.h src/transicao.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/transicao.cpp

obj/afd.o: header/afd.h src/afd.cpp obj/estado.o obj/transicao.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/afd.cpp

obj/sintatico.o: header/sintatico.h header/tp.h src/sintatico.cpp obj/lexico.o obj/afd.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/sintatico.cpp

obj/main.o: header/tp.h src/main.cpp obj/sintatico.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp
