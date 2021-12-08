// input = new File("simple.in")
input = new File("small.in")

intervals = input.readLines()[0].split(",").collect { it as int }

memo = [:]

BigInteger countLanternfishFor(int initialInterval, int days) {
  if (!(initialInterval in memo)) {
    currentDay = [(initialInterval): BigInteger.ONE]
    (1..days).each {
      newDay = currentDay.collect { daysLeft, freq ->
        (daysLeft == 0) ? null : [(daysLeft - 1) : freq]
      }.findAll().collectEntries()

      fishToDie = currentDay.getOrDefault(0, 0)
      if (fishToDie > 0) {
        newDay.put(8, newDay.getOrDefault(8, 0) + fishToDie)
        newDay.put(6, newDay.getOrDefault(6, 0) + fishToDie)
      }
      currentDay = newDay
    }
    memo[initialInterval] = currentDay.values().sum()
  }
  return memo[initialInterval]
}

BigInteger countAfterNDays(int days) { intervals.sum { countLanternfishFor(it, days) } }

println "PartI: ${countAfterNDays(80)}"

memo = [:]
println "PartI: ${countAfterNDays(256)}"
