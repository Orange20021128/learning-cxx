#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;
    int capacity; // 保存当前分配的最大索引容量

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity)
        : cache(new size_t[capacity + 1]), // +1 使得能索引到 capacity（包括 0）
          cached(1),                        // 已经缓存到的最大索引（初始缓存了 0 和 1）
          capacity(capacity) {
        // 讲解：这里动态分配数组用于缓存斐波那契数列，初始填充前两个基值。
        cache[0] = 0;
        cache[1] = 1;
    }

    // 禁止拷贝（浅拷贝会导致双重释放）
    DynFibonacci(const DynFibonacci &) = delete;
    DynFibonacci &operator=(const DynFibonacci &) = delete;

    // TODO: 实现移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept
        : cache(other.cache), cached(other.cached), capacity(other.capacity) {
        // 讲解：移动构造直接窃取资源指针并把被移动对象置为“空”状态，
        // 以避免析构时重复释放（被移动对象不再持有原资源）。
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        // 讲解：移动赋值需处理自移动（例如 x = std::move(x)），
        // 自移动时直接返回，不释放或窃取自身资源。
        if (this == &other) {
            return *this;
        }

        // 释放当前资源，然后窃取源资源
        delete[] cache;

        cache = other.cache;
        cached = other.cached;
        capacity = other.capacity;

        // 把被移动对象置为无资源状态，避免重复释放
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;

        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        // 讲解：析构时释放申请的动态内存，如果是被移动后的“空”对象（cache == nullptr），delete[] 是安全的。
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        // 讲解：
        // - capacity 表示最大允许索引，必须检查越界。
        // - cached 表示当前已计算并缓存的最大索引（初始为1，因为 cache[0], cache[1] 已有值）。
        // - 当请求的 i 大于 cached 时，按递推计算并缓存中间值直到 i。
        ASSERT(i <= capacity, "i out of range");

        // 如果需要计算更多项，逐步扩展 cached（注意索引和自增的顺序）
        while (cached < i) {
            ++cached;
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
