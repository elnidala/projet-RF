#ifndef CROSS_VALIDATION_H
#define CROSS_VALIDATION_H

#include "data_split.h"  // Includes ShapeData structure definition

// In cross_validation.h
typedef void (*ModelFunction)(SplitData split);

void crossValidation(ShapeData *data, int dataSize, int kFolds, ModelFunction modelFunc);
void knnModelFunction(SplitData split);

#endif // CROSS_VALIDATION_H
