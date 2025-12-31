#include "../exercise.h" // 包含项目提供的辅助宏/函数（例如 ASSERT），相对路径引入头文件

// READ: 数组 <https://zh.cppreference.com/w/cpp/language/array>

unsigned long long arr[90]{0, 1}; // 定义长度为90的全局数组，聚合初始化：arr[0]=0, arr[1]=1，其余元素被初始化为0

unsigned long long fibonacci(int i) { // 计算第 i 个斐波那契数，返回 unsigned long long
    switch (i) {
        case 0:
            return 0; // 基例：F(0) = 0
        case 1:
            return 1; // 基例：F(1) = 1
        default:
            // 三目表达式解释：
            // (i < 90 && arr[i] != 0) ? arr[i] : (arr[i] = fibonacci(i - 1) + fibonacci(i - 2))
            // 如果 i 在 arr 的范围内并且 arr[i] 已经非 0（已缓存），直接返回缓存值；
            // 否则计算 fibonacci(i-1)+fibonacci(i-2)，把结果写入 arr[i]（记忆化），然后返回该值。
            // 注意：arr[0] 初始为 0，与“未缓存”状态相同，但我们对 0 和 1 有专门的分支处理，所以不会误判。
            return (i < 90 && arr[i] != 0) ? arr[i] : (arr[i] = fibonacci(i - 1) + fibonacci(i - 2));
    }
}

int main(int argc, char **argv) {
    // 验证 arr 的内存大小等于 90 个 unsigned long long 的大小总和
    ASSERT(sizeof(arr) == 90 * sizeof(unsigned long long), "sizeof array is size of all its elements");
    // ---- 不要修改以下代码 ----
    ASSERT(fibonacci(2) == 1, "fibonacci(2) should be 1"); // F(2)=1，测试基本递归与记忆化功能
    ASSERT(fibonacci(20) == 6765, "fibonacci(20) should be 6765"); // 中等索引测试
    ASSERT(fibonacci(80) == 23416728348467685, "fibonacci(80) should be 23416728348467685"); // 大索引测试，检验缓存避免爆炸级别递归
    return 0; // 程序正常结束
}
