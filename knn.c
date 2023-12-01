#include "knn.h"

// Implementation of Minkowski distance
double minkowskiDistance(ShapeData a, ShapeData b, int featureCount, int p) {
    if (p <= 0 || !a.features || !b.features) {
        fprintf(stderr, "Invalid parameters for Minkowski distance\n");
        return KNN_ERR_INVALID_P; // Error handling for invalid 'p' values.
    }

    double sum = 0.0;
    for (int i = 0; i < featureCount; i++) {
        // Add the p-th power of absolute difference of each feature to the sum.
        sum += pow(fabs(a.features[i] - b.features[i]), p);
    }
    // The p-th root of the sum gives the Minkowski distance.
    return pow(sum, 1.0 / p);
}

// Precompute distances between test and training samples
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p) {
    if (!trainingSet || !testSet || p <= 0) {
        fprintf(stderr, "Invalid parameters for precomputing distances\n");
        return NULL; // Memory allocation check.
    }

    double **distances = malloc(testSize * sizeof(double *));
    if (!distances) {
        return NULL;
    }

    for (int i = 0; i < testSize; i++) {
        distances[i] = malloc(trainingSize * sizeof(double));
        if (!distances[i]) {
            // Clean up in case of memory allocation failure
            for (int j = 0; j < i; j++) {
                free(distances[j]);
            }
            free(distances);
            return NULL;
        }

        for (int j = 0; j < trainingSize; j++) {
            distances[i][j] = minkowskiDistance(testSet[i], trainingSet[j], featureCount, p);
            if (distances[i][j] < 0) {
                // Clean up and exit in case of invalid distance
                for (int k = 0; k <= i; k++) {
                    free(distances[k]);
                }
                free(distances);
                return NULL;
            }
        }
    }
    return distances;
}

// Comparator for sorting distance-label pairs
static int compareDistanceLabels(const void *a, const void *b) {
    const DistanceLabel *dlA = (const DistanceLabel *)a;
    const DistanceLabel *dlB = (const DistanceLabel *)b;
    return (dlA->distance > dlB->distance) - (dlA->distance < dlB->distance);
}

// k-NN classification implementation
int knnClassify(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k) {
    // Validate the input parameters.
    if (k <= 0 || k > trainingSize || !distances || !trainingSet) {
        fprintf(stderr, "Invalid parameters for k-NN classification\n");
        return KNN_ERR_INVALID_K; // Error handling for invalid 'k' values.
    }

    // Allocate memory for storing the distance and label of each training sample.
    DistanceLabel *distanceLabels = malloc(trainingSize * sizeof(DistanceLabel));
    if (!distanceLabels) {
        return KNN_ERR_MEMORY_ALLOCATION; // Memory allocation check
    }

    // Populate the array of distance-label pairs for each training sample.
    for (int i = 0; i < trainingSize; i++) {
        distanceLabels[i].distance = distances[testIndex][i];
        distanceLabels[i].label = trainingSet[i].class;
    }

    // Sort the distance-label pairs in ascending order of distance.
    qsort(distanceLabels, trainingSize, sizeof(DistanceLabel), compareDistanceLabels);

    // Determine the most frequent class among the k nearest neighbors
    int predictedClass = -1, maxCount = 0;
    for (int i = 0; i < k; i++) {
        int currentLabel = distanceLabels[i].label;
        int count = 1;
         // Count occurrences of the current label in the k nearest neighbors.
        for (int j = i + 1; j < k && distanceLabels[j].label == currentLabel; j++) {
            count++;
        }
        // Update the predicted class if this label is more frequent.
        if (count > maxCount) {
            maxCount = count;
            predictedClass = currentLabel;
        }
    }

    // Free the memory allocated for the distance-label pairs.
    free(distanceLabels);

    // Return the predicted class.
    return predictedClass;
}
