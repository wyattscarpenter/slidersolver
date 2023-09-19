# slidersolver

A program to solve [slider puzzles](https://en.wikipedia.org/wiki/Sliding_puzzle). Written in C. It should take a few seconds at most to run.

## Installation

Open this [link](https://www.youtube.com/watch?v=MpN91wHAr1k) in another tab and then run `make`.

## Use

Run the program. It will solve the puzzle, and then print out a trace of the solution.

To solve other slider puzzles, you must change the hardcoded values in `byte initial[]` in the code.

This is easy for [8-puzzles](https://en.wikipedia.org/wiki/9-puzzle), but for larger slider puzzles like the common [15-puzzle](https://en.wikipedia.org/wiki/15-puzzle) you'd have to modify several parts of the program, both constants and logic.

## Algorithmic Strategy

This code basically just bruteforces a solution, by exploring and caching the possibility state. There are some optimizations, but only in the form of data structures, not mathematical insights. (Some of the code runs mathematical checks to try to ensure the puzzle is valid, but this does not actually help solve the puzzle.)

## Third-Party Tools

- Make (optional)
- GCC, or another c compiler (for Make, the other c compiler must be aliased to gcc) (mandatory)
