#ifndef KNN_H
#define KNN_H

#include "data_reader.h"

// Struct to hold the distance and its corresponding label.
// Used in the k-NN algorithm to associate a training sample (label) with its distance to a test sample.
typedef struct {
    double distance;  // The calculated distance between a test sample and a training sample.
    int label;        // The class label of the training sample.
} DistanceLabel;

// Define a function pointer type for distance functions.
typedef double (*DistanceFunction)(ShapeData, ShapeData, int);

// Precomputes distances between each pair of test and training samples using a specified distance metric.
// Parameters:
//   trainingSet: Pointer to the array of ShapeData for the training set.
//   trainingSize: Number of elements in the training set.
//   testSet: Pointer to the array of ShapeData for the test set.
//   testSize: Number of elements in the test set.
//   featureCount: Number of features in each ShapeData item.
//   p: The exponent parameter of the Minkowski distance. (p=1 for Manhattan, p=2 for Euclidean)
// Returns:
//   Pointer to a 2D array where each element [i][j] represents the distance between the i-th test sample and the j-th training sample.
double** precomputeDistances(ShapeData *trainingSet, int trainingSize, ShapeData *testSet, int testSize, int featureCount, int p);

// Classifies a test sample using the k-NN algorithm based on precomputed distances.
// Parameters:
//   distances: 2D array of precomputed distances between test and training samples.
//   testIndex: Index of the test sample in the test set.
//   trainingSet: Pointer to the array of ShapeData for the training set.
//   trainingSize: Number of elements in the training set.
//   k: Number of nearest neighbors to consider for classification.
//   featureCount: Number of features in each ShapeData item.
// Returns:
//   The predicted class label for the test sample.
int knnClassifyPrecomputed(double **distances, int testIndex, ShapeData *trainingSet, int trainingSize, int k, int featureCount);

#endif // KNN_H
