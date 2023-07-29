# Pocket Cube

The [Pcket Cube](https://en.wikipedia.org/wiki/Pocket_Cube)
is the 2x2x2 version of the Rubik's Cube.
The cube consists of 8 pieces, all corners.

Starting from a random (scrambled) combination, the puzzle is solved
rotating a section of pieces until all faces are identical in color.
Only shortest (wrt.\ the number of quarter turns)
paths are searched for.

## Mathematical Analysis

Any permutation of the eight corners is possible (8!\ positions), and seven of
them can be independently rotated (37 positions).  There is nothing identifying
the orientation of the cube in space, reducing the positions by a factor of 24.

The number of possible positions of the cube is 7!x3^6.

The maximum number of turns required to solve the cube is up to 14 quarter
turns.  The number of permutation as a function of the depth is shown below.
The red part of the bars show the permutations, where any turn decreases
the distance by one.

## Algorithm

To solve the puzzle, permute the colors according to the 24 orientations, so
that one fixed corner is in solved position.  The moves can be restricted to the
6 moves that do not change the fixed corner.

Due to the small number of permutations, a complete 
breadth-first search on all possible positions can be performed.

## Test Data

A cube is represented by its [net](https://en.wikipedia.org/wiki/Net_(polyhedron)),
letters code the colors, e.g.:
```
   YP
   YR
RP BG YY GB
BB RW GG WP
   WP
   WR
```

| File          | Instances | Description                         |
| ------------- | --------- | ----------------------------------- |
| pocket-14     |       276 | all instances that require 14 moves |
| pocket-100000 |    100000 | random instances                    |

## Computational Results

Every instance can be solved in 1 micro second on an Intel Core i7-9700K.
