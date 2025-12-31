#include "../exercise.h"
#include <array>
#include <cstring>

// READ: std::array <https://zh.cppreference.com/w/cpp/container/array>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::array<int, 5> arr{{1, 2, 3, 4, 5}};
        ASSERT(arr.size() == 5, "Fill in the correct value."); // 数组元素个数为模版参数 5
        ASSERT(sizeof(arr) == 20, "Fill in the correct value."); // sizeof(int)==4, 4*5 = 20 字节（常见平台）
        int ans[]{1, 2, 3, 4, 5};
        ASSERT(std::memcmp(arr.data(), ans, sizeof(ans)) == 0, "Fill in the correct values."); // 使用 data() 获取底层指针，比较 sizeof(ans)=5*sizeof(int)
    }
    {
        std::array<double, 8> arr;
        ASSERT(arr.size() == 8, "Fill in the correct value."); // 元素个数为模版参数 8
        ASSERT(sizeof(arr) == 64, "Fill in the correct value."); // sizeof(double)==8, 8*8 = 64 字节（常见平台）
    }
    {
        std::array<char, 21> arr{"Hello, InfiniTensor!"};
        ASSERT(arr.size() == 21, "Fill in the correct value."); // 字符串长度 20 + 结尾 '\0' = 21
        ASSERT(sizeof(arr) == 21, "Fill in the correct value."); // 每个 char 1 字节，21 个元素即 21 字节
        ASSERT(std::strcmp(arr.data(), "Hello, InfiniTensor!") == 0, "Fill in the correct value."); // data() 返回 C 字符串起始地址，用 strcmp 比较
    }
    return 0;
}
