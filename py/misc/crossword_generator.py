import csv
import random

def create_crossword(word_list, size=32):
    words = sorted(word_list, key=len, reverse=True)

    grid = [["." for _ in range(size)] for _ in range(size)]
    placed_words = []
    unplaced = []

    if not words:
        return grid, placed_words, unplaced

    first = words.pop(0)
    row = size // 2
    col = (size - len(first)) // 2
    place_word(grid, first, row, col, "H")
    placed_words.append((first, row, col, "H"))

    for word in words:
        placed = False
        for pw, prow, pcol, pd in placed_words:
            for i, pletter in enumerate(pw):
                for j, wletter in enumerate(word):
                    if pletter != wletter:
                        continue

                    if pd == "H":
                        new_row = prow - j
                        new_col = pcol + i
                        if can_place(grid, word, new_row, new_col, "V"):
                            place_word(grid, word, new_row, new_col, "V")
                            placed_words.append((word, new_row, new_col, "V"))
                            placed = True
                            break
                    else:
                        new_row = prow + i
                        new_col = pcol - j
                        if can_place(grid, word, new_row, new_col, "H"):
                            place_word(grid, word, new_row, new_col, "H")
                            placed_words.append((word, new_row, new_col, "H"))
                            placed = True
                            break
                if placed: break
            if placed: break

        if not placed:
            unplaced.append(word)

    return grid, placed_words, unplaced


def can_place(grid, word, row, col, direction):
    size = len(grid)

    if direction == "H":
        # bounds
        if row < 0 or row >= size: return False
        if col < 0 or col + len(word) > size: return False

        # cell before and after must be empty (prevent joining same-direction words)
        if col - 1 >= 0 and grid[row][col - 1] != ".": return False
        if col + len(word) < size and grid[row][col + len(word)] != ".": return False

        for i, ch in enumerate(word):
            r, c = row, col + i
            cell = grid[r][c]
            # conflicting letter
            if cell != "." and cell != ch:
                return False
            # if placing a new letter (cell == '.'), ensure no perpendicular neighbors
            if cell == ".":
                # above and below must be empty (prevent touching vertical words)
                if r - 1 >= 0 and grid[r - 1][c] != ".": return False
                if r + 1 < size and grid[r + 1][c] != ".": return False

    else:
        # bounds
        if col < 0 or col >= size: return False
        if row < 0 or row + len(word) > size: return False

        # cell before and after (up and down) must be empty
        if row - 1 >= 0 and grid[row - 1][col] != ".": return False
        if row + len(word) < size and grid[row + len(word)][col] != ".": return False

        for i, ch in enumerate(word):
            r, c = row + i, col
            cell = grid[r][c]
            if cell != "." and cell != ch:
                return False
            # if placing a new letter, ensure left and right are empty
            if cell == ".":
                if c - 1 >= 0 and grid[r][c - 1] != ".": return False
                if c + 1 < size and grid[r][c + 1] != ".": return False

    return True


def place_word(grid, word, row, col, direction):
    if direction == "H":
        for i, ch in enumerate(word):
            grid[row][col + i] = ch
    else:
        for i, ch in enumerate(word):
            grid[row + i][col] = ch


def print_grid(grid):
    size = len(grid)
    for y in range(size):
        for x in range(size):
            if x == size-1:
                if grid[y][x] == ".":
                    continue
                print(f" {grid[y][x]}", end="")
            elif grid[y][x] == ".":
                print("   |", end="")
            else:
                print(f" {grid[y][x]} |", end="")
        print("")
        if y == size-1:
            continue
        for z in (range(size * 4)):
            if z == size*4-1:
                continue
            elif z%4 == 3:
                print("+", end="")
            else:
                print("-", end="")
        print("")

def get_words(arr, infile):
    with open(infile, "r") as file:
        for line in file:
            arr.append(line.strip())

def crop_grid(grid):
    size = len(grid)
    min_row, max_row = size, 0
    min_col, max_col = size, 0

    for r in range(size):
        for c in range(size):
            if grid[r][c] != ".":
                if r < min_row: min_row = r
                if r > max_row: max_row = r
                if c < min_col: min_col = c
                if c > max_col: max_col = c

    if min_row > max_row or min_col > max_col:
        return []

    cropped = [row[min_col:max_col+1] for row in grid[min_row:max_row+1]]
    return cropped

def save_to_file(grid, outfile="output.csv"):
    with open(outfile, "w", newline="") as file:
        writer = csv.writer(file)
        for row in grid:
            writer.writerow(row)

if __name__ == "__main__":
    words = []
    get_words(words, "words.txt")
    grid, placed, unplaced = create_crossword(words, 128)
    cropped = crop_grid(grid)
    print_grid(cropped)
    save_to_file(cropped)
    print("file saved as output.csv")
    print("\nPlaced:", [p[0] for p in placed])
    print("Unplaced:", unplaced)
