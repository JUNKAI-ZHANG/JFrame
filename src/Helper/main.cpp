#include <bits/stdc++.h>

#include <thread>

std::mutex _mtx;
std::recursive_mutex mtx;

void func2();

void func1() {
    std::unique_lock<std::recursive_mutex> lock(mtx);
    std::cout << "into func1" << std::endl;
    func2();
}

void func2() {
    std::unique_lock<std::recursive_mutex> lock(mtx);
    std::cout << "into func2" << std::endl;
}

void func(std::mutex& m) {
    std::unique_lock<std::mutex> lk(m);
    /* do some work */
    std::cout << "func into" << std::endl;
    func(m);  // 递归调用(或调用其他函数)并再次锁定互斥量
}

void f() {
    std::cout << std::this_thread::get_id() << std::endl;
    func(_mtx);
    std::cout << "out of func" << std::endl;
}

int main() {
    std::thread threadF(&f);
    threadF.join();
    return 0;
}