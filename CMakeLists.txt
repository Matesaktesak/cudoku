cmake_minimum_required(VERSION 3.16.3)

#project cudoku
project(cudoku)

set(SOURCE_FILES)

add_executable(cudoku ./src/main.c ./src/lib/cell.c ./src/lib/cell.h ./src/lib/ui.c ./src/lib/ui.h ./src/lib/playfield.c ./src/lib/playfield.h ./src/lib/solver.c ./src/lib/solver.h)