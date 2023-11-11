#include "knn.h"
#include <stdlib.h>
#include <math.h>

// Minkowski Distance Function
static double minkowskiDistance(ShapeData a, ShapeData b, int featureCount, int p) {
    double sum = 0.0;
    for (int i = 0; i < featureCount; i++) {
        sum += pow(fabs(a.features[i] - b.features[i]), p);
    }
    return pow(sum, 1.0 / p);
}

// Function to precompute distances
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p) {
    double **distances = malloc(testSize * sizeof(double *));
    for (int i = 0; i < testSize; i++) {
        distances[i] = malloc(trainingSize * sizeof(double));
        for (int j = 0; j < trainingSize; j++) {
            distances[i][j] = minkowskiDistance(testSet[i], trainingSet[j], featureCount, p);
        }
    }
    return distances;
}

// Comparator function for qsort
static int compareDistanceLabels(const void *a, const void *b) {
    DistanceLabel *dlA = (DistanceLabel *)a;
    DistanceLabel *dlB = (DistanceLabel *)b;
    return (dlA->distance > dlB->distance) - (dlA->distance < dlB->distance);
}

// k-NN classification function
int knnClassifyPrecomputed(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k, int featureCount) {
    DistanceLabel *distanceLabels = (DistanceLabel *)malloc(trainingSize * sizeof(DistanceLabel));
    for (int i = 0; i < trainingSize; i++) {
        distanceLabels[i].distance = distances[testIndex][i];
        distanceLabels[i].label = trainingSet[i].class;
    }

    qsort(distanceLabels, trainingSize, sizeof(DistanceLabel), compareDistanceLabels);

    int maxCount = 0, predictedClass = -1;
    for (int i = 0; i < k; i++) {
        int label = distanceLabels[i].label;
        int count = 1;
        for (int j = i + 1; j < k && j < trainingSize; j++) {
            if (distanceLabels[j].label == label) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            predictedClass = label;
        }
    }

    free(distanceLabels);
    return predictedClass;
}
