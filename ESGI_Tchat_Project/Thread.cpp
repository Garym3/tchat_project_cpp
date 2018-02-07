#include "Thread.h"

using namespace global;

pthread_mutex_t Thread::mutex;

Thread::Thread() = default;

/// <summary>
/// Creates a thread
/// </summary>
/// <param pseudo="Callback">Callback</param>
/// <param pseudo="args">Callback arguments</param>
/// <returns>Success/failure state of the creation of the thread</returns>
int Thread::create(void *Callback, void *args)
{
	const int threadState = pthread_create(&this->threadId, nullptr, reinterpret_cast<void *(*)(void*)>(Callback), args);

	if (threadState) {
		cerr << "Error: could not create the thread." << endl;
		return threadState;
	}

	printf("Thread successfully created.%s", newLine.c_str());

	return 0;
}


/// <summary>
/// Initializes a Mutex
/// </summary>
/// <returns>Success/failure state of the initialization of the Mutex</returns>
int Thread::init_mutex()
{
	if (pthread_mutex_init(&Thread::mutex, nullptr) < 0){
		cerr << "Error: could not initialize the mutex" << endl;
		return -1;
	}

	printf("Mutex initialized.%s", newLine.c_str());

	return 0;
}

/// <summary>
/// Blocks the Mutex until it becomes available
/// </summary>
/// <param pseudo="identifier">Name of function which is blocking the Mutex</param>
/// <returns>Success/failure state of the blocking of the Mutex</returns>
int Thread::lock_mutex(const string& identifier)
{
	if (pthread_mutex_lock(&Thread::mutex) == 0) {
		printf("%s acquired the lock.%s", identifier.c_str(), newLine.c_str());
		return 0;
	}

	cerr << "Error: " << identifier << " could not acquire the lock" << endl;

	return -1;
}

/// <summary>
/// Unblocks the Mutex
/// </summary>
/// <param pseudo="identifier">Name of function which is unblocking the Mutex</param>
/// <returns>Success/failure state of the unblocking of the Mutex</returns>
int Thread::unlock_mutex(const string& identifier)
{
	if (pthread_mutex_unlock(&Thread::mutex) == 0) {
		printf("%s released the lock.%s", identifier.c_str(), newLine.c_str());
		return 0;
	}

	cerr << "Error: " << identifier << "could not release the lock" << endl;

	return -1;
}
