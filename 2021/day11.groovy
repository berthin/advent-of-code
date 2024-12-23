input = new File("basic.in")

cave = input.readLines().collect { it.split().collect { it as int } }

cave.each { println it.join("") }
