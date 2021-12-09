// input = new File("basic.in").readLines()
input = new File("small.in").readLines()

positions = input[0].split(",").collect { it as int }.sort()
N = positions.size()

// Part I
fuel = (0..N - 1).collect { k ->
  l = positions[k]
  c1 = positions[0..k].sum()
  c2 = k + 1 < N ? positions[k + 1..N-1].sum() : 0
  2 * l * k + 2 * l - l * N - c1 + c2
}.min()

println "PartI $fuel"

// PartII
distance = { x, y -> Math.abs(y - x) * (Math.abs(y - x) + 1) / 2 }
f = { l -> positions.collect { distance(it, l) }.sum() }

left = positions[0]
right = positions[-1]

while (Math.abs(left - right) > 1E-8) {
  left_third = left + (right - left) / 3
  right_third = right - (right - left) / 3

  if (f(left_third) > f(right_third)) {
    left = left_third
  } else {
    right = right_third
  }
}

fuel = f(left.round()) as int
println "PartII $fuel"
