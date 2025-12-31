#include "../exercise.h"
#include <string>

// READ: 字符串 <https://zh.cppreference.com/w/cpp/string/basic_string>

int main(int argc, char **argv) {
    // READ: 字符串字面量 <https://zh.cppreference.com/w/cpp/string/basic_string/operator%22%22s>
    using namespace std::string_literals;
    auto hello = "Hello"s;
    auto world = "world";
    // READ: `decltype` 表达式 <https://zh.cppreference.com/w/cpp/language/decltype>
    // READ: `std::is_same_v` 元编程判别 <https://zh.cppreference.com/w/cpp/types/is_same>
    ASSERT((std::is_same_v<decltype(hello), std::string>), "Fill in the missing type."); // hello 使用了 "s" 字面量后缀，类型为 std::string（即 std::basic_string<char>）
    ASSERT((std::is_same_v<decltype(world), const char *>), "Fill in the missing type."); // world 用 auto 推导自字符串字面量，字面量最初是 const char[6]，被 decays 为 const char*
    // TODO: 将 `?` 替换为正确的字符串
    ASSERT(hello + ", " + world + '!' == "Hello, world!", "Fill in the missing string."); // 连接结果为 "Hello, world!"：std::string + const char* 返回 std::string，再加上 char 也会生成 std::string
    return 0;
}
