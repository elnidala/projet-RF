#ifndef DATA_SPLIT_H
#define DATA_SPLIT_H

#include "data_reader.h" // Include to access the ShapeData structure definition.

// Struct to hold split data sets: training and test sets.
typedef struct {
    ShapeData *trainingSet; // Array of ShapeData for training.
    int trainingSize;       // Number of elements in the training set.
    ShapeData *testSet;     // Array of ShapeData for testing.
    int testSize;           // Number of elements in the test set.
} SplitData;

// Function to split shape data into training and test sets.
// Parameters:
//   shapes: Array of ShapeData to be split.
//   totalSize: Total number of elements in shapes array.
//   trainingFraction: Fraction of data for training set (0.0 to 1.0).
// Returns:
//   SplitData structure with training and test sets.
//   In case of an error (e.g., memory allocation failure), returns a SplitData with NULL pointers.
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction);

#endif // DATA_SPLIT_H
