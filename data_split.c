#include "data_split.h"

// Shuffles an array of ShapeData.
void shuffleData(ShapeData *shapes, int size) {
    // Initialize random number generator only once
    static int rand_initialized = 0;
    if (!rand_initialized) {
        srand(time(NULL));
        rand_initialized = 1;
    }

    // Shuffle using Fisher-Yates algorithm
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        ShapeData temp = shapes[i];
        shapes[i] = shapes[j];
        shapes[j] = temp;
    }
}

// Splits shape data into training and test sets.
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction) {
    // Validate input parameters
    if (!shapes || trainingFraction < 0.0 || trainingFraction > 1.0) {
        return (SplitData){NULL, 0, NULL, 0};
    }

    // Shuffle data for random distribution
    shuffleData(shapes, totalSize);

    SplitData split = {NULL, 0, NULL, 0};
    int trainingSize = (int)(totalSize * trainingFraction);
    int testSize = totalSize - trainingSize;

    // Memory allocation for training and test sets
    split.trainingSet = (ShapeData *)malloc(trainingSize * sizeof(ShapeData));
    if (!split.trainingSet) {
        return (SplitData){NULL, 0, NULL, 0};
    }

    split.testSet = (ShapeData *)malloc(testSize * sizeof(ShapeData));
    if (!split.testSet) {
        free(split.trainingSet);
        return (SplitData){NULL, 0, NULL, 0};
    }

    // Distribute data into training and test sets
    for (int i = 0; i < trainingSize; i++) {
        split.trainingSet[i] = shapes[i];
    }
    for (int i = 0; i < testSize; i++) {
        split.testSet[i] = shapes[i + trainingSize];
    }

    split.trainingSize = trainingSize;
    split.testSize = testSize;

    return split;
}

// Frees the dynamically allocated memory in a SplitData structure.
void freeSplitData(SplitData *split) {
    if (split) {
        // Free memory allocated for training and test sets
        free(split->trainingSet);
        free(split->testSet);

        // Reset pointers and sizes
        split->trainingSet = NULL;
        split->testSet = NULL;
        split->trainingSize = 0;
        split->testSize = 0;
    }
}
