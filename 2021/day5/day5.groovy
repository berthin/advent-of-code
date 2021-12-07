input = new File("small.in")
// input = new File("basic.in")

lines = input.readLines().collect { it.findAll(/\d+/).collect { it as int } }
lines = lines.collect { it -> [x0: it[0], y0: it[1], x1: it[2], y1: it[3]] }

// println lines*.x0
// min_x = (lines*.x0 + lines*.x1).min()
// max_x = (lines*.x0 + lines*.x1).max()
// min_y = (lines*.y0 + lines*.y1).min()
// max_y = (lines*.y0 + lines*.y1).max()
//
// board = new int[max_x - min_x + 1][max_y - min_y + 1]

def getSlope(def line) {
  if (line.x0 == line.x1) return null // vertical line
  return (line.y1 - line.y0) / (line.x1 - line.x0)
}

def drawLine(def line) {
  if (line.x0 == line.x1) { // horitonzal
    (Math.min(line.y0, line.y1)..Math.max(line.y0, line.y1)).collect { [x: line.x0, y: it] }
  } else if (line.y0 == line.y1) {
    (Math.min(line.x0, line.x1)..Math.max(line.x0, line.x1)).collect { [x: it, y: line.y0] }
  } else {
    // do nothing
  }
}

points = lines.collect { line ->
  slope = getSlope(line)
  if (slope == null || Math.abs(slope) <= 1E-8) drawLine(line)
}.findAll()

// println points

freq = points.collectMany { [it*.x, it*.y].transpose() }.countBy { it }

println "PartI: ${freq.count { k, v -> v > 1 }}"
