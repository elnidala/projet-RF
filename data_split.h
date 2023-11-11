#ifndef DATA_SPLIT_H
#define DATA_SPLIT_H

#include "data_reader.h" // Include to use the ShapeData structure.

// Structure for holding split data sets: training and test sets.
typedef struct {
    ShapeData *trainingSet; // Pointer to array of ShapeData for training.
    int trainingSize;       // Number of elements in the training set.
    ShapeData *testSet;     // Pointer to array of ShapeData for testing.
    int testSize;           // Number of elements in the test set.
} SplitData;

// Splits the provided shape data into training and test sets.
// Parameters:
//   shapes: Pointer to the array of ShapeData to be split.
//   totalSize: Total number of elements in the shape data array.
//   trainingFraction: Fraction of data to be used as the training set (0.0 to 1.0).
// Returns:
//   A SplitData structure containing the training and test sets.
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction);

#endif // DATA_SPLIT_H
