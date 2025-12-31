#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // 正确声明静态字段（定义必须放在类外）
    static int num_a;

    A() {
        ++num_a;
    }
    // 必须把基类析构函数声明为 virtual，
    // 否则通过基类指针 delete 指向派生类对象时，派生类的析构不会被调用，
    // 导致 B::num_b 无法正确递减（资源/计数泄漏）。
    virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};
struct B final : public A {
    // 正确声明静态字段（定义必须放在类外）
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

// 在类外定义静态字段并初始化
int A::num_a = 0; // 静态成员在类外定义并初始化
int B::num_b = 0; // 静态成员在类外定义并初始化

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a"); // 一个 A 对象 + 一个 B 对象(继承自 A) => A::num_a == 2
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b"); // 只有一个 B 对象 => B::num_b == 1
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()"); // a 是 A 的实例
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()"); // b 是 B 的实例，虚函数返回 B

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B;// 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a"); // 新建一个 B，会先构造 A => A::num_a == 1
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b"); // 新建一个 B => B::num_b == 1
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()"); // 多态，调用 B::name()

    // 基类指针无法随意转换为派生类引用/指针，需要运行时检查（dynamic_cast）或确保类型安全后使用 static_cast。
    // 这里用 dynamic_cast 来做安全的向下转换（要求基类有虚函数以支持 RTTI）
    B &bb = dynamic_cast<B&>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()"); // 引用绑定到实际的 B 对象

    // ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}
