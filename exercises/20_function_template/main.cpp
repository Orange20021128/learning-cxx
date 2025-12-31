#include "../exercise.h"
#include <cmath>

// READ: 函数模板 <https://zh.cppreference.com/w/cpp/language/function_template>
// TODO: 将这个函数模板化
// 解释：将函数模板化可以让同一份代码适用于不同的数值类型（int, unsigned, float, double 等）。
//       模板在实例化时根据参数类型生成对应的函数版本，避免重复编写同样的逻辑。
template<typename T>
T plus(T a, T b) {
    return a + b;
}

int main(int argc, char **argv) {
    ASSERT(plus(1, 2) == 3, "Plus two int");
    ASSERT(plus(1u, 2u) == 3u, "Plus two unsigned int");

    // THINK: 浮点数何时可以判断 ==？何时必须判断差值？
    // 解释：对浮点数直接用 == 只有在你能确知没有任何舍入误差时才安全（很少见）。
    //       通常应当比较差值（绝对/相对误差）是否在可接受的容差范围内。
    ASSERT(plus<float>(1.25f, 2.5f) == 3.75f, "Plus two float");
    ASSERT(plus<double>(1.25, 2.5) == 3.75, "Plus two double");

    // TODO: 修改判断条件使测试通过
    // 解释：0.1 和 0.2 在二进制浮点表示中不能被精确表示，0.1 + 0.2 会得出一个非常接近但不等于 0.3 的值。
    //       因此我们用一个小的容差（epsilon）来判断它们是否“足够接近”：
    ASSERT(std::abs(plus<double>(0.1, 0.2) - 0.3) < 1e-15, "How to make this pass?");

    return 0;
}
