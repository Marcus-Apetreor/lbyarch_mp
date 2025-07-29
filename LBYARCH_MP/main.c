#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Assembly function prototype
extern void imgCvtGrayFloatToInt(const float* src, uint8_t* dst, int width, int height);

void printImage(const uint8_t* img, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%d", img[i * width + j]);
            if (j < width - 1) printf(", ");
        }
        printf("\n");
    }
}

int checkCorrectness(const float* src, const uint8_t* dst, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        uint8_t expected = (uint8_t)(src[i] * 255.0f + 0.5f);
        if (dst[i] != expected) return 0;
    }
    return 1;
}

float randFloat() {
    return (float)rand() / (float)RAND_MAX;
}

int main() {
    int sizes[3][2] = {{10, 10}, {100, 100}, {1000, 1000}};
    srand((unsigned int)time(NULL));

    for (int s = 0; s < 3; ++s) {
        int width = sizes[s][0], height = sizes[s][1];
        float* src = (float*)malloc(width * height * sizeof(float));
        uint8_t* dst = (uint8_t*)malloc(width * height * sizeof(uint8_t));

        // Initialize random float image
        for (int i = 0; i < width * height; ++i) src[i] = randFloat();

        // Timing
        double total = 0.0;
        for (int i = 0; i < 30; ++i) {
            clock_t start = clock();
            imgCvtGrayFloatToInt(src, dst, width, height);
            clock_t end = clock();
            total += (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC;
        }
        double avg = total / 30.0;
        printf("Size: %dx%d, Avg time: %.2f us\n", width, height, avg);

        // Correctness check
        if (!checkCorrectness(src, dst, width, height)) {
            printf("Output incorrect!\n");
        } else {
            printf("Output correct.\n");
        }

        // Print output for small size
        if (width * height <= 100) {
            printImage(dst, width, height);
        }
        printf("\n");

        free(src);
        free(dst);
    }
    return 0;   
}