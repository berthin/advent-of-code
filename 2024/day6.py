import sys
import re

from pathlib import Path
from functools import lru_cache


data = Path(sys.argv[1]).read_text()

map_ = [list(line) for line in data.split('\n') if line]



mv_r = [-1, 0, 1, 0]
mv_c = [0, 1, 0, -1]


def get_direction(symbol):
    return ['^', '>', 'v', '<'].index(symbol)


def get_dimensions(map_):
    return len(map_), len(map_[0])

DIMENSIONS = get_dimensions(map_)


def find_starting_position(map_):
    dim = DIMENSIONS
    for r in range(dim[0]):
        for c in range(dim[1]):
            if map_[r][c] in {'.', '#'}:
                continue
            return (r, c), get_direction(map_[r][c])

def move(position, direction):
    return position[0] + mv_r[direction], position[1] + mv_c[direction]


def inside_map(position, map_):
    dim = DIMENSIONS
    return 0 <= position[0] < dim[0] and 0 <= position[1] < dim[1]


def is_next_position_blocked(position, map_):
    return map_[position[0]][position[1]] == '#'


visited = set()
visited_with_direction = set()

def mark_visited(position, direction):
    visited.add(position)
    visited_with_direction.add((position[0], position[1], direction))


def is_stuck(position, direction):
    return (position[0], position[1], direction) in visited_with_direction


def count_visited():
    return len(visited)


def rotate(direction):
    return (direction + 1) % 4


INITIAL = find_starting_position(map_)


def part_1(map_):
    position, direction = INITIAL

    while True:
        if is_stuck(position, direction):
            return -1
        mark_visited(position, direction)

        next_position = move(position, direction)
        if not inside_map(next_position, map_):
            break

        if not is_next_position_blocked(next_position, map_):
            position = next_position
        else:
            direction = rotate(direction)

    return count_visited()


print('Part I', part_1(map_))


def is_infinite_loop(map_):
    visited.clear()
    visited_with_direction.clear()

    return part_1(map_) == -1


def count_all_different_blocks(map_):
    count = 0
    dim = DIMENSIONS
    visited_path = [(r, c) for r, c in visited]
    for r, c in visited_path:
        if map_[r][c] == '.':
            map_[r][c] = '#'
            if is_infinite_loop(map_):
                count += 1
            map_[r][c] = '.'
    return count



print('Part II', count_all_different_blocks(map_))
