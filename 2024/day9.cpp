#include <array>
#include <cstdio>
#include <vector>
#include <cstring>
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

#include "Debug.hpp"

using namespace std;

constexpr int DOT = -1; 

Debug debug;

typedef vector<int> Blocks;
typedef vector<pair<int, int>> SimplifiedBlocks;  // id, count
typedef vector <priority_queue<int, vector<int>, greater<int>>> EmptySpaces; // space_count x [indexes...]

struct Disk {
    string encoded_blocks;

    Blocks decode() {
        Blocks decoded_blocks;
        int block_id = 0;
        for (int i = 0; i < encoded_blocks.size(); ++i) {
            auto digit = int(encoded_blocks[i]) - int('0');
            int value = free_block(i) ? DOT : block_id;
            add_n_blocks(decoded_blocks, value, digit);
            block_id += value != DOT;
        }
        return decoded_blocks;
    }

    SimplifiedBlocks decode_simplified() {
        SimplifiedBlocks decoded_blocks;
        int block_id = 0;
        for (int i = 0; i < encoded_blocks.size(); ++i) {
            auto digit = int(encoded_blocks[i]) - int('0');
            int value = free_block(i) ? DOT : block_id;
            decoded_blocks.push_back({value, digit});
            block_id += value != DOT;
        }
        return decoded_blocks;
    }

    bool free_block(int index) {
        return index % 2 == 1;
    }

    void add_n_blocks(Blocks& blocks, int value, int n) {
        for (int i = 0; i < n; ++i) blocks.push_back(value);
    }
};

Disk read_disk() {
    Disk disk;
    cin >> disk.encoded_blocks;
    return disk;
}

Blocks compress(const Blocks& blocks) {
    int left = 0;
    int right = blocks.size() - 1;

    Blocks compressed_blocks;
    while (true) {
        for (; blocks[left] != DOT && left < right; left++) compressed_blocks.push_back(blocks[left]);
        for (; blocks[right] == DOT && left < right; right--);
        if (left > right) break;
        if (blocks[right] == DOT) break;
        compressed_blocks.push_back(blocks[right]);
        left++;
        right--;
    }

    return compressed_blocks;
}


pair<int, int> find_empty_space(EmptySpaces& spaces, int count) {
    int low_index = -1;
    for (int i = count; i < spaces.size(); ++i) {
        if (spaces[i].empty()) continue;

        if (low_index == -1 || spaces[low_index].top() > spaces[i].top()) {
            low_index = i;
        }
    }

    if (low_index == -1) return {-1, -1};

    int index = spaces[low_index].top();
    spaces[low_index].pop();

    return {index, low_index};
}

Blocks compress(const SimplifiedBlocks& blocks) {
    int total_size = 0;
    for (auto& block: blocks) total_size += block.second;

    Blocks compressed_blocks(total_size, DOT);

    vector<int> blocks_index_lut;
    EmptySpaces empty_spaces(10); 

    int index = 0;
    for (auto [id, count]: blocks) {
        if (id == DOT) {
            empty_spaces[count].push(index);
        } else {
            assert(blocks_index_lut.size() == id);
            blocks_index_lut.push_back(index);
        }
        index += count;
    }
    debug << "total size: " << total_size << endl;

    int left = 0;
    int right = blocks.size() - 1;
    while (true) {
        for (; blocks[right].first == DOT && left < right; --right);
        if (left > right) break;

        auto [block_id, block_count] = blocks[right];
        debug << "Searching for: " << block_id << " " << block_count << endl;
        auto [index, free_count] = find_empty_space(empty_spaces, block_count);
        debug << "found index: " << index << " " << "free: " << free_count << endl;


        if (free_count < block_count || index == -1 || index > blocks_index_lut[block_id]) {
            debug << "Just copying, block_id: " << block_id << " @ index: " << blocks_index_lut[block_id] << endl;
            for (int i = 0; i < block_count; ++i) {
                compressed_blocks[i + blocks_index_lut[block_id]] = block_id;
            }
        } else {
            for (int i = 0; i < block_count; ++i) {
                compressed_blocks[i + index] = block_id;
            }
        }
        if (free_count - block_count > 0) {
            empty_spaces[free_count - block_count].push(index + block_count);
        }
        right--;
    }

    return compressed_blocks;
}

void print_blocks(const Blocks& blocks) {
    for(auto x: blocks) {
        debug << (x == DOT ? "." : to_string(x));
    }
    debug << endl;
}

int64_t checksum(const Blocks& blocks) {
    int64_t sum = 0;
    for (int id = 0; id < blocks.size(); ++id) {
        if (blocks[id] != DOT) sum += id * blocks[id];
    }
    return sum;
}

int main() {
    auto disk = read_disk();
    auto blocks = disk.decode();

    print_blocks(blocks);
    auto compressed_blocks = compress(blocks);
    print_blocks(compressed_blocks);
    
    cout << "Part I: " << checksum(compressed_blocks) << endl;

    auto simplified_blocks = disk.decode_simplified();
    debug << simplified_blocks.size() << endl;
    compressed_blocks = compress(simplified_blocks);
    print_blocks(compressed_blocks);

    cout << "Part II: " << checksum(compressed_blocks) << endl;
    return 0;
}