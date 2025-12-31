#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // 把传入的 shape 复制到成员 shape 中，并计算总体元素数量 size。
        // 解释：这是一个 4 维张量，整体元素数量等于四个维度长度的乘积。
        // 例如 shape = [1,2,3,4] 时，size = 1*2*3*4 = 24。
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // 为了把扁平数组索引和多维索引对应起来，直接使用四重循环逐维遍历。
        // 解释：逐维遍历可以清晰地根据每一维是否需要广播来决定在 others 中取哪个下标。
        for (unsigned int i0 = 0; i0 < shape[0]; ++i0) {
            for (unsigned int i1 = 0; i1 < shape[1]; ++i1) {
                for (unsigned int i2 = 0; i2 < shape[2]; ++i2) {
                    for (unsigned int i3 = 0; i3 < shape[3]; ++i3) {
                        // 计算 this 的扁平索引
                        // 解释：按行主序（最后一维连续）把多维下标转换为线性下标。
                        unsigned int idx_this = (((i0 * shape[1] + i1) * shape[2] + i2) * shape[3]) + i3;

                        // 对 others，如果对应维度为 1 则广播（总是使用 0 下标），否则使用当前下标。
                        // 解释：单向广播的规则就是 others 在某一维为 1 时，它在那一维的值会被重复用于 this 的所有索引。
                        unsigned int j0 = (others.shape[0] == 1 ? 0u : i0);
                        unsigned int j1 = (others.shape[1] == 1 ? 0u : i1);
                        unsigned int j2 = (others.shape[2] == 1 ? 0u : i2);
                        unsigned int j3 = (others.shape[3] == 1 ? 0u : i3);

                        // 可选的安全检查（在测试用例中可省略），确保要么相等要么为1。
                        // 解释：如果 others 在某维既不是 1 也不等于 this 的长度，那就是不支持的形状组合。
                        // 这里不抛异常以保持简单，但在调试时可打开断言。
                        // ASSERT(others.shape[0] == 1 || others.shape[0] == shape[0], "Incompatible shapes");
                        // ASSERT(others.shape[1] == 1 || others.shape[1] == shape[1], "Incompatible shapes");
                        // ASSERT(others.shape[2] == 1 || others.shape[2] == shape[2], "Incompatible shapes");
                        // ASSERT(others.shape[3] == 1 || others.shape[3] == shape[3], "Incompatible shapes");

                        // 计算 others 的扁平索引并执行加法
                        unsigned int idx_others = (((j0 * others.shape[1] + j1) * others.shape[2] + j2) * others.shape[3]) + j3;
                        data[idx_this] += others.data[idx_others];
                    }
                }
            }
        }
        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
