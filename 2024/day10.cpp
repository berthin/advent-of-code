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
#include <vector>
#include "Debug.hpp"

using namespace std;

Debug debug;

static constexpr int TRAIL_END_VALUE = 9;
static constexpr int TRAIL_HEAD_VALUE = 0;
static constexpr int UNKNOWN_VALUE = -1;
static constexpr int REACHED = -1;

struct Position {
    int r, c;

    vector<Position> move_all_directions() const {
        const auto directions = vector<Position> {
            {-1, 0},
            {+1, 0},
            {0, -1},
            {0, +1},
        };

        return directions 
            | views::transform([this](auto& direction) { return Position(r + direction.r, c + direction.c); }) 
            | ranges::to<vector>();
    }

    bool operator==(Position other) const {
        return r == other.r && c == other.c;
    }
};

ostream& operator<<(ostream& os, const Position& p) {
    os << "Point(r=" << p.r << ", c=" << p.c << ")";
    return os; 
}

struct HashPosition {
    size_t operator()(const Position& p) const {
        auto h1 = hash<int>()(p.r);
        auto h2 = hash<int>()(p.c);
        return h1 ^ (h2 << 1);
    }
};


struct TopographicMap {
    vector<vector<int>> data;

    void mark_as_reached(const Position& position) {
        data[position.r][position.c] = REACHED;
    }

    const int at(const Position& position) const {
        return data[position.r][position.c];
    }

    const int rows() const {
        return data.size();
    }

    const int cols() const {
        return data[0].size();
    }

    vector<Position> trail_heads() const {
        return views::iota(0, rows() * cols())
            | views::filter([this](int index) { return (data[index % cols()][index / cols()] == TRAIL_HEAD_VALUE); })
            | views::transform([this](int index) { return Position{index % cols(), index / cols()}; }) 
            | ranges::to<vector>();
    }

    bool is_inside(const Position& position) const {
        return 0 <= position.r && position.r < rows() && 0 <= position.c && position.c < cols();
    }

};

TopographicMap read_map() {
    TopographicMap tmap;
    
    for (auto line: views::istream<string>(cin)) {
        tmap.data.push_back(
            line 
            | views::transform([](auto ch) { return ch == '.' ? UNKNOWN_VALUE : (ch - '0'); }) 
            | ranges::to<vector>()
        );
    }

    return tmap;
}

unordered_map<Position, int, HashPosition> memo;

int count_trails(const TopographicMap& tmap, const Position& head) {
    debug << "head: " << head << "value: " << tmap.at(head) << endl;

    if (tmap.at(head) == TRAIL_END_VALUE) return 1;

    if (memo.count(head) > 0) return memo[head];

    int count = memo[head];

    count = ranges::fold_left(
        head.move_all_directions() 
            | views::filter([&](const auto& next) { return (tmap.is_inside(next) && tmap.at(next) == tmap.at(head) + 1); })
            | views::transform([&](auto next) { return count_trails(tmap, next); })
        , 0, std::plus<int>()
    );

    return memo[head] = count;
}

int count_how_many_reachable_ends(const TopographicMap& tmap, const Position& head) {
    unordered_set<Position, HashPosition> visited;
    queue<Position> q;
    q.push(head);

    int count = 0;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        if (tmap.at(current) == TRAIL_END_VALUE) count += 1;

        ranges::for_each(
            current.move_all_directions() 
                | views::filter([&](const auto& next) { return (tmap.is_inside(next) && tmap.at(next) == tmap.at(current) + 1); }),
            [&](const auto& next) {
                q.push(next);
        });
    }

    return count;
}

ostream& operator<<(ostream& os, const TopographicMap& tmap) {
    for (auto& r: tmap.data) {
        for (auto x: r) {
            if (x == UNKNOWN_VALUE) os << ".";
            else os << x;
        }
        os << endl;
    }
    return os;
}

int main() {
    TopographicMap tmap = read_map();
    debug << tmap;

    auto heads = tmap.trail_heads();

    auto [score_part1, score_part2] = ranges::fold_left(heads, make_pair(0, 0), [&](auto init_value, auto& head) {
        return make_pair(
            init_value.first + count_how_many_reachable_ends(tmap, head),
            init_value.second + count_trails(tmap, head)
        );
    });

    cout << "Part I: " << score_part1 << endl;
    cout << "Part II: " << score_part2 << endl;

    return 0;
}