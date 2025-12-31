#include "../exercise.h"
#include <map>

// READ: `std::map` <https://zh.cppreference.com/w/cpp/container/map>
// READ: `std::unordered_map` <https://zh.cppreference.com/w/cpp/container/unordered_map>

template<class k, class v>
bool key_exists(std::map<k, v> const &map, k const &key) {
    // 使用 find 检查元素是否存在：find 在未找到时返回 map.end()，
    // 与 operator[] 不同，find 不会在容器中创建新的元素，因此不会修改原始 map。
    return map.find(key) != map.end();
}

template<class k, class v>
void set(std::map<k, v> &map, k key, v value) {
    // 使用 operator[] 插入或更新元素：当 key 不存在时会创建新元素并赋值，
    // 当 key 已存在时会覆盖原有的 value。这里使用 operator[] 语义简单直观；
    // 如果想避免默认构造开销，可改用 map.insert_or_assign(key, std::move(value))（C++17 起）。
    map[key] = std::move(value);
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    using namespace std::string_literals;

    std::map<std::string, std::string> secrets;

    set(secrets, "hello"s, "world"s);
    ASSERT(key_exists(secrets, "hello"s), "\"hello\" shoud be in `secrets`");
    ASSERT(!key_exists(secrets, "foo"s), "\"foo\" shoud not be in `secrets`");

    set(secrets, "foo"s, "bar"s);
    set(secrets, "Infini"s, "Tensor"s);
    ASSERT(secrets["hello"] == "world", "hello -> world");
    ASSERT(secrets["foo"] == "bar", "foo -> bar");
    ASSERT(secrets["Infini"] == "Tensor", "Infini -> Tensor");

    set(secrets, "hello"s, "developer"s);
    ASSERT(secrets["hello"] == "developer", "hello -> developer");

    return 0;
}
