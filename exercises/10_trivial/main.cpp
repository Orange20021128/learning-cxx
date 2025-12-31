#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

// TODO: 实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    // 如果请求的索引已经在缓存中，直接返回（避免重复计算）
    if (i < cache.cached) {
        return cache.cache[i];
    }

    // 从当前缓存长度开始，逐项计算直到第 i 项（包含）
    // 注意：cache.cached 表示已经填充了多少项，索引从 0 开始，
    // 所以当 cached==2 时，cache.cache[0] 和 cache.cache[1] 已经可用，
    // 我们应该从 k = 2 开始计算。
    for (int k = cache.cached; k <= i; ++k) {
        cache.cache[k] = cache.cache[k - 1] + cache.cache[k - 2];
    }

    // 更新已缓存的项数为 i+1（因为索引从 0 开始）
    cache.cached = i + 1;
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    //
    // 解释：
    // - 我们把 cache[0] 初始化为 0，cache[1] 初始化为 1，这是斐波那契序列的前两项；
    // - 把 cached 初始化为 2，表示前两项已准备好，后续调用 fibonacci 会从索引 2 开始计算并扩展缓存。
    // - 使用聚合初始化可以直接设置数组和字段，避免未初始化读取导致未定义行为。
    FibonacciCache fib = {{0ULL, 1ULL}, 2};

    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
