import groovy.transform.Canonical

input = new File("basic.in")
input = new File("small.in")


heightMap = input.readLines().collect { it.collect { it as int } }

// Part I
adjacents = [
  [row: 0, col: -1],
  [row: 0, col: +1],
  [row: -1, col: 0],
  [row: +1, col: 0],
]

nRows = heightMap.size()
nCols = heightMap[0].size()

risk = 0
basins = []

visited = new int[nRows][nCols]

comparator = [ compare: { pointA, pointB -> pointA.height <=> pointB.height } ] as Comparator

mark = 1
int bfs(int row, int col) {
  queue = new PriorityQueue(100, comparator)
  queue << [row: row, col: col, height: heightMap[row][col]]

  counter = 0
  while (queue.size() > 0) {
    node = queue.poll()
    if (visited[node.row][node.col] == mark) continue

    visited[node.row][node.col] = mark
    counter += (heightMap[node.row][node.col] != 9) ? 1 : 0

    adjacents.each { adjacent ->
      r = adjacent.row + node.row
      c = adjacent.col + node.col

      if (!(0 <= r && r < nRows && 0 <= c && c < nCols)) return; // non existent adjacent is ok
      if (visited[r][c] == mark) return;
      h = heightMap[r][c]

      if (node.height < h) queue << [row: r, col: c, height: h]
    }
  }
  return counter
}

heightMap.eachWithIndex { heightRow, row ->
  heightRow.eachWithIndex { height, col ->
    isOneLowestPoint = adjacents.every { adjacent ->
      r = adjacent.row + row
      c = adjacent.col + col
      if (!(0 <= r && r < nRows && 0 <= c && c < nCols)) return true // non existent adjacent is ok
      height < heightMap[r][c]
    }
    if (isOneLowestPoint) {
      risk += (height + 1)
      basins << bfs(row, col)
      mark += 1
    }
  }
}

println "PartI: $risk"

partII = (basins.sort { -it })[0..2].inject(1) { prv, cur -> prv * cur }
println "PartII: $partII"
