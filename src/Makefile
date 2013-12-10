CC := g++ -std=gnu++11 -g
CFLAGS = -Wall -c
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard $(INCL_HOME)/*.h)
OBJECTS := $(addprefix $(OBJ_HOME)/,$(SOURCES:.cpp=.o))
INCLUDES := -I$(INCL_HOME)

main: $(OBJECTS)
	

$(OBJ_HOME)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDES)

.PHONY: clean
clean:
	