#pragma once

#include "noncopyable.h"

#include <memory>
#include <thread>
#include <algorithm>
#include <condition_variable>
#include <functional>
#include <chrono>
namespace engine
{
	class ThreadPool : NonCopyable
	{
	private:

		std::atomic<bool> m_bPaused = false;

		std::atomic<bool> m_bRuning = false;
		std::atomic<bool> m_bWaiting = false;
		mutable std::mutex m_taskQueueMutex;

		std::unique_ptr<std::thread[]> m_threads = nullptr;

	private:
		void worker()
		{
			while (m_bRuning)
			{
				std::function<void()> task;

				std::unique_lock<std::mutex> lockTasksQueue(m_taskQueueMutex);
			}
		}
	};
}