#include "normalization.h"
#include <float.h>   // For DBL_MAX and DBL_MIN constants.
#include <stdlib.h>  // For dynamic memory allocation functions.

// Thread function to find minimum and maximum values for a subset of data.
void *findMinMaxThread(void *args) {
    ThreadArgs *arg = (ThreadArgs *)args;  // Cast the argument back to ThreadArgs type.

    // Iterate through the assigned data chunk.
    for (int i = arg->startIdx; i < arg->endIdx; i++) {
        for (int j = 0; j < arg->featureCount; j++) {
            // Update minimum and maximum values for each feature.
            if (arg->data[i].features[j] < arg->min[j]) {
                arg->min[j] = arg->data[i].features[j];
            }
            if (arg->data[i].features[j] > arg->max[j]) {
                arg->max[j] = arg->data[i].features[j];
            }
        }
    }
    return NULL;  // Required by pthread_create API.
}

// Finds the minimum and maximum values for each feature across all ShapeData.
void findMinMax(ShapeData *data, int dataSize, double *min, double *max, int featureCount) {
    int numThreads = 4;  // Number of threads to be used for parallel processing.
    pthread_t threads[numThreads];  // Array to store thread identifiers.
    ThreadArgs threadArgs[numThreads];  // Array for arguments to each thread.

    // Divide the data among threads and initialize local min and max values.
    int chunkSize = dataSize / numThreads;  // Determine size of data chunk per thread.
    for (int i = 0; i < numThreads; i++) {
        threadArgs[i].data = data;
        threadArgs[i].startIdx = i * chunkSize;
        threadArgs[i].endIdx = (i == numThreads - 1) ? dataSize : (i + 1) * chunkSize;
        threadArgs[i].min = (double *)malloc(featureCount * sizeof(double));
        threadArgs[i].max = (double *)malloc(featureCount * sizeof(double));
        // Initialize local min and max arrays.
        for (int j = 0; j < featureCount; j++) {
            threadArgs[i].min[j] = DBL_MAX;
            threadArgs[i].max[j] = -DBL_MAX;
        }
        // Create threads to process chunks of data.
        pthread_create(&threads[i], NULL, findMinMaxThread, (void *)&threadArgs[i]);
    }

    // Join threads and aggregate min and max values found.
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);  // Wait for thread to complete.
        for (int j = 0; j < featureCount; j++) {
            // Update global min and max from thread-local values.
            if (threadArgs[i].min[j] < min[j]) {
                min[j] = threadArgs[i].min[j];
            }
            if (threadArgs[i].max[j] > max[j]) {
                max[j] = threadArgs[i].max[j];
            }
        }
        // Free memory allocated for thread-local min and max arrays.
        free(threadArgs[i].min);
        free(threadArgs[i].max);
    }
}

// Normalizes the data by scaling feature values to a 0-1 range.
void normalizeData(ShapeData *data, int dataSize, int featureCount) {
    // Allocate memory for global min and max values.
    double *min = (double *)malloc(featureCount * sizeof(double));
    double *max = (double *)malloc(featureCount * sizeof(double));

    // Find the min and max values for each feature across all data.
    findMinMax(data, dataSize, min, max, featureCount);

    // Normalize each feature value to a range of 0 to 1.
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < featureCount; j++) {
            data[i].features[j] = (data[i].features[j] - min[j]) / (max[j] - min[j]);
        }
    }

    // Free memory allocated for min and max values.
    free(min);
    free(max);
}
