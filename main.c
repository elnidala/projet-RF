#include "data_reader.h"
#include "data_split.h"
#include "normalization.h"
#include "standardization.h"
#include "knn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <directory> <file_extension> <training_fraction> <process_method>\n", argv[0]);
        fprintf(stderr, "process_method: none, normalize, standardize\n");
        return EXIT_FAILURE;
    }

    const char *directory = argv[1];
    const char *extension = argv[2];
    float trainingFraction = atof(argv[3]);
    char *processMethod = argv[4];
    int count, i, k = 2; // Example: k set to 5, adjust based on your needs
    int featureCount = 16; // Assuming feature count is known and fixed

    // Read all files
    ShapeData *shapes = readAllFiles(directory, extension, &count);

    // Process data based on method specified
    if (strcmp(processMethod, "normalize") == 0) {
        normalizeData(shapes, count, featureCount);
    } else if (strcmp(processMethod, "standardize") == 0) {
        standardizeData(shapes, count, featureCount);
    }

    // Split data into training and test sets
    SplitData split = splitData(shapes, count, trainingFraction);

    // Assume p is provided as a command-line argument or set by default
    int p = (argc > 5) ? atoi(argv[5]) : 2; // Default to Euclidean if not specified

    // Precompute distances using the Minkowski distance with parameter p
    double **precomputedDistances = precomputeDistances(split.trainingSet, split.trainingSize, split.testSet, split.testSize, featureCount, p);


    int bien = 0;
    int mal = 0;

    // Apply k-NN classification
    printf("\nApplying k-NN Classification (k = %d):\n", k);
    for (i = 0; i < split.testSize; i++) {
        int predictedClass = knnClassifyPrecomputed(precomputedDistances, i, split.trainingSet, split.trainingSize, k, featureCount);
        printf("Class: %d, Sample: %d\n", split.testSet[i].class, split.testSet[i].sample);
        printf("Test Sample %d predicted as class %d\n", i, predictedClass);
        if (predictedClass == split.testSet[i].class) {
            bien++;
        }
        else {
            mal++;
        }
        
        printf("\n");
    }

    printf("====== %d - %d - %d", split.testSize, bien, mal);

    // Free the precomputed distances
    for (int i = 0; i < split.testSize; i++) {
        free(precomputedDistances[i]);
    }
    free(precomputedDistances);

    // Free the allocated memory
    freeShapeData(shapes, count);
    free(split.trainingSet);
    free(split.testSet);

    return EXIT_SUCCESS;
}
