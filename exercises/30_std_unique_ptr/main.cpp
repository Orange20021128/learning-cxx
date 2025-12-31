#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
    }

    ~Resource() {
        RECORDS.push_back(_records);
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();
}
Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----

    std::vector<const char *> answers[]{
        {"fd" /* reset(nullptr) -> new R; forward records 'f'; drop records 'd' and destroys -> "fd" */},
        // 分析 problems[1] 中资源的生命周期：
        // reset(nullptr) -> R1 (no 'r')
        // forward(R1) records 'f' -> "f"
        // forward(...) records another 'f' -> "ff"
        // reset(...) sees ptr non-null, records 'r' on R1 -> "ffr", returns new R2, R1 destroyed -> push "ffr"
        // drop(R2) records 'd' and destroys -> push "d"
        {"ffr", "d"},
        // 分析 problems[2] 中资源的生命周期：
        // reset(nullptr) -> R1
        // reset(R1) records 'r' on R1, returns R2, R1 destroyed -> push "r"
        // drop(R2) records 'd' and destroys -> push "d"
        // reset(nullptr) -> R3
        // drop(R3) records 'd' and destroys -> push "d"
        {"r", "d", "d"},
    };

    // ---- 不要修改以下代码 ----

    for (auto i = 0; i < 3; ++i) {
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0; j < problems[i].size(); ++j) {
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    return 0;
}
