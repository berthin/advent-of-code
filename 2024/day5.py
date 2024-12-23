import sys
import re

from pathlib import Path


data = Path(sys.argv[1]).read_text()

rules, updates = data.split('\n\n')

order = dict()

for rule in rules.split():
    before, after = map(int, re.findall('\d+', rule))
    if before not in order:
        order[before] = set()
    order[before].add(after)


def check_order(page, following_pages):
    for next_page in following_pages:
        if next_page in order and page in order[next_page]:
            return False
    return True


def order_is_correct(pages):
    for index, page in enumerate(pages):
        if not check_order(page, pages[index + 1:]):
            return False
    return True


def take_middle(pages):
    return pages[len(pages)//2]


def insertion_sort(pages_original):
    pages = [x for x in pages_original]
    n = len(pages)
    for i in range(n):
        stop = False
        while not stop:
            stop = True
            page = pages[i]
            for j in range(i + 1, n):
                next_page = pages[j]
                if next_page in order and page in order[next_page]:
                    pages[i], pages[j] = pages[j], pages[i]
                    stop = False

    return pages

# 3 4 2 1
# 2 4 3 1


def fix_order(pages):
    return insertion_sort(pages)


answer = 0
answer_partII = 0
for update in updates.split():
    pages = list(map(int, re.findall('\d+', update)))
    if order_is_correct(pages):
        answer += take_middle(pages)
    else:
        answer_partII += take_middle(fix_order(pages))


print('Part I', answer)
print('Part II', answer_partII)



