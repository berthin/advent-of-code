import sys
import re

from pathlib import Path


data = Path(sys.argv[1]).read_text()

pattern_mul = re.compile('mul\((?P<n1>\d+),(?P<n2>\d+)\)')

print('Part I', sum(int(n1) * int(n2) for n1, n2 in pattern_mul.findall(data)))

pattern_part2 = re.compile("mul\((?P<n1>\d+),(?P<n2>\d+)\)|(?P<do>do\(\))|(?P<dont>don't\(\))")

enable = True
answer = 0

for n1, n2, do, dont in pattern_part2.findall(data):
    if do:
        enable = True
    elif dont:
        enable = False
    elif enable:
        answer += int(n1) * int(n2)

print('Part II', answer)
