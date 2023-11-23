#include "knn.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Implementation of the Minkowski distance function.
double minkowskiDistance(ShapeData a, ShapeData b, int featureCount, int p) {
    if (p <= 0) {
        fprintf(stderr, "Invalid p value for Minkowski distance: %d\n", p);
        return ERR_INVALID_P_VALUE; // Error code for invalid p value.
    }

    double sum = 0.0;
    for (int i = 0; i < featureCount; i++) {
        sum += pow(fabs(a.features[i] - b.features[i]), p);
    }
    return pow(sum, 1.0 / p);
}

// Precompute distances between test and training samples.
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p) {
    double **distances = malloc(testSize * sizeof(double *));
    if (!distances) {
        perror("Memory allocation failed for distances array");
        return NULL;
    }

    for (int i = 0; i < testSize; i++) {
        distances[i] = malloc(trainingSize * sizeof(double));
        if (!distances[i]) {
            for (int j = 0; j < i; j++) {
                free(distances[j]);
            }
            free(distances);
            perror("Memory allocation failed for distances row");
            return NULL;
        }

        for (int j = 0; j < trainingSize; j++) {
            distances[i][j] = minkowskiDistance(testSet[i], trainingSet[j], featureCount, p);
            if (distances[i][j] < 0) {
                fprintf(stderr, "Error computing distance between samples %d and %d\n", i, j);
                exit(ERR_DISTANCE);
            }
        }
    }
    return distances;
}

// Comparator function for qsort.
static int compareDistanceLabels(const void *a, const void *b) {
    const DistanceLabel *dlA = (const DistanceLabel *)a;
    const DistanceLabel *dlB = (const DistanceLabel *)b;
    return (dlA->distance > dlB->distance) - (dlA->distance < dlB->distance);
}

// k-NN classification function.
int knnClassify(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k) {
    if (k <= 0 || k > trainingSize) {
        fprintf(stderr, "Invalid k value for k-NN classification: %d\n", k);
        return -1; // Error code for invalid k value.
    }

    DistanceLabel *distanceLabels = malloc(trainingSize * sizeof(DistanceLabel));
    if (!distanceLabels) {
        perror("Memory allocation failed for DistanceLabel array");
        return -1; // Error code for memory allocation failure.
    }

    for (int i = 0; i < trainingSize; i++) {
        distanceLabels[i].distance = distances[testIndex][i];
        distanceLabels[i].label = trainingSet[i].class;
    }

    qsort(distanceLabels, trainingSize, sizeof(DistanceLabel), compareDistanceLabels);

    int predictedClass = -1, maxCount = 0;
    for (int i = 0; i < k; i++) {
        int count = 1;
        for (int j = i + 1; j < k && j < trainingSize; j++) {
            if (distanceLabels[j].label == distanceLabels[i].label) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            predictedClass = distanceLabels[i].label;
        }
    }

    free(distanceLabels);
    return predictedClass;
}
