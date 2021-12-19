// Assume that folding happens always in halves

input = new File("simple.in").readLines()
input = new File("small.in").readLines()

dots = input.collect {
  matcher = (it =~ /^(?<x>\d+),(?<y>\d+)$/)
  matcher.matches() ? [x: matcher.group('x') as int, y: matcher.group('y') as int ] : null
}.findAll()

folds = input.collect {
  matcher = (it =~ /^fold along (?<orientation>[xy])=(?<position>\d+)$/)
  matcher.matches() ? [orientation: matcher.group('orientation'), position: matcher.group('position') as int] : null
}.findAll()

def mustBeFolded(def fold, def dot) {
  if (fold.orientation == 'x') {
    return (dot.x >= fold.position)
  } else {
    return (dot.y >= fold.position)
  }
}

def foldDot(def fold, def dot) {
  if (fold.orientation == 'x') {
    [x: 2 * fold.position - dot.x, y: dot.y]
  } else {
    [x: dot.x, y: 2 * fold.position - dot.y]
  }
}

def foldPaper(def fold, def dots) {
  dots.collect { dot ->
    mustBeFolded(fold, dot) ? foldDot(fold, dot) : dot
  }.unique()
}

println "PartI: ${foldPaper(folds[0], dots).size()}"

dots = folds.inject(dots) { dots, fold -> foldPaper(fold, dots) }

xmax = dots*.x.max()
ymax = dots*.y.max()
display = new String[xmax + 1][ymax + 1]
dots.each { dot -> display[dot.x][dot.y] = '#' }

println "PartII:"
(0..ymax).each { y ->
  (0..xmax).each { x -> print display[x][y] ?: " " }
  println ""
}
