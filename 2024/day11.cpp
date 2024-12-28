// Use C++23 to compile the code (clang++ min version 18)
// clang++ -std=c++23 -stdlib=libc++ day10.cpp -O3

#include <algorithm>
#include <array>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <queue>
#include <ranges>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include "Debug.hpp"
#include <functional>

using namespace std;

Debug debug;

typedef list<uint64_t> Stones;

struct Rule {
    function<bool(Stones::iterator)> check;
    function<void(Stones& stones, int index, Stones::iterator it)> apply;

    bool match(Stones::iterator it) {
        return check(it);
    }
};

struct Arrangement {
    Stones stones;
    vector<Rule> rules;

    void add_rule(const Rule& rule) {
        rules.push_back(rule);
    }

    void blink() {
        auto it = stones.begin();
        for (int index = 0; it != stones.end(); ++it, ++index ) {
            // auto rule = find_if(rules, bind_front(&Rule::match, index));
            auto rule = ranges::find_if(rules, [it](auto& rule) { return rule.match(it); });
            rule->apply(stones, index, it);
        }
    }
};

Arrangement read_arrangement() {
    Arrangement arrangement;
    arrangement.stones = ranges::istream_view<uint64_t>(cin) | ranges::to<list>();
    return arrangement;
}

int number_of_digits(auto number) {
    return int(ceil(log10(number + 1)));
}

static auto power_10 = views::iota(0, 20) 
    | views::transform([](auto x) { return uint64_t(pow(uint64_t(10), x)); }) 
    | ranges::to<vector>();

typedef uint64_t u64; 
unordered_map<u64, u64> memo[76];

uint64_t blink(u64 number, int times) {
    if (times == 0) return 1;
    if (memo[times].count(number) != 0) return memo[times][number];

    u64 answer = 0;
    int n_digits = number_of_digits(number);

    if (number == 0) {
        answer = blink(1, times - 1);
    } else if (n_digits % 2 == 0) {
        u64 left = number / power_10[n_digits / 2];
        u64 right = number % power_10[n_digits / 2];
        answer = blink(left, times - 1) + blink(right, times - 1);
    } else {
        answer = blink(number * 2024, times - 1);
    }

    if (memo[times].size() < 1E6) memo[times][number] = answer;
    return answer;
}

int main() {
    Rule engraved_with_0(
        [](Stones::iterator it) { return *it == 0; },
        [](Stones& stones, int index, Stones::iterator it) {
            (*it) = 1;
        }
    );

    Rule engraved_with_even(
        [](Stones::iterator it) { return number_of_digits(*it) % 2 == 0; },
        [](Stones& stones, int index, Stones::iterator it) {
            int half = number_of_digits(*it) / 2;
            stones.insert(it, {(*it) / power_10[half]});
            (*it) = (*it) % power_10[half];
        }
    );
    Rule none_of_the_above(
        [](Stones::iterator) { return true; },
        [](Stones& stones, int index, Stones::iterator it) {
            (*it) *= 2024;
        }
    );

    Arrangement arrangement = read_arrangement();
    arrangement.add_rule(engraved_with_0);
    arrangement.add_rule(engraved_with_even);
    arrangement.add_rule(none_of_the_above);

    // Part I
    // for (auto iteration: views::iota(0, 25)) arrangement.blink();
    // cout << "Part I: " << arrangement.stones.size() << endl;

    uint64_t answer = 0;
    for (u64 number: arrangement.stones) {
        answer += blink(number, 75);
    }

    cout << "Part II: " << answer << endl;

    return 0;
}