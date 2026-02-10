CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -O2
TARGET   = sistema_frutas

SOURCES = main.cpp \
          Configuracao.cpp \
          Celeiro.cpp \
          Deposito.cpp \
          Caminhao.cpp \
          Fazendeiro.cpp \
          Transportador.cpp \
          Cliente.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

Configuracao.o: Configuracao.h
Celeiro.o: Celeiro.h Fruta.h Configuracao.h
Deposito.o: Deposito.h Fruta.h Configuracao.h
Caminhao.o: Caminhao.h Fruta.h Configuracao.h
Fazendeiro.o: Fazendeiro.h Fruta.h Celeiro.h Configuracao.h
Transportador.o: Transportador.h Celeiro.h Deposito.h Caminhao.h Configuracao.h
Cliente.o: Cliente.h Deposito.h Configuracao.h
main.o: Configuracao.h Celeiro.h Deposito.h Caminhao.h Fazendeiro.h Transportador.h Cliente.h

.PHONY: all clean run rebuild
