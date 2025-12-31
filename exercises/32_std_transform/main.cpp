#include "../exercise.h"
#include <algorithm>
#include <string>
#include <vector>

// READ: `std::transform` <https://zh.cppreference.com/w/cpp/algorithm/transform>
// READ: `std::vector::begin` <https://zh.cppreference.com/w/cpp/container/vector/begin>

int main(int argc, char **argv) {
    std::vector<int> val{8, 13, 21, 34, 55};
    // 预先分配目标容器的大小，保证 transform 写入时有足够空间，
    // 这样 ans.size() 会等于 val.size()，便于后续断言检查。
    std::vector<std::string> ans(val.size());

    // 使用 std::transform 将输入范围 [val.begin(), val.end()) 的每个元素
    // 通过给定的一元操作转换后写入到以 ans.begin() 为起点的输出区间。
    // 这里的一元操作是一个 lambda：先将元素乘以 2，然后用 std::to_string 转为字符串。
    std::transform(val.begin(), val.end(), ans.begin(), [](int x) {
        return std::to_string(x * 2);
    });

    ASSERT(ans.size() == val.size(), "ans size should be equal to val size");
    ASSERT(ans[0] == "16", "ans[0] should be 16");
    ASSERT(ans[1] == "26", "ans[1] should be 26");
    ASSERT(ans[2] == "42", "ans[2] should be 42");
    ASSERT(ans[3] == "68", "ans[3] should be 68");
    ASSERT(ans[4] == "110", "ans[4] should be 110");
    return 0;
}
