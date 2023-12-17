#ifndef KNN_H
#define KNN_H

#include "data_reader.h" // Include for ShapeData structure definition.

#include <math.h>

// Error codes
#define KNN_SUCCESS 0
#define KNN_ERR_INVALID_P -1
#define KNN_ERR_INVALID_K -2
#define KNN_ERR_MEMORY_ALLOCATION -3
#define KNN_ERR_NULL_POINTER -4


/** 
 * Structure to associate a distance with a class label.
 * Used in the k-NN algorithm for mapping distances to training sample labels.
 */
typedef struct {
    double distance; /**< Distance between test and training samples. */
    int label;       /**< Class label of the training sample. */
} DistanceLabel;

/** 
 * DistanceFunction: Pointer type for various distance calculation functions.
 */
typedef double (*DistanceFunction)(ShapeData, ShapeData, int);

/**
 * Calculates Minkowski distance between two ShapeData instances.
 * @param a First ShapeData instance.
 * @param b Second ShapeData instance.
 * @param featureCount Number of features in each ShapeData instance.
 * @param p Minkowski distance exponent (1 for Manhattan, 2 for Euclidean, etc.).
 * @return Calculated Minkowski distance, or KNN_ERR_INVALID_P for invalid p value.
 */
double minkowskiDistance(ShapeData a, ShapeData b, int featureCount, int p);

/**
 * Precomputes distances between test and training samples.
 * @param trainingSet Array of training samples.
 * @param trainingSize Number of samples in the training set.
 * @param testSet Array of test samples.
 * @param testSize Number of samples in the test set.
 * @param featureCount Number of features in each ShapeData instance.
 * @param p Minkowski distance exponent.
 * @return 2D array of distances, NULL on memory allocation failure.
 */
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p);

/**
 * Classifies a test sample using the k-NN algorithm.
 * @param distances Precomputed distances array.
 * @param testIndex Index of the test sample.
 * @param trainingSet Array of training samples.
 * @param trainingSize Number of samples in the training set.
 * @param k Number of nearest neighbors to use.
 * @return Predicted class label, or KNN_ERR_INVALID_K for invalid k value.
 */
int knnClassify(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k);

#endif // KNN_H