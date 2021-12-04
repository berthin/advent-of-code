input = new File("small.in")
//input = new File("basic.in")

depths = input.readLines().collect { it as Integer }

// Part I
int countLargerNumbers(List numbers) {
  [
    numbers.subList(1, numbers.size()),
    numbers.subList(0, numbers.size() -1 ),
  ].transpose().count { it[0] > it[1] }
}

println "PartI: ${countLargerNumbers(depths)}"

// Part II
slidingWindows = (0..depths.size() - 3).collect { depths.subList(it, it + 3).sum() }
println "PartII: ${countLargerNumbers(slidingWindows)}"
