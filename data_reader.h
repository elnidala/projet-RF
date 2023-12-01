#ifndef DATA_READER_H
#define DATA_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Error codes for various failure scenarios
#define SUCCESS 0
#define ERR_DIR_OPEN_FAILED 1
#define ERR_FILE_OPEN_FAILED 2
#define ERR_MEMORY_ALLOCATION_FAILED 3
#define ERR_INVALID_FILENAME -1
#define ERR_UNKNOWN_FILE_TYPE 5
#define ERR_FEATURES_VALUES 6

/** 
 * Structure representing the data of a shape.
 */
typedef struct {
    int class;           // Class identifier of the shape.
    int sample;          // Sample number for the shape.
    double *features;    // Array of feature values.
    int featureCount;    // Number of elements in the features array.
} ShapeData;

/**
 * Reads all files with a specified extension in a given directory.
 * @param directory Path to the directory containing files.
 * @param extension File extension to filter the files to be read.
 * @param count Pointer to an integer to store the number of files read.
 * @return Pointer to an array of ShapeData, each element representing one file's data.
 */
ShapeData* readAllFiles(const char *directory, const char *extension, int *count);

/**
 * Reads and processes a single file to extract shape data.
 * @param filename Path to the file to be read.
 * @return ShapeData structure containing data extracted from the file.
 */
ShapeData readFile(const char *filename);

/**
 * Frees the dynamically allocated memory for an array of ShapeData.
 * @param data Pointer to the array of ShapeData.
 * @param count Number of elements in the array.
 */
void freeShapeData(ShapeData *data, int count);

/**
 * Determines the expected number of features in a file based on its extension.
 * @param filename Path to the file.
 * @return Expected number of features for known file types, or ERR_UNKNOWN_FILE_TYPE for unknown types.
 */
int getExpectedFeatureCount(const char *filename);

/**
 * Parses the filename to extract class and sample information.
 * @param filename The filename to parse.
 * @param class Pointer to store the extracted class number.
 * @param sample Pointer to store the extracted sample number.
 * @return SUCCESS if parsing is successful, ERR_INVALID_FILENAME otherwise.
 */
int parseFilename(const char *filename, int *class, int *sample);

/**
 * Allocates memory for storing feature values in ShapeData.
 * @param data Pointer to the ShapeData structure where features should be stored.
 * @return SUCCESS if memory allocation is successful, ERR_MEMORY_ALLOCATION_FAILED otherwise.
 */
int allocateFeatures(ShapeData *data);

/**
 * Reads feature values from a file and stores them in an array.
 * @param file Pointer to the FILE object to read from.
 * @param features Pointer to the array where features should be stored.
 * @param featureCount The number of features to read.
 * @return SUCCESS if all features are read successfully, ERR_FEATURES_VALUES otherwise.
 */
int readFeaturesFromFile(FILE *file, double *features, int featureCount);

#endif // DATA_READER_H
