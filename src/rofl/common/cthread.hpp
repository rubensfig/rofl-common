/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * cthread.hpp
 *
 *  Created on: May 21, 2015
 *      Author: andi
 */

#ifndef SRC_ROFL_COMMON_CTHREAD_HPP_
#define SRC_ROFL_COMMON_CTHREAD_HPP_

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <inttypes.h>
#include <ostream>
#include <list>
#include <map>
#include <set>

#include "rofl/common/exception.hpp"
#include "rofl/common/locking.hpp"
#include "rofl/common/logging.hpp"
#include "rofl/common/ctimer.hpp"

namespace rofl {

class eThreadBase : public exception {
public:
	eThreadBase(
			const std::string& __arg) :
				exception(__arg)
	{};
};

class eThreadNotFound : public eThreadBase {
public:
	eThreadNotFound(
			const std::string& __arg) :
				eThreadBase(__arg)
	{};
};

class cthread; // forward declaration

class cthread_env
{
	friend class cthread;
public:
	static
	cthread_env&
	call_env(cthread_env* env) {
		AcquireReadLock lock(cthread_env::thread_envs_lock);
		if (cthread_env::thread_envs.find(env) == cthread_env::thread_envs.end()) {
			throw eThreadNotFound("cthread_env::call_env() cthread_env instance not found");
		}
		return *(env);
	};
public:
	virtual
	~cthread_env() {
		AcquireReadWriteLock lock(cthread_env::thread_envs_lock);
		cthread_env::thread_envs.erase(this);
	};
	cthread_env() {
		AcquireReadWriteLock lock(cthread_env::thread_envs_lock);
		cthread_env::thread_envs.insert(this);
	};
protected:
	virtual void
	handle_wakeup(
			cthread& thread)
	{};
	virtual void
	handle_timeout(
			cthread& thread, uint32_t timer_id, const std::list<unsigned int>& ttypes)
	{};
	virtual void
	handle_read_event(
			cthread& thread, int fd)
	{};
	virtual void
	handle_write_event(
			cthread& thread, int fd)
	{};
private:
	static std::set<cthread_env*>  thread_envs;
	static crwlock                 thread_envs_lock;
};



class cthread {
public:

	/**
	 *
	 */
	virtual
	~cthread()
	{ release(); };

	/**
	 *
	 */
	cthread(
			cthread_env* env) :
				env(env)
	{ initialize(); };

public:

	/**
	 *
	 */
	pthread_t
	get_thread_id() const
	{ return tid; };

public:

	/**
	 * @brief	Wake up RX thread via rx pipe
	 */
	void
	wakeup();

	/**
	 * @brief	Add file descriptor to set of observed fds
	 */
	void
	add_read_fd(
			int fd);

	/**
	 * @brief	Drop file descriptor from set of observed fds
	 */
	void
	drop_read_fd(
			int fd);

	/**
	 * @brief	Add file descriptor to set of observed fds
	 */
	void
	add_write_fd(
			int fd);

	/**
	 * @brief	Drop file descriptor from set of observed fds
	 */
	void
	drop_write_fd(
			int fd);

public:

	/**
	 *
	 */
	void
	clear_timers();

	/**
	 *
	 */
	ctimer&
	add_timer(
			uint32_t timer_id, const ctimespec& tspec);

	/**
	 *
	 */
	ctimer&
	set_timer(
			uint32_t timer_id);

	/**
	 *
	 */
	const ctimer&
	get_timer(
			uint32_t timer_id) const;

	/**
	 *
	 */
	bool
	drop_timer(
			uint32_t timer_id);

	/**
	 *
	 */
	bool
	has_timer(
			uint32_t timer_id) const;

private:

	/**
	 * @brief	Initialize data structures for running TX and RX threads
	 *
	 * Opens an epoll socket and registers file read descriptor for wake up
	 * pipes for TX and RX thread. Does not prepare the Radius socket itself.
	 * See radius_open() for adding the Radius socket to the running event
	 * loop. Starts TX and RX loop.
	 */
	void
	initialize();

	/**
	 * @brief	Release data structures allocated for worker thread
	 *
	 * Closes Radius socket and TX and RX pipe file descriptors. Stops TX and RX
	 * threads.
	 */
	void
	release();

	/**
	 * @brief	Starts worker thread
	 *
	 * Sets run flag run_rx_thread to true and calls
	 * syscall pthread_create() for starting worker thread.
	 */
	void
	start_thread();

	/**
	 * @brief	Stops worker thread
	 *
	 * Stops worker thread by setting variable run_rx_thread
	 * to false and waking up the thread. Waits for thread termination via pthread_join().
	 * If joining the thread fails, syscall pthread_cancel() destroys ultimately the thread.
	 */
	void
	stop_thread();

	/**
	 * @brief	Static function for starting RX thread for this cradsock instance
	 */
	static void*
	start_loop(
			void* arg)
	{ return static_cast<cthread*>(arg)->run_loop(); };

	/**
	 * @brief	Main loop for RX thread
	 */
	void*
	run_loop();

private:

	// thread environment
	cthread_env*        env;

	// internal logging facility
	logging             log;

	// thread related variables
	static const int 	PIPE_READ_FD;
	static const int 	PIPE_WRITE_FD;
	int 				pipefd[2];	// pipe descriptors for worker thread
	pthread_t			tid;		// pthread_t for worker thread
	bool				run_thread;	// true: continue to run worker thread
	bool				wakeup_pending;	// true: wakeup already scheduled for worker thread
	int 				retval;		// worker thread return value
	int					epfd;		// worker thread epoll fd

	crwlock				tlock;		// thread lock

	std::set<int>		rfds;		// set of file descriptors observed for reading
	std::set<int>		wfds;		// set of file descriptors observed for writing

	std::set<ctimespec> ordered_timers;     // ordered set of timers
	std::map<uint32_t, ctimer*> timers;     // map of timers
};

}; // end of namespace rofl

#endif /* SRC_ROFL_COMMON_CTHREAD_HPP_ */
