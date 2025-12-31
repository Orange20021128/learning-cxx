#include "../exercise.h"
#include <cmath>

enum class DataType {
    Float,
    Double,
};

/// @brief Tagged union 即标签化联合体，是联合体的一种常见应用。
///        Rust enum 在实现上就是标签化联合体。
struct TaggedUnion {
    DataType type;
    // NOTICE: struct/union 可以相互任意嵌套。
    union {
        float f;
        double d;
    };
};

// TODO: 将这个函数模板化用于 sigmoid_dyn
template<typename T>
T sigmoid(T x) {
    // 解释：模板化使得同一函数名可以作用于 float 和 double。
    // std::exp 对不同浮点类型有重载，返回对应类型；使用模板可以
    // 避免重复实现 float/double 两个版本。
    return static_cast<T>(1) / (static_cast<T>(1) + std::exp(-x));
}

TaggedUnion sigmoid_dyn(TaggedUnion x) {
    TaggedUnion ans{x.type};
    // TODO: 根据 type 调用 sigmoid
    // 解释：TaggedUnion 使用 type 标签来表明 union 中哪个字段当前有效。
    // 在运行时根据标签选择对应的分支，调用模板化的 sigmoid 并把结果
    // 存回相应的 union 字段（ans.f 或 ans.d）。这样可以在保持单一
    // 接口的同时正确处理不同的数值类型。
    switch (x.type) {
        case DataType::Float:
            ans.f = sigmoid<float>(x.f);
            break;
        case DataType::Double:
            ans.d = sigmoid<double>(x.d);
            break;
        default:
            // 不应到达；保留原标签
            break;
    }
    return ans;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    TaggedUnion xf{DataType::Float};
    xf.f = 5.f;
    auto yf = sigmoid_dyn(xf);
    ASSERT(yf.type == DataType::Float, "type mismatch");
    ASSERT(yf.f == 1 / (1 + std::exp(-5.f)), "sigmoid float");

    TaggedUnion xd{DataType::Double};
    xd.d = 5.0;
    auto yd = sigmoid_dyn(xd);
    ASSERT(yd.type == DataType::Double, "type mismatch");
    ASSERT(yd.d == 1 / (1 + std::exp(-5.0)), "sigmoid double");
    return 0;
}
