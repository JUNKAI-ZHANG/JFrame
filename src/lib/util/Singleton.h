#ifndef __SINGLETON_H_
#define __SINGLETON_H_

template <typename T>
class Singleton {
   public:
    static T& Instance() {
        static T* kInstance = new T;
        return *kInstance;
    }

   protected:
    Singleton() {}

   private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
};

#endif  // __SINGLETON_H_