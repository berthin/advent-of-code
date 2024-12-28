from math import ceil, log10

memo = [{} for _ in range(76)]
power_10 = [(10 ** x) for x in range(45)]

def number_of_digits(number: int) -> int:
    return int(ceil(log10(number + 1)))


def blink(number: int, times: int) -> int:
    if times == 0: return 1
    if number in memo[times]: return memo[times][number]

    n_digits = number_of_digits(number)

    if number == 0:
        answer = blink(1, times - 1)
    elif n_digits % 2 == 0:
        left = number // power_10[n_digits // 2]
        rigth = number % power_10[n_digits // 2]
        answer = blink(left, times - 1) + blink(rigth, times - 1)
    else:
        answer = blink(number * 2024, times - 1) 

    if len(memo[times]) < 1E7:
        memo[times][number] = answer
    
    return answer


if __name__ == '__main__':
    numbers = list(map(int, input().split(' ')))
    
    answer = sum(blink(number, 75) for number in numbers)

    print(answer)
    
    
    
    