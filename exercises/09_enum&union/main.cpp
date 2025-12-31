#include "../exercise.h"

// READ: 枚举类型 <https://zh.cppreference.com/w/cpp/language/enum>

// `enum` 是 C 的兼容类型，本质上其对应类型的常量。
// 在 `enum` 中定义标识符等价于定义 constexpr 常量，
// 这些标识符不需要前缀，可以直接引用。
// 因此 `enum` 定义会污染命名空间。
enum ColorEnum : unsigned char {
    COLOR_RED = 31,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
};

// 有作用域枚举型是 C++ 引入的类型安全枚举。
// 其内部标识符需要带前缀引用，如 `Color::Red`。
// 作用域枚举型可以避免命名空间污染，并提供类型安全保证。
enum class Color : int {
    Red = COLOR_RED,
    Green,
    Yellow,
    Blue,
};

ColorEnum convert_by_pun(Color c) {
    // READ: <https://zh.cppreference.com/w/cpp/language/union>
    // `union` 表示在同一内存位置存储的不同类型的值。
    // 其常见用法是实现类型双关转换，即将一种类型的值转换为另一种无关类型的值。
    // 但这种写法实际上仅在 C 语言良定义，在 C++ 中是未定义行为。
    // 这是比较少见的 C++ 不与 C 保持兼容的特性。
    // READ: 类型双关 <https://tttapa.github.io/Pages/Programming/Cpp/Practices/type-punning.html>
    union TypePun {
        ColorEnum e;
        Color c;
    };

    TypePun pun;
    // 类型双关转换（填写处）
    // 下面的写法：先写入 pun.c 再读取 pun.e，
    // 利用 union 在同一内存位置存储不同成员的事实来重新解释位表示。
    // 注意：根据 C++ 标准，读取与上次写入不同的 union 成员是未定义行为（UB）。
    // 在实践中，对于纯值的枚举（trivially copyable）这通常按位重解释为其整数表示并得到期望结果，
    // 但不能依赖于此可移植性保证。
    //
    // 如果希望得到定义良好的、可移植的做法，请使用通过整数的显式转换，例如：
    //   return static_cast<ColorEnum>(static_cast<int>(c));
    // 这会先将有作用域枚举转为其底层整数类型，再转为目标枚举类型（定义良好）。
    //
    // 这里为了完成练习并与原意对齐，使用类型双关（尽管是 UB）。
    pun.c = c;

    return pun.e;
}

int main(int argc, char **argv) {
    ASSERT(convert_by_pun(Color::Red) == COLOR_RED, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Green) == COLOR_GREEN, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Yellow) == COLOR_YELLOW, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Blue) == COLOR_BLUE, "Type punning conversion");
    return 0;
}
