#pragma once

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
#include "Global.h"

using namespace std;

class Thread {
public:
	pthread_t threadId{};

private:
	static pthread_mutex_t mutex;

public:
	Thread();
	int Create(void *Callback, void *args);
	int Join() const;

	static int InitMutex();
	static int LockMutex(const string& identifier);
	static int UnlockMutex(const string& identifier);
};