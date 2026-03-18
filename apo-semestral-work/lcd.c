#include "lcd.h"
#include "player.h"

// Framebuffer representing the screen (each pixel is a 16-bit color value)
unsigned short lcd_fb[LCD_HEIGHT][LCD_WIDTH];

// Tiles array representing the game area on the LCD screen
unsigned short tiles[LCD_HEIGHT / SQUARE_SIZE][LCD_WIDTH / SQUARE_SIZE];

void lcd_draw_rectangle(unsigned sx, unsigned sy, unsigned w, unsigned h, unsigned short color)
{
    for (unsigned x = sx; x < sx + w; x++)
    {
        for (unsigned y = sy; y < sy + h; y++)
        {
            lcd_fb[y][x] = color;
        }
    }
}

void lcd_update()
{
    mzapo_lcd_update((unsigned short *)lcd_fb);
}

void lcd_clear()
{
    for (int x = 0; x < LCD_WIDTH; x++)
    {
        for (int y = 0; y < LCD_HEIGHT; y++)
        {
            lcd_fb[y][x] = 0;
        }
    }
}

void tiles_draw(score_t *score, player_t *player1, player_t *player2)
{
    // 1, 2 – player 1 (2 = temporary path)
    // 3, 4 – player 2 (4 = temporary path)

    uint16_t color;
    for (int i = 0; i < LCD_HEIGHT / SQUARE_SIZE; i++)
    {
        for (int j = 0; j < LCD_WIDTH / SQUARE_SIZE; j++)
        {
            if (tiles[i][j] == 1)
            {
                color = player1->color;
                score_update(score, player1);
            }
            else if (tiles[i][j] == 2)
            {
                // Temporary path color for player 1 (slightly altered)
                uint8_t r = (player1->color >> 11) & 0x1F;
                uint8_t g = (player1->color >> 5) & 0x3F;
                uint8_t b = player1->color & 0x1F;

                r = (r + 5) % 32;
                g = (g + 10) % 64;
                b = (b > 5) ? b - 5 : 0;

                color = RGB(r << 3, g << 2, b << 3);
            }
            else if (tiles[i][j] == 3)
            {
                color = player2->color;
                score_update(score, player2);
            }
            else if (tiles[i][j] == 4)
            {
                // Temporary path color for player 2 (slightly altered)
                uint8_t r = (player2->color >> 11) & 0x1F;
                uint8_t g = (player2->color >> 5) & 0x3F;
                uint8_t b = player2->color & 0x1F;

                r = (r + 5) % 32;
                g = (g + 10) % 64;
                b = (b > 5) ? b - 5 : 0;

                color = RGB(r << 3, g << 2, b << 3);
            }
            else
            {
                continue;
            }
            lcd_draw_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, color);
        }
    }
}

/**
 * Flood-fill algorithm to determine and fill captured area.
 * Tiles not reachable from screen edge are considered captured.
 */
void fill_captured_area(player_t *player)
{
    int width = LCD_WIDTH / SQUARE_SIZE;
    int height = LCD_HEIGHT / SQUARE_SIZE;

    int territory_tile = (player->player_num == 1) ? 1 : 3;
    int path_tile = (player->player_num == 1) ? 2 : 4;

    bool visited[height][width];
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            visited[y][x] = false;
    point_t queue[width * height];
    int front = 0, back = 0;

    // Enqueue all border tiles that are empty
    for (int x = 0; x < width; x++)
    {
        if (tiles[0][x] == 0)
        {
            queue[back++] = (point_t){x, 0};
            visited[0][x] = true;
        }
        if (tiles[height - 1][x] == 0)
        {
            queue[back++] = (point_t){x, height - 1};
            visited[height - 1][x] = true;
        }
    }
    for (int y = 0; y < height; y++)
    {
        if (tiles[y][0] == 0)
        {
            queue[back++] = (point_t){0, y};
            visited[y][0] = true;
        }
        if (tiles[y][width - 1] == 0)
        {
            queue[back++] = (point_t){width - 1, y};
            visited[y][width - 1] = true;
        }
    }

    // BFS (Breadth-First Search) to find all reachable tiles from the border
    while (front < back)
    {
        point_t p = queue[front++];
        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};
        for (int d = 0; d < 4; d++)
        {
            int nx = p.x + dx[d];
            int ny = p.y + dy[d];
            if (nx >= 0 && ny >= 0 && nx < width && ny < height)
            {
                if (!visited[ny][nx] && ((tiles[ny][nx] == 0) || (tiles[ny][nx] == (4 - territory_tile))))
                {
                    visited[ny][nx] = true;
                    queue[back++] = (point_t){nx, ny};
                }
            }
        }
    }

    // Mark unreachable tiles as captured (territory)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (((tiles[y][x] == 0) || (tiles[y][x] == (4 - territory_tile))) && !visited[y][x])
            {
                tiles[y][x] = territory_tile;
            }
            else if (tiles[y][x] == path_tile)
            {
                tiles[y][x] = territory_tile;
            }
        }
    }
}

void lcd_draw_char(int x, int y, char ch, unsigned short color, int scale, font_descriptor_t *fdes)
{
    int w = char_width(ch, fdes);
    const font_bits_t *ptr;
    if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size))
    {
        if (fdes->offset)
        {
            ptr = &fdes->bits[fdes->offset[ch - fdes->firstchar]];
        }
        else
        {
            int bw = (fdes->maxwidth + 15) / 16;
            ptr = &fdes->bits[(ch - fdes->firstchar) * bw * fdes->height];
        }
        int i, j;
        for (i = 0; i < fdes->height; i++)
        {
            font_bits_t val = *ptr;
            for (j = 0; j < w; j++)
            {
                if ((val & 0x8000) != 0)
                {
                    lcd_draw_pixel_big(x + scale * j, y + scale * i, scale, color);
                }
                val <<= 1;
            }
            ptr++;
        }
    }
}

void lcd_draw_pixel(int x, int y, unsigned short color)
{
    if (x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT)
    {
        lcd_fb[y][x] = color;
    }
}

void lcd_draw_pixel_big(int x, int y, int scale, unsigned short color)
{
    for (int i = 0; i < scale; i++)
    {
        for (int j = 0; j < scale; j++)
        {
            lcd_draw_pixel(x + i, y + j, color);
        }
    }
}

int char_width(int ch, font_descriptor_t *fdes)
{
    return !fdes->width ? fdes->maxwidth : fdes->width[ch - fdes->firstchar];
}

void lcd_draw_string(int x, int y, const char *str, unsigned short color, int scale, font_descriptor_t *fdes)
{
    while (*str)
    {
        // Draw each character at the current position
        lcd_draw_char(x, y, *str, color, scale, fdes);

        // Move x to the right for the next character (adjust based on the font's width)
        x += char_width(*str, fdes) * scale; // Assuming scaling factor of 4

        str++; // Move to the next character in the string
    }
}

void dump_tiles()
{
    for (int i = 0; i < LCD_HEIGHT / SQUARE_SIZE; i++)
    {
        for (int j = 0; j < LCD_WIDTH / SQUARE_SIZE; j++)
        {
            tiles[i][j] = 0;
        }
    }
}
