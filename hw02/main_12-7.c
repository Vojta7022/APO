#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLORS 3

int main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[1], "rb");

    int width, height, maxval;
    fscanf(fp, "P6\n%d %d\n%d\n", &width, &height, &maxval);

    unsigned char *buffer = malloc(2 * width * height * COLORS);
    unsigned char *image = buffer;
    unsigned char *output = buffer + width * height * COLORS;

    fread(image, 1, width * height * COLORS, fp);
    fclose(fp);

    int histogram[6] = {0};

    for (int i = 0; i < width; i++)
    {
        output[i * COLORS] = image[i * COLORS];
        output[i * COLORS + 1] = image[i * COLORS + 1];
        output[i * COLORS + 2] = image[i * COLORS + 2];
        histogram[(int)round(0.2126 * image[i * COLORS] + 0.7152 * image[i * COLORS + 1] + 0.0722 * image[i * COLORS + 2]) / 51]++;
    }

    for (int i = 1; i < height - 1; i++)
    {
        unsigned char *row = image + i * width * COLORS;
        unsigned char *output_row = output + i * width * COLORS;

        output_row[0] = row[0];
        output_row[1] = row[1];
        output_row[2] = row[2];
        histogram[(int)round(0.2126 * row[0] + 0.7152 * row[1] + 0.0722 * row[2]) / 51]++;

        output_row[(width - 1) * COLORS] = row[(width - 1) * COLORS];
        output_row[(width - 1) * COLORS + 1] = row[(width - 1) * COLORS + 1];
        output_row[(width - 1) * COLORS + 2] = row[(width - 1) * COLORS + 2];
        histogram[(int)round(0.2126 * row[(width - 1) * COLORS] + 0.7152 * row[(width - 1) * COLORS + 1] + 0.0722 * row[(width - 1) * COLORS + 2]) / 51]++;

        for (int j = 1; j < width - 1; j++)
        {
            int sumR = 0, sumG = 0, sumB = 0;

            unsigned char *row_km1 = image + (i - 1) * width * COLORS;
            unsigned char *row_kp1 = image + (i + 1) * width * COLORS;

            // Red
            sumR -= row_km1[j * COLORS];
            sumR -= row[(j - 1) * COLORS];
            sumR += 5 * row[j * COLORS];
            sumR -= row[(j + 1) * COLORS];
            sumR -= row_kp1[j * COLORS];

            // Green
            sumG -= row_km1[j * COLORS + 1];
            sumG -= row[(j - 1) * COLORS + 1];
            sumG += 5 * row[j * COLORS + 1];
            sumG -= row[(j + 1) * COLORS + 1];
            sumG -= row_kp1[j * COLORS + 1];

            // Blue
            sumB -= row_km1[j * COLORS + 2];
            sumB -= row[(j - 1) * COLORS + 2];
            sumB += 5 * row[j * COLORS + 2];
            sumB -= row[(j + 1) * COLORS + 2];
            sumB -= row_kp1[j * COLORS + 2];

            sumR = sumR < 0 ? 0 : (sumR > 255 ? 255 : sumR);
            sumG = sumG < 0 ? 0 : (sumG > 255 ? 255 : sumG);
            sumB = sumB < 0 ? 0 : (sumB > 255 ? 255 : sumB);

            output_row[j * COLORS] = sumR;
            output_row[j * COLORS + 1] = sumG;
            output_row[j * COLORS + 2] = sumB;

            histogram[(int)round(0.2126 * sumR + 0.7152 * sumG + 0.0722 * sumB) / 51]++;
        }
    }

    for (int i = 0; i < width; i++)
    {
        output[(height - 1) * width * COLORS + i * COLORS] = image[(height - 1) * width * COLORS + i * COLORS];
        output[(height - 1) * width * COLORS + i * COLORS + 1] = image[(height - 1) * width * COLORS + i * COLORS + 1];
        output[(height - 1) * width * COLORS + i * COLORS + 2] = image[(height - 1) * width * COLORS + i * COLORS + 2];
        histogram[(int)round(0.2126 * image[(height - 1) * width * COLORS + i * COLORS] + 0.7152 * image[(height - 1) * width * COLORS + i * COLORS + 1] + 0.0722 * image[(height - 1) * width * COLORS + i * COLORS + 2]) / 51]++;
    }

    fp = fopen("output.ppm", "wb");
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(output, 1, width * height * COLORS, fp);
    fclose(fp);

    free(buffer);

    fp = fopen("output.txt", "wb");
    fprintf(fp, "%d %d %d %d %d", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4] + histogram[5]);
    fclose(fp);

    return EXIT_SUCCESS;
}
