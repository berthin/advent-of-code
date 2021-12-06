input = new File("small.in")
// input = new File("basic.in")

input = input.readLines().findAll() // Sanitize input

drawns = input[0].split(",").collect { it as int }
boardSize = 5

boards = input[1..-1].collate(boardSize).collect { board ->
  board.collect { it.trim().split(" ").findAll().collect { number -> number as int } }
}

def determineWinningNumber(def board, List drawns) {
  lut = [(0..boardSize - 1), (0..boardSize - 1)].combinations().collectEntries { position ->
    number = board[position[0]][position[1]]
    [(number): position]
  }

  columns = [:]
  rows = [:]
  sumUnmarked = board.flatten().sum()

  [drawns, (0..drawns.size() - 1)].transpose().collect { it ->
    def (drawn, index) = it

    if (!(drawn in lut)) return null
    def (row, col) = lut[drawn]

    sumUnmarked -= drawn

    columns.put(col, columns.getOrDefault(col, 0) + 1)
    rows.put(row, rows.getOrDefault(row, 0) + 1)

    if (columns.containsValue(boardSize) || rows.containsValue(boardSize)) {
      return [score: sumUnmarked * drawn, index: index]
    }
    return null
  }.find()
}

winnings = boards.collect { board ->
  determineWinningNumber(board, drawns)
}

// PartI
winner = winnings.min { it.index }
println "PartI: ${winner.score}"

// Part II
winner = winnings.max { it.index }
println "PartII: ${winner.score }"
