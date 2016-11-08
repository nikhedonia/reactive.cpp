#ifndef REACTIVE_THREAD_POOL_HPP
#define REACTIVE_THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace reactive {

class ThreadPool {
public:
    ThreadPool(size_t);
    void enqueue(std::function<void()>);
    ~ThreadPool();
private:
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
  : stop(false) {
  for(size_t i = 0;i<threads;++i)
    workers.emplace_back([this] {
    while (true) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->condition.wait(lock,[this]{
          return this->stop || !this->tasks.empty();
        });

        if(this->stop && this->tasks.empty()) { return; }
        task = std::move(this->tasks.front());
        this->tasks.pop();
      }
      task();
    }
  });
}

// add new work item to the pool
void ThreadPool::enqueue(std::function<void()> f) {
  std::unique_lock<std::mutex> lock(queue_mutex);
  tasks.emplace(f);
  condition.notify_one();
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for(std::thread &worker: workers) {
    worker.join();
  }
}


auto wait = [](auto time) {
  return [time](auto push, auto...x) {
    std::this_thread::sleep_for(time);
    return push(x...);
  };
};

auto asyncify = [](auto& Pool) {
  return [&] (auto push, auto...x) {
    Pool.enqueue([=]()mutable{ push(x...); });
  };
};

}

#endif
