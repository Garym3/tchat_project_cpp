CC = g++
CPPFLAGS = -std=c++11 -pthread
EXEC_NAME = tchat
INCLUDES = -I/include/QueueUserAPCEx.h -I/include/config.h -I/include/context.h -I/include/implement.h -I/include/need_errno.h -I/include/pthread.h -I/include/sched.h -I/include/semaphore.h
LIBS = 
OBJ_FILES = Client.o History.o Main.o Server.o Thread.o
INSTALL_DIR = /home

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -o $@ -c $<

install :
	mkdir -p $(INSTALL_DIR)
	cp -rf $(EXEC_NAME) $(INSTALL_DIR)
	touch $(INSTALL_DIR)/histo
