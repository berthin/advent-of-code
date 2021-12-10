input = new File("basic.in")
input = new File("small.in")

chunks = input.readLines()

def findFirstMismatch(String chunk, String pattern) {
  open = pattern[(0..pattern.size()-1).by(2).collect()] as List
  close = pattern[(1..pattern.size()-1).by(2).collect()] as List

  stack = [] as Stack

  corrupted = chunk.find { chr ->
    if (chr in open) {
      stack << chr
    } else {
      opposite = open[close.indexOf(chr)]
      if (stack.pop() != opposite) {
        return chr
      }
    }
    null
  }

  [corrupted: corrupted, stack: stack, open: open, close: close]
}

mismatches = chunks.collect { chunk ->
  try {
    mismatch = findFirstMismatch(chunk, "()[]{}<>")
  } catch (EmptyStackException e) {
    mismatch.corrupted = null
  }
  mismatch
}

// PartI
scoreCorrupted = mismatches.sum { mismatch ->
  [
    ")": 3,
    "]": 57,
    "}": 1197,
    ">": 25137,
  ].getOrDefault(mismatch.corrupted, 0)
}
println "PartI $scoreCorrupted"

// PartII
scoresIncomplete = mismatches.collect { mismatch ->
  if (mismatch.corrupted != null) return 0
  def (open, close) = [mismatch.open, mismatch.close]
  mismatch.stack.reverse().inject(BigInteger.ZERO) { prv, ch ->
    [
      ")": 1,
      "]": 2,
      "}": 3,
      ">": 4,
    ].get(close[open.indexOf(ch)]) + prv * 5
  }
}.findAll().sort()

println "PartII ${scoresIncomplete[(scoresIncomplete.size() - 1) / 2]}"
