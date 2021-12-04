input = new File("small.in")
// input = new File("basic.in")

commands = input.readLines().collect { it ->
  (direction, units) = it.split(" ")
  units = units as Integer
  new Tuple(direction == "forward" ? units : 0 , ["down": 1, "up": -1].get(direction, 0) * units)
}

// Part I
horizontal = commands.sum { it[0] }
depth = commands.sum { it[1] }
println "PartI ${horizontal * depth}"

// PartII
data = commands.inject(new Tuple(0, 0)) { data, dir ->
  def (int aim, int depth) = data
  def (int forward, int lastAim) = dir

  aim += lastAim
  depth += aim * forward

  new Tuple(aim, depth)
}

println "PartII: ${horizontal * data[1]}"
