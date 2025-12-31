#include "../exercise.h"
#include <cstring>
#include <vector>

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        ASSERT(vec.size() == 5, "Fill in the correct value."); // 大小是元素个数
        ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // vector 对象通常包含三个指针（begin/end/capacity），x86_64 下为 3 * 8 = 24 字节
        int ans[]{1, 2, 3, 4, 5};
        ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values."); // data() 返回内部连续缓冲区指针
    }
    {
        std::vector<double> vec{1, 2, 3, 4, 5};
        {
            ASSERT(vec.size() == 5, "Fill in the correct value."); // 初始有 5 个元素
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 对象大小与上面相同（实现细节）
            double ans[]{1, 2, 3, 4, 5};
            ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values."); // data() 可与原数组比较字节
        }
        {
            vec.push_back(6);
            ASSERT(vec.size() == 6, "Fill in the correct value."); // push_back 增加元素数量
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 对象大小不随元素数量变化
            vec.pop_back();
            ASSERT(vec.size() == 5, "Fill in the correct value."); // pop_back 移除最后一个元素
            ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 仍然不变
        }
        {
            vec[4] = 6;
            ASSERT(vec[0] == 1, "Fill in the correct value."); // 按索引访问，0-based
            ASSERT(vec[1] == 2, "Fill in the correct value.");
            ASSERT(vec[2] == 3, "Fill in the correct value.");
            ASSERT(vec[3] == 4, "Fill in the correct value.");
            ASSERT(vec[4] == 6, "Fill in the correct value."); // 将第 5 个元素改为 6
        }
        {
            // THINK: `std::vector` 插入删除的时间复杂度是什么？
            // 插入/删除中间位置平均为 O(n)，在末尾为 amortized O(1)
            vec.insert(vec.begin() + 1, 1.5);
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 3, 4, 6}), "Make this assertion pass.");
            vec.erase(vec.begin() + 3);
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 4, 6}), "Make this assertion pass.");
        }
        {
            vec.shrink_to_fit();
            ASSERT(vec.capacity() == 5, "Fill in the correct value."); // shrink_to_fit 尝试将 capacity 收缩到 size（此处为 5）
            vec.clear();
            ASSERT(vec.empty(), "`vec` is empty now.");
            ASSERT(vec.size() == 0, "Fill in the correct value."); // 清空后 size 为 0
            ASSERT(vec.capacity() == 5, "Fill in the correct value."); // clear 不改变 capacity，仍为之前的容量
        }
    }
    {
        std::vector<char> vec(48, 'z'); // 调用 (count, value) 构造函数，创建 48 个 'z'
        ASSERT(vec[0] == 'z', "Make this assertion pass.");
        ASSERT(vec[47] == 'z', "Make this assertion pass.");
        ASSERT(vec.size() == 48, "Make this assertion pass.");
        ASSERT(sizeof(vec) == 24, "Fill in the correct value."); // 对象大小同上
        {
            auto capacity = vec.capacity();
            vec.resize(16);
            ASSERT(vec.size() == 16, "Fill in the correct value."); // resize 缩短后 size 为 16
            ASSERT(vec.capacity() == capacity, "Fill in a correct identifier."); // 缩短不会改变 capacity，仍为之前保存的 capacity
        }
        {
            vec.reserve(256);
            ASSERT(vec.size() == 16, "Fill in the correct value."); // reserve 不改变 size
            ASSERT(vec.capacity() == 256, "Fill in the correct value."); // reserve 增加 capacity 至至少 256
        }
        {
            vec.push_back('a');
            vec.push_back('b');
            vec.push_back('c');
            vec.push_back('d');
            ASSERT(vec.size() == 20, "Fill in the correct value."); // 16 + 4 = 20
            ASSERT(vec.capacity() == 256, "Fill in the correct value."); // capacity 保持 256
            ASSERT(vec[15] == 'z', "Fill in the correct value."); // 原来索引 0..15 仍是 'z'
            ASSERT(vec[16] == 'a', "Fill in the correct value."); // 新加入元素从索引 16 开始
            ASSERT(vec[17] == 'b', "Fill in the correct value.");
            ASSERT(vec[18] == 'c', "Fill in the correct value.");
            ASSERT(vec[19] == 'd', "Fill in the correct value.");
        }
    }

    return 0;
}
