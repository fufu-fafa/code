from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
import csv

CELL_SIZE = 30

def draw_crossword(csv_file, pdf_file):
    with open(csv_file, newline="") as f:
        reader = csv.reader(f)
        grid = list(reader)

    rows = len(grid)
    cols = max(len(r) for r in grid)

    c = canvas.Canvas(pdf_file, pagesize=letter)
    width, height = letter

    start_x = (width - cols * CELL_SIZE) / 2
    start_y = (height + rows * CELL_SIZE) / 2

    for r, row in enumerate(grid):
        for col, cell in enumerate(row):
            x = start_x + col * CELL_SIZE
            y = start_y - r * CELL_SIZE

            if cell == ".":
                continue
            else:
                c.setFillGray(1)
                c.rect(x, y - CELL_SIZE, CELL_SIZE, CELL_SIZE, fill=1, stroke=1)

                if cell.strip():
                    c.setFillGray(0)
                    c.setFont("Helvetica", 12)
                    c.drawCentredString(x + CELL_SIZE/2, y - CELL_SIZE + 8, cell)

    c.showPage()
    c.save()
draw_crossword("output.csv", "crossword.pdf")
