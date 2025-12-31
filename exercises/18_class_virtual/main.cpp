#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    ASSERT(a.virtual_name() == 'A', MSG); // a 是 A 对象，虚函数按动态类型 => 'A'
    ASSERT(b.virtual_name() == 'B', MSG); // b 是 B 对象，虚函数被 B 覆盖 => 'B'
    ASSERT(c.virtual_name() == 'C', MSG); // c 是 C 对象，虚函数被 C 覆盖 => 'C'
    ASSERT(d.virtual_name() == 'C', MSG); // d 是 D 对象，但 C 已 final 并实现为 'C'，D 继承该实现 => 'C'
    ASSERT(a.direct_name() == 'A', MSG);  // non-virtual，按静态/对象类型解析 => A::direct_name => 'A'
    ASSERT(b.direct_name() == 'B', MSG);  // 同上 => B::direct_name => 'B'
    ASSERT(c.direct_name() == 'C', MSG);  // 同上 => C::direct_name => 'C'
    ASSERT(d.direct_name() == 'D', MSG);  // 同上 => D::direct_name => 'D'

    A &rab = b;
    B &rbc = c;
    C &rcd = d;

    ASSERT(rab.virtual_name() == 'B', MSG); // 虚函数按动态类型，rab 引用的是 B 对象 => B::virtual_name => 'B'
    ASSERT(rbc.virtual_name() == 'C', MSG); // rbc 引用的是 C 对象 => C::virtual_name => 'C'
    ASSERT(rcd.virtual_name() == 'C', MSG); // rcd 引用的是 D 对象，但 D 未重写虚函数（C 为 final）=> 'C'
    ASSERT(rab.direct_name() == 'A', MSG);  // direct_name 非虚，按引用的静态类型 A& => A::direct_name => 'A'
    ASSERT(rbc.direct_name() == 'B', MSG);  // 按 B& 的静态类型 => B::direct_name => 'B'
    ASSERT(rcd.direct_name() == 'C', MSG);  // 按 C& 的静态类型 => C::direct_name => 'C'

    A &rac = c;
    B &rbd = d;

    ASSERT(rac.virtual_name() == 'C', MSG); // rac 指向 C 对象，虚调用 => C::virtual_name => 'C'
    ASSERT(rbd.virtual_name() == 'C', MSG); // rbd 指向 D 对象，但虚函数实现来自 C => 'C'
    ASSERT(rac.direct_name() == 'A', MSG);  // rac 为 A&，非虚函数按 A::direct_name => 'A'
    ASSERT(rbd.direct_name() == 'B', MSG);  // rbd 为 B&，非虚函数按 B::direct_name => 'B'

    A &rad = d;

    ASSERT(rad.virtual_name() == 'C', MSG); // 虚调用按动态类型 D，但使用 C 的实现 => 'C'
    ASSERT(rad.direct_name() == 'A', MSG);  // 非虚按 A& 的静态类型 => A::direct_name => 'A'

    return 0;
}

// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>
