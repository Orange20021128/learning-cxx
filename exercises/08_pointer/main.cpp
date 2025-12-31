#include "../exercise.h"

// READ: 数组向指针退化 <https://zh.cppreference.com/w/cpp/language/array#%E6%95%B0%E7%BB%84%E5%88%B0%E6%8C%87%E9%92%88%E7%9A%84%E9%80%80%E5%8C%96>
// 解释：本题要判断从 ptr 开始，每 stride 个元素取 1 个元素，组成长度为 len 的数列是否为斐波那契数列。
// 关键点：
//  - 数组名在表达式中会退化为指针，所以可以传入 arr + k 作为起始地址。
//  - 指针加法是按元素大小进行位移，ptr + n 会移动 n 个 int（不是字节数），因此直接用 ptr + i * stride 即可访问“每 stride 个元素”。
//  - 只需检查对所有 i (0 ≤ i ≤ len-3) 是否满足 a[i+2] == a[i] + a[i+1]。
//  - 函数要求 len >= 3（由 ASSERT 保证），因此无需额外处理过短的序列。
//  - 注意整数溢出可能导致和不等于预期值，这里按题意不特别处理。
bool is_fibonacci(int *ptr, int len, int stride) {
    ASSERT(len >= 3, "`len` should be at least 3");
    // （可选）也可以断言 stride >= 1，但题目测试只使用正 stride，这里不强制：
    // ASSERT(stride >= 1, "`stride` should be at least 1");

    // 遍历每个三元组，检查 ptr[i*stride], ptr[(i+1)*stride], ptr[(i+2)*stride]
    // 是否满足：第三项等于前两项之和。
    for (int i = 0; i + 2 < len; ++i) {
        // 使用指针算术访问对应元素：
        // ptr + i * stride 会跳过 i*stride 个 int 元素（不是字节），等价于 &arr[offset]
        int a = *(ptr + i * stride);
        int b = *(ptr + (i + 1) * stride);
        int c = *(ptr + (i + 2) * stride);

        // 如果任意位置不满足斐波那契关系，则整个序列不是斐波那契序列
        if (a + b != c) {
            return false;
        }
    }

    // 全部三元组都满足则是斐波那契序列
    return true;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    int arr0[]{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55},
        arr1[]{0, 1, 2, 3, 4, 5, 6},
        arr2[]{99, 98, 4, 1, 7, 2, 11, 3, 18, 5, 29, 8, 47, 13, 76, 21, 123, 34, 199, 55, 322, 0, 0};
    // clang-format off
    ASSERT( is_fibonacci(arr0    , sizeof(arr0) / sizeof(*arr0)    , 1),         "arr0 is Fibonacci"    );
    ASSERT( is_fibonacci(arr0 + 2, sizeof(arr0) / sizeof(*arr0) - 4, 1), "part of arr0 is Fibonacci"    );
    ASSERT(!is_fibonacci(arr1    , sizeof(arr1) / sizeof(*arr1)    , 1),         "arr1 is not Fibonacci");
    ASSERT( is_fibonacci(arr1 + 1,  3                              , 1), "part of arr1 is Fibonacci"    );
    ASSERT(!is_fibonacci(arr2    , sizeof(arr2) / sizeof(*arr2)    , 1),         "arr2 is not Fibonacci");
    ASSERT( is_fibonacci(arr2 + 2, 10                              , 2), "part of arr2 is Fibonacci"    );
    ASSERT( is_fibonacci(arr2 + 3,  9                              , 2), "part of arr2 is Fibonacci"    );
    ASSERT(!is_fibonacci(arr2 + 3, 10                              , 2), "guard check"                  );
    ASSERT(!is_fibonacci(arr2 + 1, 10                              , 2), "guard check"                  );
    // clang-format on
    return 0;
}
