#include "cross_validation.h"

void crossValidation(ShapeData *data, int dataSize, int kFolds, ModelFunction modelFunc) {
    int foldSize = dataSize / kFolds;

    for (int i = 0; i < kFolds; i++) {
        int testStart = i * foldSize;
        int testEnd = testStart + foldSize;
        SplitData split;
        split.trainingSet = malloc((dataSize - foldSize) * sizeof(ShapeData));
        split.testSet = malloc(foldSize * sizeof(ShapeData));
        split.trainingSize = 0;
        split.testSize = 0;

        // Populate training and test sets
        for (int j = 0; j < dataSize; j++) {
            if (j >= testStart && j < testEnd) {
                split.testSet[split.testSize++] = data[j];
            } else {
                split.trainingSet[split.trainingSize++] = data[j];
            }
        }

        // Call the model function
        modelFunc(split);

        // Free memory
        free(split.trainingSet);
        free(split.testSet);
    }
}

