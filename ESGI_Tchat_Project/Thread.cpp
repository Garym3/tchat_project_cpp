#include "Thread.h"

using namespace std;

pthread_mutex_t Thread::mutex;

Thread::Thread() = default;

/// <summary>
/// Creates a thread
/// </summary>
/// <param name="Callback">Callback</param>
/// <param name="args">Callback arguments</param>
/// <returns>Success/failure state of the creation of the thread</returns>
int Thread::Create(void *Callback, void *args) {
	const int threadState = pthread_create(&this->threadId, nullptr, static_cast<void *(*)(void*)>(Callback), args);

	if (threadState) {
		cerr << "Error while creating threads." << endl;
		return threadState;
	}
	cout << "Thread successfully created." << endl;
	return 0;
}

/// <summary>
/// Joins two threads (peut-être obsolète)
/// </summary>
/// <returns>Success/failure state of the join of the threads</returns>
int Thread::Join() const
{
	pthread_join(this->threadId, nullptr);
	return 0;
}

/// <summary>
/// Init a Mutex
/// </summary>
/// <returns>Success/failure state of the initialization of the Mutex</returns>
int Thread::InitMutex() {

	if (pthread_mutex_init(&Thread::mutex, nullptr) < 0) {
		cerr << "Error while initializing mutex" << endl;
		return -1;
	}

	cout << "Mutex initialized." << endl;
	return 0;
}

/// <summary>
/// Blocks the Mutex until it becomes available
/// </summary>
/// <param name="identifier">Name of function which is blocking the Mutex</param>
/// <returns>Success/failure state of the blocking of the Mutex</returns>
int Thread::LockMutex(const char *identifier) {
	cout << identifier << " is trying to acquire the lock..." << endl;

	if (pthread_mutex_lock(&Thread::mutex) == 0) {
		cout << identifier << " acquired the lock!" << endl;
		return 0;
	}

	cerr << "Error while " << identifier << " was trying to acquire the lock" << endl;
	return -1;
}

/// <summary>
/// Unblocks the Mutex
/// </summary>
/// <param name="identifier">Name of function which is unblocking the Mutex</param>
/// <returns>Success/failure state of the unblocking of the Mutex</returns>
int Thread::UnlockMutex(const char *identifier) {
	cout << identifier << " is trying to release the lock..." << endl;

	if (pthread_mutex_unlock(&Thread::mutex) == 0) {
		cout << identifier << " released the lock!" << endl;
		return 0;
	}

	cerr << "Error while " << identifier << " was trying to release the lock" << endl;
	return -1;
}
