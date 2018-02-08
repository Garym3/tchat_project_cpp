#pragma once

// Needed for pthread compilation
#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <string>
#include "../include/pthread.h"

#include "Global.h"

using namespace std;

class Thread {
public:
	pthread_t threadId{};

private:
	static pthread_mutex_t mutex;

public:
	Thread();
	int create(void *Callback, void *args);

	static int init_mutex();
	static int lock_mutex(const string& identifier);
	static int unlock_mutex(const string& identifier);
};