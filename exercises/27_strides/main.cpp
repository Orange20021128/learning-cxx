#include "../exercise.h"
#include <vector>

// 张量即多维数组。连续存储张量即逻辑结构与存储结构一致的张量。
// 通常来说，形状为 [d0, d1, ..., dn] 的张量，第 n 维是 dn 个连续的元素，第 n-1 维是 dn-1 个连续的 dn 个元素，以此类推。
// 张量的步长或跨度指的是张量每个维度上坐标 +1 时，数据指针跨过的范围。
// 因此，一个连续张量，其第 n 维的步长为 1，第 n-1 维的步长为 dn，第 n-2 维的步长为 dn*dn-1，以此类推。
// 例如，一个 2x3x4 张量，其步长为 [12, 4, 1]。

// READ: 类型别名 <https://zh.cppreference.com/w/cpp/language/type_alias>
using udim = unsigned int;

/// @brief 计算连续存储张量的步长
/// @param shape 张量的形状
/// @return 张量每维度的访问步长
std::vector<udim> strides(std::vector<udim> const &shape) {
    std::vector<udim> strides(shape.size());
    // 解释（加入到注释中）：
    // 逻辑：对于形状 [d0, d1, ..., d_{n-1}]，
    //   stride[i] = product_{j = i+1 .. n-1} d_j
    // 特别地，最后一维的 stride[n-1] = 1。
    // 为了高效计算，可以从后向前遍历，用一个 running（后缀积）保存当前已乘过的维度积：
    //   初始 running = 1
    //   从后向前：stride[i] = running; running *= shape[i];
    // 这样每个维度只参与一次乘法，时间复杂度 O(n)，空间复杂度 O(n)（返回的 strides）。
    //
    // 举例：shape = {2,3,4}
    //   running=1 -> strides[2]=1, running*=4 -> running=4
    //   strides[1]=4, running*=3 -> running=12
    //   strides[0]=12
    //
    // 下面用逆向迭代器把思路直接写成代码。
    udim running = 1;
    // strides.rbegin() 与 shape.rbegin() 同步向前（其实是逻辑上的向后遍历）
    auto it_s = strides.rbegin();
    auto it_shape = shape.rbegin();
    for (; it_s != strides.rend() && it_shape != shape.rend();
         ++it_s, ++it_shape) {
        *it_s = running;      // 当前维度的步长等于后缀积
        running *= *it_shape; // 把当前维度大小累乘到后缀积，供更高维使用
    }
    return strides;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    ASSERT((strides({2, 3, 4}) == std::vector<udim>{12, 4, 1}), "Make this assertion pass.");
    ASSERT((strides({3, 4, 5}) == std::vector<udim>{20, 5, 1}), "Make this assertion pass.");
    ASSERT((strides({1, 3, 224, 224}) == std::vector<udim>{150528, 50176, 224, 1}), "Make this assertion pass.");
    ASSERT((strides({7, 1, 1, 1, 5}) == std::vector<udim>{5, 5, 5, 5, 1}), "Make this assertion pass.");
    return 0;
}
