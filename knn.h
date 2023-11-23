#ifndef KNN_H
#define KNN_H

#include "data_reader.h" // Include for ShapeData structure definition.

// Error codes
#define ERR_INVALID_P_VALUE -1.0
#define ERR_DISTANCE 2

// DistanceLabel: Structure to associate a distance with a class label.
// Used in the k-NN algorithm for mapping distances to training sample labels.
typedef struct {
    double distance; // Distance between test and training samples.
    int label;       // Class label of the training sample.
} DistanceLabel;

// DistanceFunction: Pointer type for various distance calculation functions.
typedef double (*DistanceFunction)(ShapeData, ShapeData, int);

// minkowskiDistance: Calculates Minkowski distance between two ShapeData instances.
// Parameters:
//   a, b: ShapeData instances for comparison.
//   featureCount: Number of features in each ShapeData instance.
//   p: Minkowski distance exponent (1 for Manhattan, 2 for Euclidean, etc.).
// Returns: Calculated Minkowski distance.
double minkowskiDistance(ShapeData a, ShapeData b, int featureCount, int p);

// precomputeDistances: Precomputes distances between test and training samples.
// Parameters:
//   trainingSet: Array of ShapeData for training samples.
//   trainingSize: Number of samples in the training set.
//   testSet: Array of ShapeData for test samples.
//   testSize: Number of samples in the test set.
//   featureCount: Number of features in each ShapeData instance.
//   p: Minkowski distance exponent.
// Returns: 2D array of precomputed distances, NULL on memory allocation failure.
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p);

// knnClassify: Classifies a test sample using the k-NN algorithm.
// Parameters:
//   distances: 2D array of precomputed distances.
//   testIndex: Index of the test sample in the test set.
//   trainingSet: Array of ShapeData for training samples.
//   trainingSize: Number of samples in the training set.
//   k: Number of nearest neighbors for classification.
// Returns: Predicted class label for the test sample, -1 on error.
int knnClassify(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k);

#endif // KNN_H
