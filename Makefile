CC := g++ -std=gnu++11 -g 
export OBJ_HOME := $(realpath obj)
export INCL_HOME := $(realpath include)
LIBS := -lGL -lGLU -lglut -lboost_system -lpng
EXECUTABLE := pong

$(EXECUTABLE): $(wildcard src/*) $(wildcard include/*)
	cd src && $(MAKE)
	$(CC) -Wall obj/*.o  $(LIBS) -o $(EXECUTABLE)

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f $(EXECUTABLE)
	cd src && $(MAKE) clean
