input = new File("small.in")
// input = new File("basic.in")

lines = input.readLines().collect { it.findAll(/\d+/).collect { it as int } }
lines = lines.collect { it -> [x0: it[0], y0: it[1], x1: it[2], y1: it[3]] }

def getSlope(def line) {
  if (line.x0 == line.x1) return null // vertical line
  return (line.y1 - line.y0) / (line.x1 - line.x0)
}

def drawLine(def line, boolean useDiagonal) {
  slope = getSlope(line)
  switch (slope) {
    case { it == null }: // vertical
      return (Math.min(line.y0, line.y1)..Math.max(line.y0, line.y1)).collect { [x: line.x0, y: it] }
    case { Math.abs(it) < 1E-8 }:  // horizontal
      return (Math.min(line.x0, line.x1)..Math.max(line.x0, line.x1)).collect { [x: it, y: line.y0] }
    case { useDiagonal && Math.abs(it) == 1 }:
      x0 = slope == 1 ? Math.min(line.x0, line.x1) : Math.max(line.x0, line.x1)
      return (Math.min(line.y0, line.y1)..Math.max(line.y0, line.y1)).collect {
        [
          x: x0 + (slope as int) * (it - Math.min(line.y0, line.y1)),
          y: it
        ]
      }
  }
}

def countIntersections(def lines, boolean useDiagonal) {
  points = lines.collect { drawLine(it, useDiagonal) }.findAll().collectMany { it }
  freq = points.countBy { it }

  freq.count { k, v -> v > 1 }
}

println "PartI: ${countIntersections(lines, false)}"
println "PartII: ${countIntersections(lines, true)}"
