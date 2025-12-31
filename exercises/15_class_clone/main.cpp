#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>

class DynFibonacci {
    size_t *cache;
    int cached;   // 已经计算并存入 cache 的最大索引（inclusive）
    int capacity; // 缓存容量（最大可计算的索引范围是 0 .. capacity-1）

public:
    // 实现动态设置容量的构造器
    DynFibonacci(int capacity)
        : cache(new size_t[capacity]), 
          // 初始时我们至少准备好 fibonacci(0)=0，若 capacity>1 则也准备好 fibonacci(1)=1
          cached(capacity > 1 ? 1 : 0),
          capacity(capacity)
    {
        // 解释：new size_t[capacity] 在堆上分配数组用于缓存。
        // cached 表示当前已填写到哪个索引（包含），控制后续计算起点以避免重复计算。
        ASSERT(capacity > 0, "capacity must be positive");
        cache[0] = 0;
        if (capacity > 1) cache[1] = 1;
    }

    // 实现复制构造器（深拷贝）
    DynFibonacci(DynFibonacci const &other)
        : cache(new size_t[other.capacity]),
          cached(other.cached),
          capacity(other.capacity)
    {
        // 解释：默认的浅拷贝会导致两个对象共享同一块内存，析构时会 double-free。
        // 因此复制构造函数必须分配新的数组并拷贝内容（深拷贝）。
        for (int i = 0; i <= cached; ++i) {
            cache[i] = other.cache[i];
        }
    }

    // 实现析构器，释放缓存空间
    ~DynFibonacci() {
        // 解释：配对 new[] 必须用 delete[] 释放，避免内存泄漏。
        delete[] cache;
    }

    // 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        // 边界检查
        ASSERT(i >= 0 && i < capacity, "i out of range");
        // 若请求的索引尚未计算，则从 cached+1 继续计算到 i（利用已缓存的值）
        for (int idx = cached + 1; idx <= i; ++idx) {
            // 解释：每次只计算新的项并记录到 cache 中，从而避免重复计算整棵递归树。
            cache[idx] = cache[idx - 1] + cache[idx - 2];
        }
        // 更新 cached 到最新计算的位置
        if (i > cached) cached = i;
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = fib;
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}
