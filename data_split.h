#ifndef DATA_SPLIT_H
#define DATA_SPLIT_H

#include "data_reader.h" // Include to access the ShapeData structure definition.

#include <time.h>

// Define error codes for split operations
#define SPLIT_SUCCESS 0
#define SPLIT_ERR_INVALID_INPUT -1
#define SPLIT_ERR_MEMORY_FAILURE -2

/**
 * Structure to hold split data sets: training and test sets.
 */
typedef struct {
    ShapeData *trainingSet; // Array of ShapeData for the training set
    int trainingSize;       // Number of elements in the training set
    ShapeData *testSet;     // Array of ShapeData for the test set
    int testSize;           // Number of elements in the test set
} SplitData;

/**
 * Splits shape data into training and test sets.
 * The function shuffles the data before splitting to ensure random distribution.
 *
 * @param shapes Pointer to the array of ShapeData to be split
 * @param totalSize Total number of elements in the shapes array
 * @param trainingFraction Fraction of data to be used for training (0.0 - 1.0)
 * @return A SplitData structure containing training and test sets
 */
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction);

/**
 * Shuffles an array of ShapeData.
 * Algorithm for randomizing the order of elements.
 *
 * @param shapes Pointer to the array of ShapeData to shuffle
 * @param size Number of elements in the shapes array
 */
void shuffleData(ShapeData *shapes, int size);

/**
 * Frees the dynamically allocated memory in a SplitData structure.
 *
 * @param split Pointer to the SplitData structure to be freed
 */
void freeSplitData(SplitData *split);

#endif // DATA_SPLIT_H
