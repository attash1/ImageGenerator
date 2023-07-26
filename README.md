Reads commands from a file specified by the user to draw an image and outputs design into another file. The file the design is placed in, paint.txt, is created by the program

By default, character used for drawn line is *


Command Format:
<PenStatus>,<Direction>,<Distance>,B (Optional), P (Optional)

PenStatus - Determines if command draws line or moves cursor (Pen is lifted up)
                1 = Pen up, moves cursor without drawing line
                2 = Pen down, draws line

Direction - Direction cursor moves
                N - North
                E - East
                S - South
                W - West

Distance - Number of positions moved by the cursor in given direction


B - Makes line drawn bold, replacing * character used for drawn line with #.

P - Outputs the current state of paint.txt into the console.
