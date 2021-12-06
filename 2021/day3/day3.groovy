input = new File("small.in")
// input = new File("basic.in")

binaryNumbers = input.readLines()

// Part I
countOnes = []
binaryNumbers.collect { it.toList() }.transpose().eachWithIndex { column, index ->
  countOnes[index] = column.sum { it == "1" ? 1 : 0 }
}

BigInteger mostOrLeastCommon(def comparison, List ones, int total) {
  new BigInteger(ones.collect { comparison(it * 2, total) ? "1" : "0" }.join(""), 2)
}

leastCommon = { x, y -> x < y }
mostCommon = { x, y -> x > y }
gammaRate = mostOrLeastCommon(leastCommon, countOnes, binaryNumbers.size())
epsilonRate = mostOrLeastCommon(mostCommon, countOnes, binaryNumbers.size())

println "PartI ${gammaRate * epsilonRate}"

// PartII

BigInteger getRating(def comparison, List numbers, boolean untie) {
  N = numbers[0].size() - 1
  (0..N).each { index ->
    if (numbers.size() == 1) return
    ones = numbers.collect { (it[index] == "1") ? 1 : 0 }.sum()
    keepOnes = (ones * 2) == numbers.size() ? untie : comparison(ones * 2, numbers.size())

    numbers = numbers.findAll { number ->
      (keepOnes && number[index] == "1") || (!keepOnes && number[index] == "0")
    }
  }
  new BigInteger(numbers[0], 2)
}

oxygenRating = getRating(mostCommon, binaryNumbers.clone(), true)
co2Rating =  getRating(leastCommon, binaryNumbers.clone(), false)

println "PartII: ${oxygenRating * co2Rating}"
