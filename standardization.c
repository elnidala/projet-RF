#include "standardization.h"
#include <math.h>    // For mathematical operations such as sqrt.
#include <stdlib.h>  // For dynamic memory allocation functions.

// Calculates the mean and standard deviation for each feature across all ShapeData.
static void calcMeanAndStd(ShapeData *data, int dataSize, int featureCount, double *mean, double *std) {
    // Initialize mean and standard deviation arrays to zero.
    for (int i = 0; i < featureCount; i++) {
        mean[i] = 0;
        std[i] = 0;
    }

    // Summing up values to calculate the mean.
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < featureCount; j++) {
            mean[j] += data[i].features[j];
        }
    }

    // Calculating the mean for each feature.
    for (int i = 0; i < featureCount; i++) {
        mean[i] /= dataSize;
    }

    // Calculating the sum of squared deviations for standard deviation.
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < featureCount; j++) {
            std[j] += (data[i].features[j] - mean[j]) * (data[i].features[j] - mean[j]);
        }
    }

    // Calculating the standard deviation for each feature.
    for (int i = 0; i < featureCount; i++) {
        std[i] = sqrt(std[i] / dataSize);
    }
}

// Standardizes the data to have a mean of 0 and standard deviation of 1.
void standardizeData(ShapeData *data, int dataSize, int featureCount) {
    // Allocate memory for mean and standard deviation arrays.
    double *mean = (double *)malloc(featureCount * sizeof(double));
    double *std = (double *)malloc(featureCount * sizeof(double));

    // Calculate mean and standard deviation for each feature.
    calcMeanAndStd(data, dataSize, featureCount, mean, std);

    // Standardizing each feature value.
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < featureCount; j++) {
            if (std[j] != 0) {  // Avoid division by zero.
                data[i].features[j] = (data[i].features[j] - mean[j]) / std[j];
            }
        }
    }

    // Free the allocated memory for mean and standard deviation.
    free(mean);
    free(std);
}
