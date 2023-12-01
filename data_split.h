#ifndef DATA_SPLIT_H
#define DATA_SPLIT_H

#include "data_reader.h" // Include to access the ShapeData structure definition.

#include <time.h>

/**
 * Structure to hold the split data sets: training and test sets.
 */
typedef struct {
    ShapeData *trainingSet; // Array of ShapeData for the training set.
    int trainingSize;       // Number of elements in the training set.
    ShapeData *testSet;     // Array of ShapeData for the test set.
    int testSize;           // Number of elements in the test set.
} SplitData;

/**
 * Splits shape data into training and test sets.
 * This function shuffles the data and then splits it based on the specified fraction.
 * 
 * @param shapes Array of ShapeData to be split.
 * @param totalSize Total number of elements in the shapes array.
 * @param trainingFraction Fraction of the data to be used for training (between 0.0 and 1.0).
 * @return A SplitData structure containing the separated training and test sets. If memory
 * allocation fails, returns SplitData with NULL pointers and zero sizes.
 */
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction);

/**
 * Shuffles an array of ShapeData.
 * This function randomizes the order of elements in the array to ensure random distribution
 * for splitting into training and test sets.
 * 
 * @param shapes Array of ShapeData to be shuffled.
 * @param size Number of elements in the shapes array.
 */
static void shuffleData(ShapeData *shapes, int size);

#endif // DATA_SPLIT_H
