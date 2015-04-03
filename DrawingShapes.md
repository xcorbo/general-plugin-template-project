<h1>Introduction</h1>

Header file: <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/graphics/graphics.h'>graphics/graphics.h</a></tt>

The shape drawing module arrived with GPTP v2.33. It provides functions for drawing basic shapes and text on the in-game screen and map.

To use the drawing module, the `graphics::resetAllGraphics()` function must be called every frame. By default, this is already done in `hooks/game_hooks.cpp`, so you don't have to worry about this.

To use the actual drawing functions, you must add the following code at the top of the source file:

```
#include <graphics/graphics.h>
```
<br />

<h1>Table of Contents</h1>


<br />

# Colors #

All shape-drawing functions (except `drawText()`) take the shape's color as an argument. The list of available colors is defined in `graphics/graphics.h`, under the `graphics` namespace. They are: `RED, BLUE, TEAL, PURPLE, ORANGE, BROWN, WHITE, YELLOW, GREEN, CYAN, BLACK`, and `GREY`.

<img src='http://gptp.googlecode.com/svn/wiki/images/DrawingShapes/drawing-module-colors.png' border='1' />

`graphics::drawText()` does not take a color as an argument. Instead, it uses the same string color codes used in map editors and TBL files. Use hexadecimal character codes to manually specify text color:

```
graphics::drawText(15, 25, "\x06This is red text. \x01Normal color \x11orange");
graphics::drawText(45, 25, "\x06This is red text. \x01Normal color \x11orange", );
```
<br />


# Positioning #

All drawing functions take a `CoordType` parameter that specifies how the given coordinates are interpreted. For example, when drawing a box at (10, 20):

  * `graphics::ON_SCREEN` places it at (10, 20) relative to the top left corner of the screen. This is the default.
  * `graphics::ON_MAP` places it at (10, 20) relative to the top left corner of the map.
  * `graphics::ON_MOUSE` places it at (10, 20) relative to the current mouse position.

Note that all shapes are drawn over the in-game map and units, but not over the in-game console (unit status display and minimap). Overlapping shapes are drawn over the ones created previously, regardless of their positioning.<br />


# Function Reference #

All of the following functions are available in the `graphics` namespace.<br />

## Drawing Functions ##

### `drawText` ###
```
void drawText(int x, int y, const std::string& str, FontSize fontSize = FONT_MEDIUM, CoordType ct = ON_SCREEN);
```

Draws a text string at (`x`, `y`). You can specify a font size for the `fontSize` argument:

  * `FONT_SMALL`
  * `FONT_MEDIUM`: This is the default.
  * `FONT_LARGE`
  * `FONT_LARGEST`

![http://gptp.googlecode.com/svn/wiki/images/DrawingShapes/drawing-module-fonts.png](http://gptp.googlecode.com/svn/wiki/images/DrawingShapes/drawing-module-fonts.png)
<br />

### `drawDot` ###
```
void drawDot(int x, int y, ColorId color, CoordType ct = ON_SCREEN);
```

Draws a dot at (`x`, `y`).<br />

### `drawLine` ###
```
void drawLine(int x1, int y1, int x2, int y2, ColorId color, CoordType ct = ON_SCREEN);
```

Draws a line connecting (`x1`, `y1`) and (`x2`, `y2`).<br />

### `drawBox` ###
```
void drawBox(int left, int top, int right, int bottom, ColorId color, CoordType ct = ON_SCREEN);
```

Draws an empty box with the bounds given by `left`, `top`, `right`, `bottom`.<br />

### `drawFilledBox` ###
```
void drawFilledBox(int left, int top, int right, int bottom, ColorId color, CoordType ct = ON_SCREEN);
```

Draws a filled box with the bounds given by `left`, `top`, `right`, `bottom`.<br />

### `drawCircle` ###
```
void drawCircle(int x, int y, int radius, ColorId color, CoordType ct = ON_SCREEN);
```

Draws an empty circle with `radius` centered at (`x`, `y`).<br />

### `drawFilledCircle` ###
```
void drawFilledCircle(int x, int y, int radius, ColorId color, CoordType ct = ON_SCREEN);
```

Draws a filled circle with `radius` centered at (`x`, `y`).<br />


## Miscellaneous ##

### `resetAllGraphics` ###
```
void resetAllGraphics();
```

Resets all previous graphics and clears error messages. This should be called every frame in `hooks/game_hooks.cpp`.