#include <array>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>
#include <unordered_map>
#include <string>
#include <cassert>
#include <sstream>
#include <iostream>
#include <queue>
#include <cstdint>
#include <set>
#include <map>
#include <cmath>
#include <unordered_set>
#include <algorithm>
#include <numeric>

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
        auto directions = vector<Position> {
            {-1, 0},
            {+1, 0},
            {0, -1},
            {0, +1},
        };

        vector<Position> all_directions;
        for (auto& move: directions) {
            all_directions.push_back({r + move.r, c + move.c });
        }
        return all_directions;
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

struct Bound {
    int lower;
    int upper;

    bool check(const int& value) const {
        return lower <= value && value < upper;
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
        vector<Position> heads;
        for (int r = 0; r < rows(); ++r) {
            for (int c = 0; c < cols(); ++c) {
                if (data[r][c] == TRAIL_HEAD_VALUE) heads.push_back({r, c});
            }
        }
        return heads;
    }

    bool is_inside(const Position& position) const {
        return 0 <= position.r && position.r < rows() && 0 <= position.c && position.c < cols();
    }

};

TopographicMap read_map() {
    string line;
    TopographicMap tmap;
    while (cin >> line) {
        vector<int> row;
        for (char ch: line) {
            if (ch == '.') row.push_back(UNKNOWN_VALUE);
            else row.push_back(ch - '0');
        }
        tmap.data.push_back(row);
    }
    return tmap;
}

unordered_map<Position, int, HashPosition> memo;

int count_trails(const TopographicMap& tmap, const Position& head) {
    debug << "head: " << head << "value: " << tmap.at(head) << endl;

    if (tmap.at(head) == TRAIL_END_VALUE) return 1;

    if (memo.count(head) > 0) return memo[head];

    int count = memo[head];

    for (auto& next: head.move_all_directions()) {
        if (tmap.is_inside(next) && tmap.at(next) == tmap.at(head) + 1) {
            count += count_trails(tmap, next);
        }
    }

    memo[head] = count;
    return count;
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

        if (tmap.at(current) == TRAIL_END_VALUE) {
            count += 1;
        }

        for (auto& next: current.move_all_directions()) {
            if (tmap.is_inside(next) && tmap.at(next) == tmap.at(current) + 1) {
                q.push(next);
            }
        }
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

    auto [score_part1, score_part2] = accumulate(heads.begin(), heads.end(), make_pair(0, 0), [&](auto init_value, auto& head) {
        return make_pair(
            init_value.first + count_how_many_reachable_ends(tmap, head),
            init_value.second + count_trails(tmap, head)
        );
    });

    cout << "Part I: " << score_part1 << endl;
    cout << "Part II: " << score_part2 << endl;

    return 0;
}