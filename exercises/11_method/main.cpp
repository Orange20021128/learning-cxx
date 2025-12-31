#include "../exercise.h"
#include <iostream>

struct Fibonacci {
    unsigned long long cache[128];
    int cached;

    // 在结构体构造函数中初始化缓存。
    // 解释：斐波那契序列的前两项是 0 和 1，把它们放入缓存并把 cached 设为 2，
    // 表示当前已有两个已知项，后续请求只需从 cached 开始计算到所需下标，避免重复计算。
    Fibonacci() {
        cache[0] = 0;
        cache[1] = 1;
        cached = 2;
    }

    // 实现正确的缓存优化斐波那契计算
    // 解释：如果请求的下标 i 已在缓存内（i < cached），直接返回缓存值；
    // 否则从 cached 开始顺序计算并填充 cache，直到计算出 cache[i]，然后更新 cached。
    unsigned long long get(int i) {
        if (i < 0 || i >= 128) {
            // 非法下标不应发生于本练习，用断言或返回 0 都可；这里用断言便于发现错误。
            ASSERT(false, "index out of range");
            return 0;
        }
        if (i < cached) return cache[i];
        for (int k = cached; k <= i; ++k) {
            cache[k] = cache[k - 1] + cache[k - 2];
        }
        cached = i + 1;
        return cache[i];
    }
};

int main(int argc, char **argv) {
    // 已由 Fibonacci() 构造函数初始化缓存（cache[0]=0, cache[1]=1, cached=2）
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
