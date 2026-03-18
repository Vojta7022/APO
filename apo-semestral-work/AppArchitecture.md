### Architecture Overview

- **Input** is received from buttons and interpreted as player commands or menu navigation.
- **The main loop** in `tileman.c` controls game progression and coordinates updates.
- **Game logic** interacts with modules like `player`, `score`, and `menu` to update state.
- **Output modules** (`lcd`, `led`, `audio`) provide user feedback during gameplay.

### Project Structure
| File / Module     | Description |
|-------------------|-------------|
| `tileman.c`       | Main game loop, responsible for initializing and controlling the overall application flow. |
| `audio.c/.h`      | Handles sound playback using the onboard speaker. |
| `lcd.c/.h`        | Controls the LCD display connected to the board. Handles text rendering. |
| `led.c/.h`        | Controls the LED strip. |
| `menu.c/.h`       | Menu screen logic and rendering. |
| `mzapo.c/.h`      | Configuration of  physical addresses. |
| `params.h`        | Contains constants, configuration values, and global parameters. |
| `player.c/.h`     | Functions for creating and controlling the player entity. |
| `score.c/.h`      | Score counting and display logic. |

### I/O Peripherals
#### Inputs
- **Buttons**: Used to navigate menu and to control player characters.
#### Outputs
- **Speaker**: Plays tone at startup, buzzes when player is hit.
- **LED Strip**: Displays remaining gametime.
- **LCD Display**: Renders the menus and game.


### GitLab repository
    <https://gitlab.fel.cvut.cz/ponrtvoj/apo-semestral-work>
