#include "data_reader.h"
#include <math.h> // Included for math functions like isnan() and isinf().

// Reads and processes all files with a given extension in a specified directory.
// Populates an array of ShapeData structures with their contents.
ShapeData* readAllFiles(const char *directory, const char *extension, int *count) {
    DIR *dir;
    struct dirent *ent;
    char filename[1024];     // Buffer to store the complete filepath.
    int capacity = 100;      // Initial capacity for ShapeData array.
    int n = 0;               // Counter for number of files processed.
    ShapeData *data = malloc(capacity * sizeof(ShapeData)); // Allocate initial array.

    if ((dir = opendir(directory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, extension)) { // Check if the file has the specified extension.
                // Construct full filepath.
                snprintf(filename, sizeof(filename), "%s/%s", directory, ent->d_name);
                if (n >= capacity) {
                    // Double the capacity if the initial capacity is exceeded.
                    capacity *= 2;
                    data = realloc(data, capacity * sizeof(ShapeData));
                }
                // Read data from file and store in array.
                data[n++] = readFile(filename);
            }
        }
        closedir(dir); // Close the directory once all files are processed.
    } else {
        // Error handling if the directory cannot be opened.
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }
    *count = n; // Set the count of read files.
    return data; // Return the array of ShapeData.
}

// Reads a single file to extract shape data.
ShapeData readFile(const char *filename) {
    FILE *file = fopen(filename, "r"); // Open file for reading.
    if (!file) {
        // Error handling if file opening fails.
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    ShapeData data;
    // Initialize ShapeData structure.
    data.class = 0;
    data.sample = 0;
    // Determine the expected number of features based on the file name.
    data.featureCount = getExpectedFeatureCount(filename);

    // Allocate memory for storing feature values.
    data.features = malloc(data.featureCount * sizeof(double));
    if (data.features == NULL) {
        // Error handling for memory allocation failure.
        fprintf(stderr, "Error allocating memory for features\n");
        fclose(file);
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    // Read and store each feature value from the file.
    char buffer[1024];
    int featureIndex = 0;
    while (featureIndex < data.featureCount && fgets(buffer, sizeof(buffer), file) != NULL) {
        data.features[featureIndex] = atof(buffer);
        featureIndex++;
    }

    fclose(file); // Close the file after reading.
    return data;  // Return the filled ShapeData struct.
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
        return -1; // Return -1 if no extension is found.
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

    return -1; // Return -1 for unknown file types.
}
