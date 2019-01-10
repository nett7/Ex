#ifndef TASK11_THREAD_STACK_H
#define TASK11_THREAD_STACK_H
#include<stack>
#include<thread>
#include<condition_variable>
class ExceptionFunction : public std::exception {
    const char* what() const throw() {
        return "Failed";
    }
};
template<typename T>
class thread_stack {
private:
    std::stack<T> data;
    mutable std::mutex m;
    std::condition_variable cond;
    void lock_g();
    void lock_u();
public:
    thread_stack& operator= (const thread_stack&) = delete;
    thread_stack(const thread_stack& st);
    thread_stack();
    void push(T value);


    void pop(T &value);


    void pop_sleep(T &value);


    bool pop_without_exception(T &value);

    bool empty();
    size_t size();
};
///////////POP_SLEEP///////////////
template <typename T>
void thread_stack<T>::pop_sleep(T &value) {
    lock_u();
    value = std::move(data.top());
    data.pop();
}



/////////////POP_WITHOUT_EXCEPTION///////////
template <typename T>
bool thread_stack<T>::pop_without_exception(T &value) {
    lock_g();
    if(data.empty())
        return false;
    value = std::move(data.top());
    data.pop();
    return true;
}


/////////////PUSH///////////
template <typename T>
void thread_stack<T>::push(T value) {
    lock_g();
    data.push(std::move(value));
    cond.notify_one();
}



template <typename T>
void thread_stack<T>::pop(T &value) {
    lock_g();
    if(data.empty())
        throw ExceptionFunction();
    value=std::move(data.top());
    data.pop();
}
////////////////OTHER_FUNCTION///////////////
template <typename T>
thread_stack<T>::thread_stack() {
}

template <typename T>
thread_stack<T>::thread_stack(const thread_stack& st) {
    st.lock_g();
    data=st.data;
}

template <typename T>
bool thread_stack<T>::empty() {
    lock_g();
    return data.empty();
}

template <typename T>
size_t thread_stack<T>::size() {
    lock_g();
    return data.size();
}
////////////LOCKABLE_FUNCTION///////////
template <typename T>
void thread_stack<T>::lock_g() {
    std::lock_guard<std::mutex> lock(m);
}

template <typename T>
void thread_stack<T>::lock_u() {
    std::unique_lock<std::mutex> lock(m);
    cond.wait(lock, [this]{ return !data.empty();});
}
#endif