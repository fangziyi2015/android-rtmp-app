#ifndef ANDROID_RTMP_APP_SAFE_QUEUE_H
#define ANDROID_RTMP_APP_SAFE_QUEUE_H

#include <queue>

using namespace std;


template<typename T>
class SafeQueue {
    typedef void (*ReleaseCallback)(T *);

    queue<T> data;
    pthread_mutex_t mutex_t;
    pthread_cond_t cond_t;

    bool isWork = false;

    ReleaseCallback releaseCallback;

public:
    SafeQueue() {
        pthread_mutex_init(&mutex_t, nullptr);
        pthread_cond_init(&cond_t, nullptr);
    }

    ~SafeQueue() {
        pthread_mutex_destroy(&mutex_t);
        pthread_cond_destroy(&cond_t);
    }

    void push(T value) {
        pthread_mutex_lock(&mutex_t);

        data.push(value);
        pthread_cond_signal(&cond_t);

        pthread_mutex_unlock(&mutex_t);
    }

    int pop(T &_p) {
        int result = 0;
        pthread_mutex_lock(&mutex_t);
        while (isWork && data.empty()) {
            pthread_cond_wait(&cond_t, &mutex_t);
        }

        if (!data.empty()) {
            _p = data.front();
            data.pop();
            result = 1;
        }

        pthread_mutex_unlock(&mutex_t);
        return result;
    }

    void setWork(bool _isWork) {
        pthread_mutex_lock(&mutex_t);
        this->isWork = _isWork;
        pthread_cond_signal(&cond_t);
        pthread_mutex_unlock(&mutex_t);
    }

    void setReleaseCallback(ReleaseCallback callback) {
        this->releaseCallback = callback;
    }

    int size() {
        return data.size();
    }

    bool empty() {
        return data.empty();
    }

    void clear() {
        pthread_mutex_lock(&mutex_t);
        unsigned int size = data.size();
        for (int i = 0; i < size; ++i) {
            T element = data.front();
            if (releaseCallback) {
                releaseCallback(&element);
            }
            data.pop();
        }
        pthread_mutex_unlock(&mutex_t);
    }

};

#endif //ANDROID_RTMP_APP_SAFE_QUEUE_H
