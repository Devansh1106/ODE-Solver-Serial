
# This is a simple makefile just to build PROG
#COMP_FLAGS = -O0 -ggdb3 -fsanitize=address

#COMP_FLAGS = -O3

DEF_FLAGS   = $(COMP_FLAGS) -mcmodel=small	-Wall	-g -Wextra

CXX := g++ $(DEF_FLAGS)

DIR_SRC := $(wildcard source/*.cpp)

CFLAGS = -I include

PROG    = main

####################################################################
# Lines from here to down should not be changed. They are the actual
# rules which make uses to build PROG
.KEEP_STATE:

all : $(PROG)

OBJS = $(DIR_SRC:%.cpp=%.o)
$(PROG):$(OBJS)
	g++	-o $(PROG) $(OBJS) 	$(CFLAGS)
	rm -f *.o $(OBJS)

run : all
	./$(PROG)
