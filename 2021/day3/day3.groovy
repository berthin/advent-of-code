// input = new File("small.in")
input = new File("basic.in")

binaryNumbers = input.readLines()

// Part I
countOnes = []
binaryNumbers.collect { it.toList() }.transpose().eachWithIndex { column, index ->
  countOnes[index] = column.sum { it == "1" ? 1 : 0 }
}

BigInteger mostOrLeastCommon(def comparison, List ones, int total) {
  new BigInteger(ones.collect { comparison(it * 2, total) ? "1" : "0" }.join(""), 2)
}

gammaRate = mostOrLeastCommon( { x, y -> x > y }, countOnes, binaryNumbers.size())
epsilonRate = mostOrLeastCommon( { x, y -> x < y }, countOnes, binaryNumbers.size())

println "PartI ${gammaRate * epsilonRate}"
