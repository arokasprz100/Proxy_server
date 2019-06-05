CXX = g++ -g -Wall -Wextra -std=c++14 -pedantic-errors 
LDFLAGS = -lssl -lcrypto
DEP_FLAGS = -MMD -MP

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)
CXXFLAGS+=$(DEP_FLAGS)

TARG = Server

all: $(TARG)

$(TARG): $(OBJ)
	$(CXX) $^$(LFLAGS) -o $@ $(LDFLAGS)

$(OBJ) : $(SRC)
	$(CXX) -c $(SRC) $(DEP_FLAGS)

.PHONY: clean

clean:
	rm $(OBJ) $(TARG) $(DEP)

-include $(DEP)
