# Measurement Units #

StarCraft uses several types of measurement units that one should know about when making a mod. This should not be confused with game units, such as Marines, Zerglings, and Zealots.

## Distance ##

  * Pixel: The basic unit of distance.
  * Matrix: Used for sight range, seek range (AKA target acquisition range), map tiles, as well as to describe weapon range in official websites. 1 matrix = 32 pixels.
  * Submatrix: Used in `weapons.dat` for describing weapon range. 1 submatrix = 16 pixels = 0.5 matrix.
  * Subpixel: Used in `flingy.dat` for unit speed and acceleration. 256 subpixels = 1 pixel.

## Time ##

  * Frame: The basic unit of time. Used in `flingy.dat` for unit speed and acceleration.
  * Second: Depends on game speed settings. See the table below:

| **Game Speed** | **Frames Per Second** |
|:---------------|:----------------------|
| Slowest | 6 |
| Slower | 9 |
| Slow | 12 |
| Normal | 15 |
| Fast | 18 |
| Faster | 21 |
| Fastest | 24 |

## Angle ##

  * Binary radian (brad): The basic unit of angle. Used in `flingy.dat` for unit turn speed (incorrectly labeled "Turn Radius"), as well as in `weapons.dat` for attack angle and launch spin. 1 brad = 1.40625 degrees, and 256 brad = 360 degrees.
  * Frame radian (frad): Represents the angle difference between two frames of a frameset in a GRP file. Used by the `turncwise`, `turnccwise`, and `turn1cwise` opcodes in `iscript.bin`. 1 frad = 8 brad = 11.25 degrees, and 32 frad = 360 degrees.

## HP, Shields, and Energy ##

StarCraft uses integers to calculate fractions of HP, shields, and energy. For example, a unit with 40 hit points in-game actually has the internal value of 40 x 256 = 10240 hit points.

## Supply ##

StarCraft handles supply in integers. To handle the 0.5 supply of Zerglings and Scourges, it internally uses 2 times the supply amount displayed in-game.

For example, Zerglings actually have 1 supply (instead of 0.5) in the plugin; Marines and Medics have 2 supply, instead of 1; and Zealots and Dragoons have 4 supply (instead of 2).