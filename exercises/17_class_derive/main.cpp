#include "../exercise.h"

// READ: 派生类 <https://zh.cppreference.com/w/cpp/language/derived_class>

static int i = 0;

struct X {
    int x;

    X(int x_) : x(x_) {
        std::cout << ++i << ". " << "X(" << x << ')' << std::endl;
    }
    X(X const &other) : x(other.x) {
        std::cout << ++i << ". " << "X(X const &) : x(" << x << ')' << std::endl;
    }
    ~X() {
        std::cout << ++i << ". " << "~X(" << x << ')' << std::endl;
    }
};
struct A {
    int a;

    A(int a_) : a(a_) {
        std::cout << ++i << ". " << "A(" << a << ')' << std::endl;
    }
    A(A const &other) : a(other.a) {
        std::cout << ++i << ". " << "A(A const &) : a(" << a << ')' << std::endl;
    }
    ~A() {
        std::cout << ++i << ". " << "~A(" << a << ')' << std::endl;
    }
};
struct B : public A {
    X x;

    B(int b) : A(1), x(b) {
        std::cout << ++i << ". " << "B(" << a << ", X(" << x.x << "))" << std::endl;
    }
    B(B const &other) : A(other.a), x(other.x) {
        std::cout << ++i << ". " << "B(B const &) : A(" << a << "), x(X(" << x.x << "))" << std::endl;
    }
    ~B() {
        std::cout << ++i << ". " << "~B(" << a << ", X(" << x.x << "))" << std::endl;
    }
};

int main(int argc, char **argv) {
    X x = X(1);
    A a = A(2);
    B b = B(3);

    // TODO: 补全三个类型的大小
    // sizeof(int) 在常见平台上为 4 字节，所以：
    // - X 只含有一个 int -> sizeof(X) == 4
    // - A 只含有一个 int -> sizeof(A) == 4
    // - B 是由 A (一个 int) + X (一个 int) 组成，通常没有额外填充 -> sizeof(B) == 8
    // 注意：严格来说 sizeof 值依赖于平台与对齐规则，但在常见 32/64 位实现上为 4 和 8。
    static_assert(sizeof(X) == 4, "There is an int in X");
    static_assert(sizeof(A) == 4, "There is an int in A");
    static_assert(sizeof(B) == 8, "B is an A with an X");

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    // 这是不可能的，A 无法提供 B 增加的成员变量的值
    // B ba = A(4);

    // 这也是不可能的，因为 A 是 B 的一部分，就好像不可以把套娃的外层放进内层里。
    A ab = B(5);// 然而这个代码可以编译和运行！
    // 说明（加入到注释中）：
    // 1) 先临时构造一个 B(5)：会依次构造 B 的 A 子对象 A(1)、成员 X(5)，然后打印 B(...)。
    // 2) 用这个临时 B 的 A 子对象来调用 A 的拷贝构造函数，初始化 ab（这一步只复制 A 中的成员 a）。
    // 3) 临时的 B 被销毁：先执行 ~B（打印 ~B(...)），然后销毁 B 的成员 X（~X），最后销毁其基类 A（~A）。
    // 结果是：ab 只是一个 A 的对象，B 特有的成员（如 X）被“切掉”了（object slicing）。
    // 是否“安全”？从内存安全角度不会产生悬指针之类的问题（因为复制的是子对象），但会丢失派生类的信息与状态，通常不是想要的行为。
    // NOTICE: 真实场景中不太可能出现这样的代码

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    return 0;
}
