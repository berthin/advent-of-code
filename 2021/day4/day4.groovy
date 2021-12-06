input = new File("small.in").readLines().findAll()
// input = new File("basic.in").readLines().findAll()

// Part I
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
      score = sumUnmarked * drawn
      return [score, index]
    }
    return null
  }.find()
}

// PartI

winnings = boards.collect { board ->
  determineWinningNumber(board, drawns)
}

winner = winnings.min { it[1] }
println "PartI: ${winner[0]}"

// Part II
winner = winnings.max { it[1] }
println "PartII: ${winner[0]}"
