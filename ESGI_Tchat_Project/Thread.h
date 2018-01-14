#ifndef _mythread_h_
#define _mythread_h_

#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <string>
#include <pthread.h>

#ifndef __WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>

using namespace std;

class Thread {
public:
	pthread_t tid{};

private:
	static pthread_mutex_t mutex;

public:
	Thread();
	int Create(void *Callback, void *args);
	int Join() const;

	static int InitMutex();
	static int LockMutex(const char *identifier);
	static int UnlockMutex(const char *identifier);
};

#endif