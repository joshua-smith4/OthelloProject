#ifndef EVENTSYSTEM_HPP_INCLUDED
#define EVENTSYSTEM_HPP_INCLUDED

#include <thread>
#include <atomic>
#include <map>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace evt
{
    using event_id = unsigned long;

    template <class T>
    class EventSystem;

    template <class Ret, class... Args>
    class EventSystem<Ret(Args...)>
    {
    public:
        struct Event
        {
        friend class EventSystem<Ret(Args...)>;
        private:
            Event() : id(0u), args() {}
        public:
            Event(event_id i) : id(i), args() {}
            Event(event_id i, std::tuple<Args...> a) : id(i), args(a) {}
            unsigned id;
            std::tuple<Args...> args; 
        };

        using handler_type = std::function<Ret(Args...)>;

        EventSystem(size_t n)
            : handler_map(), universal_handlers(),
            thread_pool(), event_pool(), 
            _continue(true), num_threads(n),
            event_pool_mtx(false), cv_mtx(), event_pool_cv()
        {
            for(auto i = 0u; i < num_threads; ++i) 
                thread_pool.emplace_back(
                        std::thread(&EventSystem::worker_routine, this));
        }

        ~EventSystem()
        {
            _continue = false;
            for(auto&& t : thread_pool) t.join();
        };

        EventSystem(EventSystem const&) = delete;
        EventSystem& operator=(EventSystem const&) = delete;

        void add_handler(event_id evt_id, handler_type handler)
        {
            handler_map[evt_id].push_back(handler);
        }
        void add_universal_handler(handler_type handler)
        {
            universal_handlers.push_back(handler);
        }
        void remove_handlers(event_id evt_id)
        {
            handler_map.erase(evt_id);
        }
        void remove_universal_handers()
        {
            universal_handlers.clear();
        }
        void post(Event const& evt)
        {
            while(event_pool_mtx.test_and_set(std::memory_order_acquire));
            event_pool.push(evt);
            event_pool_mtx.clear(std::memory_order_release);
            event_pool_cv.notify_all();
        }
        
    private:
        void get_and_execute_event()
        {
                Event evt;
                bool foundEvent = false;
                {
                    while(event_pool_mtx.test_and_set(std::memory_order_acquire));
                    if(!event_pool.empty())
                    {
                        evt = event_pool.front();
                        event_pool.pop();
                        foundEvent = true;
                    } 
                    event_pool_mtx.clear(std::memory_order_release);
                }
                if(!foundEvent) return;
                // TODO: may need to protect handler_map with mutex as well
                for(auto&& handler : universal_handlers) 
                    std::apply(handler, evt.args);
                auto handlers_ptr = handler_map.find(evt.id);
                if(handlers_ptr == handler_map.end()) return;
                for(auto&& handler : handlers_ptr->second)
                {
                    std::apply(handler, evt.args);
                }
        }

        void worker_routine()
        {
            while(_continue)
            {
                std::unique_lock<std::mutex> lck(cv_mtx);
                event_pool_cv.wait_for(lck,
                        std::chrono::milliseconds(300),
                        [this]()
                        {
                        return !this->event_pool.empty() || !_continue;
                        });
                get_and_execute_event();
            }
            while(!event_pool.empty())
            {
                get_and_execute_event();
            }
        }

        std::map<event_id,std::vector<handler_type>> handler_map; 
        std::vector<handler_type> universal_handlers;
        std::vector<std::thread> thread_pool;
        std::queue<Event> event_pool;

        bool _continue;
        size_t num_threads;
        std::atomic_flag event_pool_mtx;
        std::mutex cv_mtx;
        std::condition_variable event_pool_cv;
    };
};

#endif

