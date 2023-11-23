#include "data_reader.h"

// Reads and processes all files with a given extension in a specified directory.
// Populates an array of ShapeData structures with their contents.
ShapeData* readAllFiles(const char *directory, const char *extension, int *count) {
    DIR *dir;                // Directory stream
    struct dirent *ent;      // For directory entries
    int capacity = 100;      // Initial capacity for ShapeData array.
    int n = 0;               // Counter for number of files processed.
    ShapeData *data = malloc(capacity * sizeof(ShapeData)); // Allocate initial array.
    
    // Check for memory allocation failure
    if (!data) {
        perror("Memory allocation failed for ShapeData array");
        exit(ERR_MEMORY_ALLOCATION_FAILED); 
    }

    // Attempt to open the directory
    dir = opendir(directory);
    if (!dir) {
        perror("Unable to open directory");
        free(data); // Clean up allocated memory
        exit(ERR_DIR_OPEN_FAILED);
    }

    // Read directory entries
    while ((ent = readdir(dir)) != NULL) {
        // Check for the specified file extension
        if (strstr(ent->d_name, extension)) {
            // Calculate required buffer size for filename
            int required_size = snprintf(NULL, 0, "%s/%s", directory, ent->d_name) + 1;
            char *filename = malloc(required_size);

            // Check for memory allocation failure for filename
            if (!filename) {
                fprintf(stderr, "Failed to allocate memory for filename\n");
                exit(ERR_MEMORY_ALLOCATION_FAILED);
            }
            
            // Construct full filepath and check for snprintf errors
            int written = snprintf(filename, required_size, "%s/%s", directory, ent->d_name);
            if (written < 0 || written >= required_size) {
                fprintf(stderr, "Error or truncation occurred in snprintf\n");
                free(filename); // Free the filename string
                exit(ERR_MEMORY_ALLOCATION_FAILED);
            }
            
            // Process the file and store its data
            ShapeData fileData = readFile(filename);
            if (fileData.features != NULL) {
                // Check if array capacity needs to be increased
                if (n >= capacity) {
                    capacity *= 2; // Double the capacity if the initial capacity is exceeded.
                    ShapeData *temp = realloc(data, capacity * sizeof(ShapeData));
                    if (!temp) {
                        perror("Memory allocation failed during resizing");
                        free(filename); // Free the filename string
                        freeShapeData(data, n); 
                        closedir(dir);
                        exit(ERR_MEMORY_ALLOCATION_FAILED);
                    }
                    data = temp;
                }
                data[n++] = fileData; // Store the read data
            }

            else {
                    fprintf(stderr, "Failed to process file: %s\n", filename);
            }
            free(filename); // Clean up filename after processing
        }
    }

    closedir(dir); // Close the directory once all files are processed.
    *count = n; // Set the count of read files.
    return data; // Return the array of ShapeData.
}

// Reads a single file to extract shape data.
ShapeData readFile(const char *filename) {
    // Attempt to open the file for reading
    FILE *file = fopen(filename, "r"); // Open file for reading.
    if (!file) {
        // Error handling if file opening fails.
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(ERR_INVALID_FILENAME);
    }

    ShapeData data;
    // Initialize ShapeData structure.
    data.class = 0;
    data.sample = 0;

    // Determine the expected number of features based on the file name.
    data.featureCount = getExpectedFeatureCount(filename);

    // Check for invalid feature count
    if (data.featureCount < 0) {
        perror("Error getting expected features");
        fclose(file);
        exit(ERR_FEATURES_VALUES);
    }
    

    // Allocate memory for storing feature values.
    data.features = malloc(data.featureCount * sizeof(double));
    if (data.features == NULL) {
        // Error handling for memory allocation failure.
        perror("Memory allocation failed for features");
        fclose(file);
        exit(ERR_MEMORY_ALLOCATION_FAILED); 
    }

    // Extract class and sample number from filename.
    const char *filename_ptr = strrchr(filename, '/');
    if (!filename_ptr) {
        filename_ptr = filename; // Handle filenames without a path.
    } else {
        filename_ptr++; // Skip the '/' character.
    }

    // Parse class and sample information from filename.
    if (sscanf(filename_ptr, "s%02dn%03d", &data.class, &data.sample) != 2) {
        fprintf(stderr, "Filename format not recognized: %s\n", filename);
        fclose(file);
        free(data.features);
        exit(ERR_INVALID_FILENAME);
    }

    // Read and store each feature value from the file.
    char buffer[1024];
    int featureIndex = 0;
    char *endptr;

    while (featureIndex < data.featureCount && fgets(buffer, sizeof(buffer), file) != NULL) {
        double featureValue = strtod(buffer, &endptr);

        // Check if conversion succeeded
        if (buffer == endptr) {
            fprintf(stderr, "Conversion error in file %s for feature at index %d\n", filename, featureIndex);
            exit(ERR_FEATURES_VALUES);
        }

        data.features[featureIndex] = featureValue;
        featureIndex++;
    }

    // Verify the expected number of features were read
    if (featureIndex != data.featureCount) {
        fprintf(stderr, "Unexpected number of features in file %s: expected %d, got %d\n", filename, data.featureCount, featureIndex);
        exit(ERR_FEATURES_VALUES);
    }

    fclose(file);
    return data;

}

// Frees the dynamically allocated memory for an array of ShapeData.
void freeShapeData(ShapeData *data, int count) {
    for (int i = 0; i < count; i++) {
        free(data[i].features); // Free the features array for each ShapeData.
    }
    free(data); // Free the array of ShapeData itself.
}


// Determines the number of expected features based on file extension.
int getExpectedFeatureCount(const char *filename) {
    const char *extension = strrchr(filename, '.'); // Extract file extension.
    if (!extension) {
        return ERR_UNKNOWN_FILE_TYPE; // Return -1 if no extension is found.
    }

    // Match extension with known file types and their respective feature counts.
    if (strcmp(extension, ".E34") == 0) {
        return 16;
    } else if (strcmp(extension, ".F0") == 0) {
        return 128;
    } else if (strcmp(extension, ".GFD") == 0) {
        return 100;
    } else if (strcmp(extension, ".SA") == 0) {
        return 90;
    }

    return ERR_UNKNOWN_FILE_TYPE; // Return -1 for unknown file types.
}
