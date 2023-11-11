#ifndef DATA_READER_H
#define DATA_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Structure representing the data of a shape.
// It includes the class, sample number, an array of feature values, and the count of these features.
typedef struct {
    int class;           // Class identifier of the shape.
    int sample;          // Sample number for the shape.
    double *features;    // Pointer to an array of feature values.
    int featureCount;    // Number of elements in the features array.
} ShapeData;


// Reads all files with a specified extension in a given directory.
// Parameters:
//   directory: Path to the directory containing files.
//   extension: File extension to filter the files to be read.
//   count: Pointer to an integer to store the number of files read.
// Returns:
//   Pointer to an array of ShapeData, each element representing one file's data.
ShapeData* readAllFiles(const char *directory, const char *extension, int *count);


// Reads and processes a single file to extract shape data.
// Parameters:
//   filename: Path to the file to be read.
// Returns:
//   ShapeData structure containing data extracted from the file.
ShapeData readFile(const char *filename);


// Frees the dynamically allocated memory for an array of ShapeData.
// Parameters:
//   data: Pointer to the array of ShapeData.
//   count: Number of elements in the array.
void freeShapeData(ShapeData *data, int count);


// Determines the expected number of features in a file based on its extension.
// Parameters:
//   filename: Path to the file.
// Returns:
//   Expected number of features for known file types, or -1 for unknown types.
int getExpectedFeatureCount(const char *filename);

#endif // DATA_READER_H
