#include <array>
#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Point {
    int r, c;

    Point(): Point(-1, -1) {}
    Point(int r_, int c_): r(r_), c(c_) {}

    bool operator==(const Point& other) {
        return r == other.r && c == other.c;
    }

};

ostream& operator<<(ostream& ss, Point p) {
    ss << "Point(r=" << p.r << ", c=" << p.c << ")" ;
    return ss;
}

struct Antenna: public Point {
    char frequency;

    Antenna(int r, int c, char ch): Point(r, c), frequency(ch) 
    {}
};

struct Grid {
    vector<string> data;

    vector<Antenna> find_antennas() {
        vector<Antenna> antennas;
        for (int r = 0; r < rows(); r++) {
            for (int c = 0; c < cols(); c++) {
                if (data[r][c] != '.') {
                    antennas.push_back(Antenna(r, c, data[r][c]));
                }
            }
        }
        return antennas;
    }

    int rows() const {
        return data.size();
    }

    int cols() const {
        return data[0].size();
    }

    bool is_inside(const Point& element) {
        return 0 <= element.r && element.r < rows() &&
            0 <= element.c && element.c < cols();
    }

    int get_frequency(const Point& point) {
        return data[point.r][point.c];
    }
};

Grid read_grid() {
    string line;
    Grid grid;
    while (getline(cin, line)) {
        grid.data.push_back(line);
    }
    return grid;
}

vector<pair<Antenna, Antenna>> combine_antennas(const vector<Antenna>& sorted_antennas) {
    vector<pair<Antenna, Antenna>> answer;
    for (int i = 0; i < sorted_antennas.size(); i++) {
        for (int j = i + 1; j < sorted_antennas.size(); ++j) {
            if (sorted_antennas[i].frequency != sorted_antennas[j].frequency) {
                break;
            }
            answer.push_back(make_pair(sorted_antennas[i], sorted_antennas[j]));
        }
    }
    return answer;
}

int cmp(const Antenna& a1, const Antenna& a2) {
    return (a1.frequency < a2.frequency);
}

struct Antinode : public Point {
    Antinode(int r, int c): Point(r, c) {}

    bool operator==(const Antinode& other) const {
        return r == other.r && c == other.c;
    }
};

struct HashPoint {
    size_t operator()(const Point& p) const {
        auto h1 = hash<int>()(p.r);
        auto h2 = hash<int>()(p.c);
        return h1 ^ (h2 << 1);
    }
};

bool operator==(const Point& p1, const Point& p2) {
    return p1.r == p2.r && p1.c == p2.c;
}

pair<int, int> get_delta(const Point& p1, const Point& p2) {
    return {p1.r - p2.r, p1.c - p2.c};
}

vector<Antinode> get_antinodes(const Antenna& a1, const Antenna& a2) {
    auto [d_r, d_c] = get_delta(a2, a1);

    return {
        Antinode(a1.r - d_r, a1.c - d_c),
        Antinode(a2.r + d_r, a2.c + d_c),
    };
}

void add_points_in_line(Grid& grid, const Point& p1, const Point& p2, unordered_set<Point, HashPoint>& points) {
    auto [d_r, d_c] = get_delta(p2, p1);

    Point point = p1;
    for (int i = 0; grid.is_inside(point); point = Point(p1.r - d_r * i, p1.c - d_c * i), ++i) {
        points.insert(point);
    }

    point = p1;
    for (int i = 0; grid.is_inside(point); point = Point(p1.r + d_r * i, p1.c + d_c * i), ++i) {
        points.insert(point);
    }
}

int main(int argc, char** argv) {
    int answer = 0;

    auto grid = read_grid();
    unordered_set<Antinode, HashPoint> antinodes_part1;
    unordered_set<Point, HashPoint> antinodes_part2;

    auto antennas = grid.find_antennas();
    sort(antennas.begin(), antennas.end(), cmp);
    for (const auto& [antenna1, antenna2]: combine_antennas(antennas)) {
        for (auto antinode: get_antinodes(antenna1, antenna2)) {
            if (grid.is_inside(antinode) && grid.get_frequency(antinode) != antenna1.frequency) {
                antinodes_part1.insert(antinode);
            }
        }
        add_points_in_line(grid, antenna1, antenna2, antinodes_part2);
    }

    cout << "Part I: " << antinodes_part1.size() << endl;
    cout << "Part II: " << antinodes_part2.size() << endl;

    return 0;
}
