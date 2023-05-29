#include "utils.h"
#include <random>

std::vector<std::string> strsplit(const std::string &s, const std::string &delim) {
    std::vector<std::string> tokens;
    std::string token;
    size_t last = 0, next = 0;
    while ((next = s.find(delim, last)) != std::string::npos) {
        token = s.substr(last, next - last);
        tokens.push_back(token);
        last = next + 1;
    }
    tokens.push_back(s.substr(last));

    return tokens;
}

std::string get_uuid() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

    std::string res;
    for (int i = 0; i < 16; i++) {
        if (dash[i])
            res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}
