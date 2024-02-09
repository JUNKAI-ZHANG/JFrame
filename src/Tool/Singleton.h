#ifndef __SINGLETON_H_
#define __SINGLETON_H_

template<typename T>
class Singleton {
public:
    static T& Instance() {
        static T *_instance = new T;
        return _instance;
    }

protected:
    Singleton() {}

private:
    Singleton(const Singleton& );
    Singleton& operator=(const Singleton& );
};

#endif // __SINGLETON_H_