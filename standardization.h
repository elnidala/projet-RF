#ifndef STANDARDIZATION_H
#define STANDARDIZATION_H

#include "data_reader.h"  // Include to use the ShapeData structure

// Function to standardize the feature values in ShapeData.
// Standardization (or Z-score normalization) scales the data to have a mean of 0 and standard deviation of 1.
// Parameters:
//   data: Pointer to the array of ShapeData.
//   dataSize: Total number of ShapeData items.
//   featureCount: Number of features in each ShapeData item.
void standardizeData(ShapeData *data, int dataSize, int featureCount);

#endif // STANDARDIZATION_H
