import sys
import re

from pathlib import Path


data = Path(sys.argv[1]).read_text().split('\n')
data = [list(f'.{line}.') for line in data if line]

data += [['.'] * len(data[0])]
data = [data[-1]] + data

rows = len(data)
cols = len(data[0])

keyword = 'XMAS'

mv_r = [-1, -1, -1, 0, 0, 1, 1, 1]
mv_c = [-1, 0, 1, -1, 1, -1, 0, 1]

answer = 0
for r in range(1, rows - 1):
    for c in range(1, cols - 1):
        for i in range(len(mv_r)):
            word = ''.join([
                    data[r+mv_r[i]*d][c+mv_c[i]*d]
                    for d in range(len(keyword))
                    if 0 <= r+mv_r[i]*d < rows and 0 <= c+mv_c[i]*d < cols
            ])
            if word == keyword:
                answer += 1

print('Part I', answer)

import numpy as np

data = np.array(data)
answer = 0

def check(puzzle):
    word = puzzle[0, 0] + puzzle[0, 2] + puzzle[1, 1] + puzzle[2, 0] + puzzle[2, 2]
    return word in ('MSAMS', 'SSAMM', 'SMASM', 'MMASS')


for r in range(1, rows - 3):
    for c in range(1, cols - 3):
        if check(data[r:r+3, c:c+3]):
            answer += 1

print('Part II', answer)



