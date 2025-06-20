#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLORS 3

int main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[1], "rb");

    int width, height, maxval;
    fscanf(fp, "P6\n%d %d\n%d\n", &width, &height, &maxval);

    FILE *out_fp = fopen("output.ppm", "wb");
    fprintf(out_fp, "P6\n%d %d\n255\n", width, height);

    unsigned char *prev_row = malloc(width * COLORS);
    unsigned char *curr_row = malloc(width * COLORS);
    unsigned char *next_row = malloc(width * COLORS);
    unsigned char *output_row = malloc(width * COLORS);

    int histogram[6] = {0};

    fread(prev_row, 1, width * COLORS, fp);

    fwrite(prev_row, 1, width * COLORS, out_fp);
    for (int i = 0; i < width; i++)
        histogram[(int)(0.2126 * prev_row[i * COLORS] +
                             0.7152 * prev_row[i * COLORS + 1] +
                             0.0722 * prev_row[i * COLORS + 2] + 0.5) /
                  51]++;

    fread(curr_row, 1, width * COLORS, fp);
    fread(next_row, 1, width * COLORS, fp);

    for (int i = 1; i < height - 1; i++)
    {
        output_row[0] = curr_row[0];
        output_row[1] = curr_row[1];
        output_row[2] = curr_row[2];
        histogram[(int)(0.2126 * curr_row[0] +
                             0.7152 * curr_row[1] +
                             0.0722 * curr_row[2] + 0.5) /
                  51]++;

        for (int j = 1; j < width - 1; j++)
        {
            int sumR = -prev_row[j * COLORS] - curr_row[(j - 1) * COLORS] +
                       5 * curr_row[j * COLORS] - curr_row[(j + 1) * COLORS] -
                       next_row[j * COLORS];

            int sumG = -prev_row[j * COLORS + 1] - curr_row[(j - 1) * COLORS + 1] +
                       5 * curr_row[j * COLORS + 1] - curr_row[(j + 1) * COLORS + 1] -
                       next_row[j * COLORS + 1];

            int sumB = -prev_row[j * COLORS + 2] - curr_row[(j - 1) * COLORS + 2] +
                       5 * curr_row[j * COLORS + 2] - curr_row[(j + 1) * COLORS + 2] -
                       next_row[j * COLORS + 2];

            sumR = sumR < 0 ? 0 : (sumR > 255 ? 255 : sumR);
            sumG = sumG < 0 ? 0 : (sumG > 255 ? 255 : sumG);
            sumB = sumB < 0 ? 0 : (sumB > 255 ? 255 : sumB);

            output_row[j * COLORS] = sumR;
            output_row[j * COLORS + 1] = sumG;
            output_row[j * COLORS + 2] = sumB;

            histogram[(int)round(0.2126 * sumR + 0.7152 * sumG + 0.0722 * sumB) / 51]++;
        }

        output_row[(width - 1) * COLORS] = curr_row[(width - 1) * COLORS];
        output_row[(width - 1) * COLORS + 1] = curr_row[(width - 1) * COLORS + 1];
        output_row[(width - 1) * COLORS + 2] = curr_row[(width - 1) * COLORS + 2];
        histogram[(int)(0.2126 * curr_row[(width - 1) * COLORS] +
                             0.7152 * curr_row[(width - 1) * COLORS + 1] +
                             0.0722 * curr_row[(width - 1) * COLORS + 2] + 0.5) /
                  51]++;

        fwrite(output_row, 1, width * COLORS, out_fp);

        unsigned char *temp = prev_row;
        prev_row = curr_row;
        curr_row = next_row;
        next_row = temp;

        fread(next_row, 1, width * COLORS, fp);
    }

    fwrite(curr_row, 1, width * COLORS, out_fp);
    for (int i = 0; i < width; i++)
        histogram[(int)(0.2126 * curr_row[i * COLORS] +
                             0.7152 * curr_row[i * COLORS + 1] +
                             0.0722 * curr_row[i * COLORS + 2] + 0.5) /
                  51]++;

    free(prev_row);
    free(curr_row);
    free(next_row);
    free(output_row);

    fclose(fp);
    fclose(out_fp);

    fp = fopen("output.txt", "wb");
    fprintf(fp, "%d %d %d %d %d", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4] + histogram[5]);
    fclose(fp);

    return EXIT_SUCCESS;
}
