PROJECT = asustool

CC = g++

CFLAGS = -g -fpermissive
INCLUDES = -Iinclude

#LFLAGS = -static
LIBS = -lssl -lcrypto -pthread -lz

SRCS = main.cpp crc32.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(PROJECT)
	@echo Compiled $(PROJECT)

$(PROJECT): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(PROJECT)
