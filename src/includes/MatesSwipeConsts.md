# Swipe Event Constants

The table below lists the possible swipe flags that can be used for swipe handling.

| Event             | Value  | Check if swipe is             | Usage                                              |
|:-----------------:|:------:| ----------------------------- | -------------------------------------------------- |
| MATES_SWIPE_NORTH | 0b0001 | From bottom to top            | `(event & MATES_SWIPE_NORTH) == MATES_SWIPE_NORTH` |
| MATES_SWIPE_SOUTH | 0b0010 | From top to bottom            | `(event & MATES_SWIPE_SOUTH) == MATES_SWIPE_SOUTH` |
| MATES_SWIPE_EAST  | 0b0100 | From left to right            | `(event & MATES_SWIPE_EAST) == MATES_SWIPE_EAST`   |
| MATES_SWIPE_WEST  | 0b1000 | From right to left            | `(event & MATES_SWIPE_WEST) == MATES_SWIPE_WEST`   |
| MATES_SWIPE_VERT  | 0b0011 | only done vertically          | `(event & MATES_SWIPE_VERT) != 0`                  |
| MATES_SWIPE_HORZ  | 0b1100 | only done horizontally        | `(event & MATES_SWIPE_HORZ) != 0`                  |
| MATES_SWIPE_TLBR  | 0b0110 | From top left to bottom right | `(event & MATES_SWIPE_TLBR) == MATES_SWIPE_TLBR`   |
| MATES_SWIPE_TRBL  | 0b1010 | From top right to bottom left | `(event & MATES_SWIPE_TRBL) == MATES_SWIPE_TRBL`   |
| MATES_SWIPE_BLTR  | 0b0101 | From bottom left to top right | `(event & MATES_SWIPE_BLTR) == MATES_SWIPE_BLTR`   |
| MATES_SWIPE_BRTL  | 0b1001 | From bottom right to top left | `(event & MATES_SWIPE_BRTL) == MATES_SWIPE_BRTL`   |