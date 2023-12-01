#include "data_reader.h"

// Parses the filename to extract class and sample information.
int parseFilename(const char *filename, int *class, int *sample) {
    // Extracting the actual filename from the path
    const char *filename_ptr = strrchr(filename, '/');
    filename_ptr = filename_ptr ? filename_ptr + 1 : filename;

    // Using sscanf to extract class and sample information from the filename
    if (sscanf(filename_ptr, "s%02dn%03d", class, sample) != 2) {
        return ERR_INVALID_FILENAME; // Return an error if the format is incorrect
    }
    return SUCCESS;
}

// Allocates memory for storing feature values.
int allocateFeatures(ShapeData *data) {
    // Allocate memory for the feature values array
    data->features = malloc(data->featureCount * sizeof(double));
    if (!data->features) {
        return ERR_MEMORY_ALLOCATION_FAILED; // Return an error if allocation fails
    }
    return SUCCESS;
}

// Reads feature values from a file and stores them in an array.
int readFeaturesFromFile(FILE *file, double *features, int featureCount) {
    char buffer[1024];
    char *endptr;
    for (int i = 0; i < featureCount; i++) {
        // Read a line from the file
        if (!fgets(buffer, sizeof(buffer), file)) {
            return ERR_FEATURES_VALUES; // Return an error if reading fails
        }
        // Convert the line to a double value and store it in the array
        features[i] = strtod(buffer, &endptr);
        if (buffer == endptr) {
            return ERR_FEATURES_VALUES; // Return an error if conversion fails
        }
    }
    return SUCCESS;
}

// Reads and processes all files with the specified extension in a directory.
ShapeData* readAllFiles(const char *directory, const char *extension, int *count) {
    DIR *dir;
    struct dirent *ent;
    int capacity = 100, n = 0;
    ShapeData *data = malloc(capacity * sizeof(ShapeData));
    if (!data) {
        perror("Memory allocation failed for ShapeData array");
        exit(ERR_MEMORY_ALLOCATION_FAILED);
    }

    dir = opendir(directory);
    if (!dir) {
        perror("Unable to open directory");
        free(data);
        exit(ERR_DIR_OPEN_FAILED);
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, extension)) {
            // Construct the full file path
            char *filename = malloc(strlen(directory) + strlen(ent->d_name) + 2);
            if (!filename) {
                perror("Memory allocation failed for filename");
                exit(ERR_MEMORY_ALLOCATION_FAILED);
            }

            sprintf(filename, "%s/%s", directory, ent->d_name);
            ShapeData fileData = readFile(filename);
            free(filename);

            // Check if the array needs to be resized
            if (n >= capacity) {
                capacity *= 2;
                ShapeData *temp = realloc(data, capacity * sizeof(ShapeData));
                if (!temp) {
                    perror("Memory allocation failed during resizing");
                    freeShapeData(data, n);
                    closedir(dir);
                    exit(ERR_MEMORY_ALLOCATION_FAILED);
                }
                data = temp;
            }
            data[n++] = fileData; // Add the read data to the array
        }
    }

    closedir(dir);
    *count = n; // Update the count of read files
    return data; // Return the array of ShapeData
}

// Reads a single file and extracts shape data.
ShapeData readFile(const char *filename) {
    ShapeData data;
    // Extract class and sample information from the filename
    if (parseFilename(filename, &data.class, &data.sample) != SUCCESS) {
        fprintf(stderr, "Filename format not recognized: %s\n", filename);
        exit(ERR_INVALID_FILENAME);
    }

    // Determine the expected number of features based on the filename
    data.featureCount = getExpectedFeatureCount(filename);
    if (data.featureCount < 0) {
        fprintf(stderr, "Error getting expected features for file: %s\n", filename);
        exit(ERR_FEATURES_VALUES);
    }

    // Allocate memory for the features
    if (allocateFeatures(&data) != SUCCESS) {
        fprintf(stderr, "Memory allocation failed for features of file: %s\n", filename);
        exit(ERR_MEMORY_ALLOCATION_FAILED);
    }

    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(ERR_FILE_OPEN_FAILED);
    }

    // Read features from the file
    if (readFeaturesFromFile(file, data.features, data.featureCount) != SUCCESS) {
        fprintf(stderr, "Error reading features from file: %s\n", filename);
        fclose(file);
        exit(ERR_FEATURES_VALUES);
    }

    fclose(file); // Close the file
    return data; // Return the read data
}

// Frees the dynamically allocated memory for an array of ShapeData structures.
void freeShapeData(ShapeData *data, int count) {
    for (int i = 0; i < count; i++) {
        free(data[i].features); // Free the memory for each features array
    }
    free(data); // Free the memory for the array of ShapeData
}

// Determines the expected number of features based on file extension.
int getExpectedFeatureCount(const char *filename) {
    // Extract the file extension
    const char *extension = strrchr(filename, '.');
    if (!extension) {
        return ERR_UNKNOWN_FILE_TYPE; // Unknown file type
    }

    // Match the extension with the expected number of features
    if (strcmp(extension, ".E34") == 0) {
        return 16;
    } else if (strcmp(extension, ".F0") == 0) {
        return 128;
    } else if (strcmp(extension, ".GFD") == 0) {
        return 100;
    } else if (strcmp(extension, ".SA") == 0) {
        return 90;
    }

    return ERR_UNKNOWN_FILE_TYPE; // Unknown file type
}
