#include "../exercise.h"

// READ: 有 cv 限定符的成员函数 <https://zh.cppreference.com/w/cpp/language/member_functions>

struct Fibonacci {
    int numbers[11];
    // TODO: 修改方法签名和实现，使测试通过
    // 解释：
    // 1) FIB 在 main 中被声明为 constexpr（也就是常量对象），因此只能调用 const 成员函数。
    //    如果成员函数不是 const，不能在 const/constexpr 对象上调用。
    // 2) 将函数标记为 constexpr 可以让它在编译期求值（当需要在常量表达式中使用时有用），
    //    同时配合 const 满足对常量对象的调用要求。
    // 3) 函数体简单返回数组元素，直接返回 numbers[i] 即可。
    constexpr int get(int i) const {
        return numbers[i];
    }
};

int main(int argc, char **argv) {
    Fibonacci constexpr FIB{{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55}};
    ASSERT(FIB.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << FIB.get(10) << std::endl;
    return 0;
}
