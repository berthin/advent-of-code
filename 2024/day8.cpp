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

    Point(int r_, int c_): r(r_), c(c_) {}

    bool operator==(const Point& other) {
        return r == other.r && c == other.c;
    }
};

struct Antenna: public Point {
    char frequency;
};

struct Grid {
    vector<string> data;

    vector<Antenna> find_antennas() {
        vector<Antenna> antennas;
        for (int r = 0; r < data.size(); r++) {
            for (int c = 0; c < data[r].size(); c++) {
                if (data[r][c] != '.') {
                    antennas.push_back(Antenna(r, c, data[r][c]));
                }
            }
        }
        return antennas;
    }

    bool is_inside(const Point& element) {
        return 0 <= element.r && element.r < data.size() &&
            0 <= element.c && element.c < data[0].size();
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
    return (a1.frequency == a2.frequency);
}

struct Antinode : public Point {
};

bool operator==(const Antinode& a1, const Antinode& a2) {
    return a1.r == a2.c && a1.c == a2.c;
}

vector<Antinode> get_antinodes(const Antenna& a1, const Antenna& a2) {
    int d_r = a2.r - a1.r;
    int d_c = a2.c - a2.c;

    vector<Antinode> v;
    v.push_back(Antinode(a1.r - d_r, a1.c - d_c));
    v.push_back(Antinode(a2.r + d_r, a2.c + d_c));
    return v;

    //
    //   3,5 a2
    //       5,7 a1
    // d=(-2, -2)
    // (7, 9), (-1, 3)
    //
    //   3,5 a1
    //       5,7 a2
    // d=(2, 2)
    // (-1, 3), (7, 9)
    //
    //       1,7 a2
    //   3,5 a1
    // d=(-2, 2)
    // (5, 3), (-7, 9)
}

int main(int argc, char** argv) {
    int answer = 0;

    auto grid = read_grid();
    unordered_set<Antinode> antinodes;

    auto antennas = grid.find_antennas();
    sort(antennas.begin(), antennas.end(), cmp);
    for (const auto& [antenna1, antenna2]: combine_antennas(antennas)) {
        for (auto antinode: get_antinodes(antenna1, antenna2)) {
            if (grid.is_inside(antinode)) {
                antinodes.insert(antinode);
            }
        }
    }

    answer = antinodes.size();

    cout << "Answer: " << answer << endl;

    return 0;
}
