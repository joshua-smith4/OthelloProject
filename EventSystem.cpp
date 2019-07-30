template <class Ret, class... Args>
EventSystem<Ret(Args...)>::Event::Event(event_id i, std::tuple<Args...> a) : id(i), args(a) {}

template <class Ret, class... Args>
EventSystem<Ret(Args...)>::Event::Event() : id(0u), args() {}

template <class Ret, class... Args>
EventSystem<Ret(Args...)>::EventSystem(size_t num_threads)

template <class Ret, class... Args>
EventSystem<Ret(Args...)>::~EventSystem()

template <class Ret, class... Args>
void EventSystem<Ret(Args...)>::worker_routine()
