#include "../exercise.h"
#include <numeric>
#include <cstddef>

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float;
    int shape[]{1, 3, 224, 224};

    // 解释：
    // 1) 张量占用的字节数 = 每个元素的字节数（sizeof(DataType)） * 元素总数（各维度的乘积）。
    // 2) 使用 std::accumulate 做折叠（fold）：以 sizeof(DataType) 作为初始值，然后依次乘以每个维度。
    // 3) 用 std::size_t 做累乘以避免中间乘法溢出（更通用、安全）。
    auto total_bytes = std::accumulate(
        std::begin(shape), std::end(shape),
        static_cast<std::size_t>(sizeof(DataType)),
        [](std::size_t acc, int dim) {
            return acc * static_cast<std::size_t>(dim);
        }
    );

    // 这里 ASSERT 用的是 int，比对常数 602112 没问题；将结果强转为 int。
    int size = static_cast<int>(total_bytes);

    // 验证：4（float 字节数）x1x3x224x224 = 602112
    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    return 0;
}
