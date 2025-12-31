#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 4, ""); // 4: shared 本身 + ptrs 中的 3 个拷贝，共 4 个 shared_ptr 拥有者（weak_ptr 不增加计数）

    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, ""); // 3: 释放了 ptrs[0]，剩下 shared + ptrs[1] + ptrs[2]

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, ""); // 2: 释放了 ptrs[1]，剩下 shared + ptrs[2]

    ptrs[2] = std::make_shared<int>(*shared);
    ASSERT(observer.use_count() == 1, ""); // 1: ptrs[2] 被赋予了一个新的控制块（不同的对象），原始控制块只有 shared 一个拥有者

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    ASSERT(observer.use_count() == 3, ""); // 3: 现在原始控制块被 ptrs[0], ptrs[1], ptrs[2] 拥有（shared 在 move 后变为 nullptr）

    // clear ptrs[0] explicitly (std::ignore = std::move(ptrs[0]) does not clear the source reliably)
    ptrs[0].reset();
    // remove the redundant self-move which has no effect
    // move ownership from ptrs[2] to ptrs[1]
    ptrs[1] = std::move(ptrs[2]);
    ASSERT(observer.use_count() == 1, ""); // 1: ptrs[0] cleared, ptrs[2] moved -> 最终只有 ptrs[1] 一个拥有者

    shared = observer.lock();
    ASSERT(observer.use_count() == 2, ""); // 2: lock() 创建一个新的 shared_ptr，拥有者变为 ptrs[1] + shared

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, ""); // 0: 清空所有 shared_ptr，原始控制块没有拥有者

    shared = observer.lock();
    ASSERT(observer.use_count() == 0, ""); // 0: observer 已过期，lock() 返回空 shared_ptr，计数仍然为 0

    return 0;
}
