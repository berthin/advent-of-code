input = new File("simple1.in")
// input = new File("simple2.in")
// input = new File("simple3.in")
// input = new File("small.in")

routes = input.readLines().collect { it.split("-") }

graph = [:]

def addEdge = { from, to ->
  if (from in graph)
    graph[from] << to
  else
    graph[from] = [to]
}

routes.each { it ->
  def (from, to) = it
  addEdge(from, to)
  addEdge(to, from)
}

visited = [:]

isSmallCave = { cave -> cave == cave.toLowerCase() }

def canVisit(String cave, String smallCave) {
  if (cave == 'start') return [false, smallCave]
  if (cave == 'end') return [true, smallCave]

  if (!isSmallCave(cave)) return [true, smallCave]

  previouslyVisited = visited.getOrDefault(cave, 0)
  if (previouslyVisited == 0) return [true, smallCave]
  if (previouslyVisited == 1 && smallCave == null) return [true, cave]

  return [false, smallCave]
}

int dfs(String cave, String smallCave) {
  if (cave == 'end') return 1

  graph[cave].sum { nextCave ->
    answer = 0
    def (can, tmp) = canVisit(nextCave, smallCave)
    if (can) {
      visited.put(nextCave, visited.getOrDefault(nextCave, 0) + 1)
      answer = dfs(nextCave, tmp)
      visited.put(nextCave, visited[nextCave] - 1)
    }
    answer
  }
}

println "PartI: ${dfs('start', 'none')}"
visited = [:]
println "PartII: ${dfs('start', null)}"
