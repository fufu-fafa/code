#include <iostream>
#include <vector>

std::unordered_map<std::string, int> count_times_appeared(const std::vector<std::string>& in_list) {
    std::unordered_map<std::string, int> out_list;
    for (const std::string str : in_list) {
        out_list[str]++;
    }
    return out_list;
}

int main() {
    std::unordered_map<std::string, int> output;
    std::vector<std::string> in_list = {
        "var1", "var2", "var2", "var3", "var1", "var4", "var3"
    };
    output = count_times_appeared(in_list);
    std::cout << "unique strings: " << output.size() << '\n';
    for (const auto& result : output) {
        std::cout << result.first << ": " << result.second << '\n';
    }
    return 0;
}
