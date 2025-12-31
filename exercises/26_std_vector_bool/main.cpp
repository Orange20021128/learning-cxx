#include "../exercise.h"
#include <vector>

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector_bool>
// READ: 模板特化 <https://zh.cppreference.com/w/cpp/language/template_specialization>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    std::vector<bool> vec(100, true); // 大小100，所有元素初始化为true（因此 vec[0] 和 vec[99] 为 true）
    ASSERT(vec[0], "Make this assertion pass.");
    ASSERT(vec[99], "Make this assertion pass.");
    ASSERT(vec.size() == 100, "Make this assertion pass.");
    // NOTICE: 平台相关！注意 CI:Ubuntu 上的值。
    std::cout << "sizeof(std::vector<bool>) = " << sizeof(std::vector<bool>) << std::endl;
    ASSERT(sizeof(vec) == 32, "Fill in the correct value."); // 在当前平台上为32字节
    {
        vec[20] = false;
        ASSERT(!vec[20], "Fill in `vec[20]` or `!vec[20]`."); // 赋值为false，所以使用!vec[20]
    }
    {
        vec.push_back(false);
        ASSERT(vec.size() == 101, "Fill in the correct value."); // push_back 后大小从100变成101
        ASSERT(!vec[100], "Fill in `vec[100]` or `!vec[100]`."); // 新增元素为false
    }
    {
        auto ref = vec[30]; // auto拷贝了 vector<bool>::reference 的代理对象（该代理持有对底层位的引用）
        ASSERT(ref, "Fill in `ref` or `!ref`"); // 初始为true
        ref = false;
        ASSERT(!ref, "Fill in `ref` or `!ref`"); // 代理被赋值为false
        // THINK: WHAT and WHY?
        ASSERT(!vec[30], "Fill in `vec[30]` or `!vec[30]`."); // 代理修改会反映到底层，所以 vec[30] 现在为 false
    }
    return 0;
}
