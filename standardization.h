
/**
 * @file standardization.h
 * @brief Header file for standardizing feature values in ShapeData.
 */

#ifndef STANDARDIZATION_H
#define STANDARDIZATION_H

#include "data_reader.h"  // Include to use the ShapeData structure

/**
 * @brief Standardizes the feature values in an array of ShapeData.
 *
 * Standardization (or Z-score normalization) scales the data to have a mean of 0 and standard deviation of 1.
 *
 * @param data Pointer to the array of ShapeData.
 * @param dataSize Total number of ShapeData items.
 * @param featureCount Number of features in each ShapeData item.
 */
void standardizeData(ShapeData *data, int dataSize, int featureCount);

#endif // STANDARDIZATION_H
