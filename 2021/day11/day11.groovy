input = new File("basic.in")
// input = new File("small.in")

class Position2D {
    int row
    int col

    def plus(Position2D other) {
        [row: this.row + other.row, col: this.col + other.col] as Position2D
    }

    boolean isInsideBounds(int rows, int cols) {
        0 <= this.row && this.row < rows && 0 <= this.col && this.col < cols
    }

    def asList() { [this.row, this.col] }

    String toString() { "(${this.row}, ${this.col})" }
}

cave = input.readLines().collect { it.collect { it as int } }
rows = cave.size()
cols = cave[0].size()

printCave = { cave -> cave.each { println it.join("") } }

neighbors = [
    [+1, +1, +1, 0, -1, -1, -1, 0],
    [-1, 0, +1, +1, -1, 0, +1, -1]
].transpose().collect { row, col -> [row: row, col: col] as Position2D }

List getNeighbors(def position) {
    neighbors.collect { it + position }.findAll { it.isInsideBounds(rows, cols) }
}

int oneStep(def cave) {
    (0..rows - 1).each { row -> (0..cols - 1).each { col -> cave[row][col] += 1 } }

    toExplode = [] as Queue
    cave.eachWithIndex { line, row ->
        line.eachWithIndex { lvl, col ->
            if (lvl > 9) toExplode << ([row: row, col: col] as Position2D)
        }
    }

    visited = [:]
    while (toExplode.size() > 0) {
        flashed = toExplode.poll()
        if (flashed.toString() in visited) continue
        visited[flashed.toString()] = true
        cave[flashed.row][flashed.col] = 0
        getNeighbors(flashed).each { neighbor ->
            if (neighbor.toString() in visited) return null
            cave[neighbor.row][neighbor.col] += 1
            if (cave[neighbor.row][neighbor.col] > 9) toExplode << neighbor
        }
    }
    visited.size()
}


// PartI
partICave = cave.collect { it.clone() }
steps = 100
flashes = (1..steps).sum { oneStep(partICave) }
println "PartI: $flashes"

// PartII
firstTimeSynced = (1..500).find { it ->
    oneStep(cave)
    areSynced = cave.sum { it.sum() } == 0
    if (areSynced) return it
    null
}

println "PartII: $firstTimeSynced"
