#include "Thread.h"

using namespace std;

//Need to actually "allocate" static member
pthread_mutex_t Thread::mutex;

Thread::Thread() = default;

int Thread::Create(void *Callback, void *args) {
	int tret = 0;

	//Supercreepy typecast
	tret = pthread_create(&this->tid, nullptr, (void *(*)(void*))(Callback), args);

	if (tret) {
		cerr << "Error while creating threads." << endl;
		return tret;
	}
	cout << "Thread successfully created." << endl;
	return 0;
}

int Thread::Join() const
{
	pthread_join(this->tid, nullptr);
	return 0;
}

int Thread::InitMutex() {

	if (pthread_mutex_init(&Thread::mutex, nullptr) < 0) {
		cerr << "Error while initializing mutex" << endl;
		return -1;
	}

	cout << "Mutex initialized." << endl;
	return 0;
}

/*
LockMutex():
Blocks until mutex becomes available
*/
int Thread::LockMutex(const char *identifier) {
	cout << identifier << " is trying to acquire the lock..." << endl;

	if (pthread_mutex_lock(&Thread::mutex) == 0) {
		cout << identifier << " acquired the lock!" << endl;
		return 0;
	}

	cerr << "Error while " << identifier << " was trying to acquire the lock" << endl;
	return -1;
}

int Thread::UnlockMutex(const char *identifier) {
	cout << identifier << " is trying to release the lock..." << endl;

	if (pthread_mutex_unlock(&Thread::mutex) == 0) {
		cout << identifier << " released the lock!" << endl;
		return 0;
	}

	cerr << "Error while " << identifier << " was trying to release the lock" << endl;
	return -1;
}
