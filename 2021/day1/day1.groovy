input = new File("small.in")

depths = input.readlines().collect { it as Integer }

// Part I
answer = [
  depths.subList(1, depths.size()),
  depths.subList(0, depths.size() -1 ),
].transpose().count { it[0] > it[1] }

println "PartI: $answer"
