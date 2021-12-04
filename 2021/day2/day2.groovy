// input = new File("small.in")
input = new File("basic.in")

commands = input.readLines().collect { it ->
  (direction, units) = it.split(" ")
  [("${direction}" as String): (units as Integer)]
}

// Part I
int getCummulateCommandsPerDirection(def commands, String direction) {
  commands.sum { it.get(direction, 0) }
}

horizontal = getCummulateCommandsPerDirection(commands, "forward")
depth = getCummulateCommandsPerDirection(commands, "down") - getCummulateCommandsPerDirection(commands, "up")
println "PartI ${horizontal * depth}"

