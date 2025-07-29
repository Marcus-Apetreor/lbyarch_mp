#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

extern void imgCvtGrayFloatToInt(float* f_img, unsigned char* i_img, int size);

void printImage(unsigned char* img, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%3d ", img[i * width + j]);
        }
        printf("\n");
    }
}

void generate_random_floats(float* arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = (float)rand() / RAND_MAX;
    }
}

double time_conversion(float* f_img, unsigned char* i_img, int size) {
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    imgCvtGrayFloatToInt(f_img, i_img, size);

    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
}

void benchmark() {
    const int NUM_RUNS = 30; // run 30 times per test for avg execution time
    const int test_sizes[][2] = {
        {10, 10},
        {100, 100},
        {1000, 1000}
    };
    const int NUM_TESTS = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (int t = 0; t < NUM_TESTS; ++t) {
        int height = test_sizes[t][0];
        int width = test_sizes[t][1];
        int size = height * width;

        float* f_img = (float*)malloc(size * sizeof(float));
        unsigned char* i_img = (unsigned char*)malloc(size * sizeof(unsigned char));

        if (!f_img || !i_img) {
            // error checking for memory allocation faliures (for the 1000x1000 images)
            printf("Memory allocation failed for %dx%d image.\n", height, width);
            free(f_img);
            free(i_img);
            continue;
        }

        double total_time = 0.0;

        for (int run = 0; run < NUM_RUNS; ++run) {
            generate_random_floats(f_img, size);
            total_time += time_conversion(f_img, i_img, size);

            if (run == 0) { //verification for benchmarks
                int mismatches = 0;
                for (int i = 0; i < size; ++i) {
                    if (i_img[i] != (unsigned char)(f_img[i] * 255.0f)) {
                        mismatches++;
                    }
                }
                if (mismatches > 0) {
                    printf("Verification failed for %dx%d image: %d mismatches found.\n", height, width, mismatches);
                }
                else {
                    printf("Verification passed for %dx%d image.\n", height, width);
                }
            }
        }

        printf("Avg time for %dx%d image over %d runs: %.5f ms\n",
            height, width, NUM_RUNS, total_time / NUM_RUNS);

        free(f_img);
        free(i_img);
    }
}

int main() {
    srand((unsigned int)time(NULL)); // random generation
    int height, width;

    printf("Enter image height and width: ");
    if (scanf_s("%d %d", &height, &width) != 2 || height <= 0 || width <= 0) {
        // error checking for user input (for invalid dimensions)
        printf("Error: Invalid dimensions inputted.\n");
        return 1;
    }

    int size = height * width;

    float* f_img = (float*)malloc(size * sizeof(float));
    unsigned char* i_img = (unsigned char*)malloc(size * sizeof(unsigned char));

    if (!f_img || !i_img) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d float pixel values (0.0 to 1.0):\n", size);
    for (int i = 0; i < size; ++i) {
        if (scanf_s("%f", &f_img[i]) != 1 || f_img[i] < 0.0f || f_img[i] > 1.0f) {
            // error checking for user inputi(for invalid pixel values)
            printf("Invalid pixel value.\n");
            free(f_img);
            free(i_img);
            return 1;
        }
    }

    imgCvtGrayFloatToInt(f_img, i_img, size);

    // check for correctness
    int errors = 0;
    for (int i = 0; i < size; ++i) {
        unsigned char expected = (unsigned char)(f_img[i] * 255.0f);
        if (i_img[i] != (unsigned char)(f_img[i] * 255.0f)) {
            errors++;
        }
    }

    if (errors == 0) {
        //if no mismatch, print success message
        printf("Output verified: All values are correct.\n");
    }
    else {
        // if mismatches are found, print the # of mismatches
        printf("Output verification failed: %d mismatches found.\n", errors);
    }

    // print img
    printf("Converted 8-bit grayscale image:\n");
    printImage(i_img, height, width);

    free(f_img);
    free(i_img);

    printf("\nAutomated Benchmarks (random generated inputs):\n");
    benchmark();

    return 0;
}
