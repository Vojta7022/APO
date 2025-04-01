#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLORS 3

int kernel[3][3] = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0}
};

int main(int argc, char const *argv[])
{
    FILE *fp = fopen(argv[1], "rb");

    int width, height, maxval;
    fscanf(fp, "P6\n%d %d\n%d\n", &width, &height, &maxval);

    unsigned char *image = malloc(width * height * COLORS);
    fread(image, 1, width * height * COLORS, fp);

    fclose(fp);

    unsigned char *output = malloc(width * height * COLORS);

    int histogram[] = {0, 0, 0, 0, 0, 0};

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0 || j == width - 1 || i == height - 1) {
                output[(i * width + j) * COLORS] = image[(i * width + j) * COLORS];
                output[(i * width + j) * COLORS + 1] = image[(i * width + j) * COLORS + 1];
                output[(i * width + j) * COLORS + 2] = image[(i * width + j) * COLORS + 2];
            } else {
                int sumR = 0, sumG = 0, sumB = 0;

                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        sumR += kernel[k + 1][l + 1] * image[((i + k) * width + (j + l)) * COLORS];
                        sumG += kernel[k + 1][l + 1] * image[((i + k) * width + (j + l)) * COLORS + 1];
                        sumB += kernel[k + 1][l + 1] * image[((i + k) * width + (j + l)) * COLORS + 2];
                    }
                }

                sumR = sumR < 0 ? 0 : (sumR > 255 ? 255 : sumR);
                sumG = sumG < 0 ? 0 : (sumG > 255 ? 255 : sumG);
                sumB = sumB < 0 ? 0 : (sumB > 255 ? 255 : sumB);

                output[(i * width + j) * COLORS] = sumR;
                output[(i * width + j) * COLORS + 1] = sumG;
                output[(i * width + j) * COLORS + 2] = sumB;
                
                int r;
                r = (0.2126*sumR + 0.7152*sumG + 0.0722*sumB + 0.5)/51;
                histogram[r]++;

                // double grayscale = round(0.2126 * sumR + 0.7152 * sumG + 0.0722 * sumB);

                // if (grayscale <= 50) histogram[0]++;
                // else if (grayscale <= 101) histogram[1]++;
                // else if (grayscale <= 152) histogram[2]++;
                // else if (grayscale <= 203) histogram[3]++;
                // else histogram[4]++;
            }
        }
    }

    free(image);

    fp = fopen("output.ppm", "wb");
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(output, 1, width * height * COLORS, fp);
    fclose(fp);

    free(output);

    fp = fopen("output.txt", "wb");
    fprintf(fp, "%d %d %d %d %d", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4] + histogram[5]);
    fclose(fp);

    return EXIT_SUCCESS;
}

