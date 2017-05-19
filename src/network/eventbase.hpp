/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Mushroom
 *    > Created Time:  2017-05-06 23:45:21
**/

#ifndef _EVENT_BASE_HPP_
#define _EVENT_BASE_HPP_

#include <map>

#include "../include/utility.hpp"
#include "../include/atomic.hpp"
#include "../include/mutex.hpp"

namespace Mushroom {

class Poller;
class Channel;
template<typename T> class ThreadPool;
template<typename T> class BoundedQueue;

typedef std::pair<int64_t, uint32_t> TimerId;
typedef std::pair<int64_t, int64_t>  TimeRep;

class EventBase : private NoCopy
{
	public:
		EventBase(int thread_num, int queue_size);

		~EventBase();

		void Loop();

		void Exit();

		Poller* GetPoller();

		void RunNow(const Task &task);

		TimerId RunAfter(int64_t milli_sec, const Task &task);

		TimerId RunEvery(int64_t milli_sec, const Task &task);

		void Cancel(const TimerId &timer_id);

	private:
		void HandleTimeout();

		void WakeUp();

		void Repeat();

		void Refresh();

		bool     running_;
		int      wake_up_[2];
		Channel *channel_;
		Poller  *poller_;

		uint64_t pid_;
		int      next_time_out_;

		atomic_32_t seq_;

		BoundedQueue<Task> *queue_;
		ThreadPool<Task>   *pool_;

		Mutex                       mutex_;
		std::map<uint32_t, TimeRep> repeat_;
		std::map<TimerId, Task>     pending_;
};

} // namespace Mushroom

#endif /* _EVENT_BASE_HPP_ */