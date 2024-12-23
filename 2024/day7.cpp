#include <array>
#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <cmath>

using namespace std;

/*
struct BigInteger {
    vector<int> digits;

};*/

struct Equation {
    uint64_t goal;
    vector<uint64_t> numbers;

    Equation(const string& input) {
        stringstream ss(input);

        ss >> goal;

        string remaining;
        getline(ss, remaining, ':');

        ss.clear();
        ss << remaining;
        uint64_t tmp;

        while (ss >> tmp) {
            numbers.push_back(tmp);
        }
    }

    ~Equation() = default;
};

vector<Equation> read_input() {
    vector<Equation> input;
    string line;

    while (getline(cin, line)) {
        input.push_back(Equation(line));
    }

    return input;
}

bool try_to_find_formula(uint64_t goal, const vector<uint64_t>& numbers, uint64_t current = 0, uint64_t index = 0) {
    // operations + *
    // always evaluation left 2 right
    if (current > goal) return 0;

    if (index == numbers.size()) return goal == current;
    if (index == 0) return try_to_find_formula(goal, numbers, numbers[0], index + 1);

    return try_to_find_formula(goal, numbers, current + numbers[index], index + 1) ||
        try_to_find_formula(goal, numbers, current * numbers[index], index + 1) ||
        // Part II:
        // Includes || which concatenates two numbers
        try_to_find_formula(goal, numbers, current * (pow(10, ceil(log10(numbers[index] + 1)))) + numbers[index], index + 1);
}

int main(int argc, char** argv) {
    auto equations = read_input();

    auto answer = 0ll;
    for (auto equation: equations) {
        answer += equation.goal * try_to_find_formula(equation.goal, equation.numbers);
    }

    cout << "Answer: " << answer << endl;

    return 0;
}
