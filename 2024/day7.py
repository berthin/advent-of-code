from pathlib import Path

import sys
import re


data = Path(sys.argv[1]).read_text()

for line in data.split('\n'):
    if not line:
        continue
    match = re.search('(\d+):(\s+\d+)+', line)
    breakpoint()

    print(goal)
    print('numbers', numbers)
