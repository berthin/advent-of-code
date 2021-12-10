input = new File("basic.in")
// input = new File("small.in")

displays = input.readLines().collect {
  parts = it.split(/\|/)
  [
    patterns: parts[0].trim().split(" "),
    digitsToDecode: parts[1].trim().split(" "),
  ]
}

// PartI
partI = displays*.digitsToDecode.sum { digit ->
  digit.count { it.size() in [2, 3, 4, 7] }
}

println "PartI: $partI"

// Part II
digits = [
  "abcefg": 0,
  "cf": 1,
  "acdeg": 2,
  "acdfg": 3,
  "bcdf": 4,
  "abdfg": 5,
  "abdefg": 6,
  "acf": 7,
  "abcdefg": 8,
  "abcdfg": 9,
]

def decode(def order, String pattern) {
  decodedPattern = pattern.collect { "abcdefg"[order.indexOf(it)] }.sort().join("")
  digits.getOrDefault(decodedPattern, null)
}

boolean doesOrderMakeSense(def order, def patterns) {
  patterns.collect {
    decode(order, it)
  }.findAll {
    it != null
  }.unique().size() == patterns.size()
}

partII = displays.sum { it ->
  patterns = it.patterns

  order = ('a'..'g').permutations().find { digitsOrder ->
    doesOrderMakeSense(digitsOrder, patterns)
  }

  it.digitsToDecode.collect { digit -> decode(order, digit) }.join("") as int
}

println "PartII: $partII"
