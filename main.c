#include "data_reader.h"
#include "data_split.h"
#include "normalization.h"
#include "standardization.h"
#include "knn.h"
#include "kmeans.h"
#include "confusion_matrix.h"
#include "cross_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char *directory;
    char *extension;
    float trainingFraction;
    char *method;
    int p;
    int k;
    char *preprocessing;
} CommandLineOptions;

void runKnn(const CommandLineOptions *options) {
    // Read all files
    int count;
    ShapeData *shapes = readAllFiles(options->directory, options->extension, &count);

    if (!shapes) {
        fprintf(stderr, "Failed to read files\n");
        exit(EXIT_FAILURE);
    }

    // Normalize or standardize data if required
    if (strcmp(options->method, "normalize") == 0) {
        normalizeData(shapes, count, shapes->featureCount);
    } else if (strcmp(options->method, "standardize") == 0) {
        standardizeData(shapes, count, shapes->featureCount);
    }

    // Split data into training and test sets
    SplitData split = splitData(shapes, count, options->trainingFraction);

    // Precompute distances
    double **distances = precomputeDistances(split.trainingSet, split.trainingSize, split.testSet, split.testSize, shapes->featureCount, options->p);

    if (!distances) {
        fprintf(stderr, "Failed to precompute distances\n");
        exit(EXIT_FAILURE);
    }

    // Apply k-NN classification
    printf("Applying k-NN Classification (k = %d):\n", options->k);

    // Create a confusion matrix
    int classCount = 9;
    ConfusionMatrix cm = createConfusionMatrix(classCount);

    int *predictedClasses = malloc(split.testSize * sizeof(int)); // Store predicted classes

    for (int i = 0; i < split.testSize; i++) {
        int predictedClass = knnClassify(distances, i, split.trainingSet, split.trainingSize, options->k);
        int actualClass = split.testSet[i].class;
        updateConfusionMatrix(&cm, actualClass, predictedClass);
        printf("Test Sample %d predicted as class %d (Actual Class: %d)\n", i, predictedClass, split.testSet[i].class);
        //printMatrix(cm);
    }

    // Populate the confusion matrix with the k-NN results
    //populateConfusionMatrix(&cm, split.testSet, split.testSize, predictedClasses);

    // Print the detailed confusion matrix
    printDetailedConfusionMatrix(cm);

    // Perform cross-validation
    int kFolds = 5; // Example number of folds
    crossValidation(shapes, count, kFolds, knnModelFunction);

    // Free the allocated resources
    for (int i = 0; i < split.testSize; i++) {
        free(distances[i]);
    }
    free(distances);
    free(predictedClasses);
    freeConfusionMatrix(&cm);
    freeShapeData(shapes, count);
    free(split.trainingSet);
    free(split.testSet);
}

void knnModelFunction(SplitData split) {
    // Precompute distances for the current fold
    double **distances = precomputeDistances(split.trainingSet, split.trainingSize, split.testSet, split.testSize, split.trainingSet->featureCount, 2);

    if (!distances) {
        fprintf(stderr, "Failed to precompute distances\n");
        return;
    }

    int classCount = 9;
    ConfusionMatrix cm = createConfusionMatrix(classCount);

    for (int i = 0; i < split.testSize; i++) {
        // Use the existing knnClassify function that utilizes precomputed distances
        int predictedClass = knnClassify(distances, i, split.trainingSet, split.trainingSize, 5);

        updateConfusionMatrix(&cm, split.testSet[i].class, predictedClass);
    }

    printDetailedConfusionMatrix(cm);

    // Free the resources used by the confusion matrix and distances
    freeConfusionMatrix(&cm);
    for (int i = 0; i < split.testSize; i++) {
        free(distances[i]);
    }
    free(distances);
}




void runKmeans(const CommandLineOptions *options) {
    int count;
    ShapeData *shapes = readAllFiles(options->directory, options->extension, &count);
    if (!shapes) {
        fprintf(stderr, "Failed to read files\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(options->preprocessing, "normalize") == 0) {
        normalizeData(shapes, count, shapes->featureCount);
    } else if (strcmp(options->preprocessing, "standardize") == 0) {
        standardizeData(shapes, count, shapes->featureCount);
    }

    int maxIterations = 100; 
    Cluster *clusters = kmeans(shapes, count, options->k, options->p, shapes->featureCount, maxIterations);

    if (!clusters) {
        fprintf(stderr, "Failed to perform k-means clustering\n");
        exit(EXIT_FAILURE);
    }

    // Printing the classes of points in each cluster
    printf("k-Means Clustering Results (k = %d):\n", options->k);
    for (int i = 0; i < options->k; i++) {
        printf("Cluster %d:\n", clusters[i].clusterClass);
        printf("  Classes in Cluster:\n");
        for (int j = 0; j < clusters[i].size; j++) {
            printf("    Point %d: Class %d\n", j + 1, clusters[i].points[j].class);
        }
        printf("\n");
    }

    // Free resources...
    for (int i = 0; i < options->k; i++) {
        free(clusters[i].centroid->features);
        free(clusters[i].centroid);
        free(clusters[i].points);
    }
    free(clusters);
    freeShapeData(shapes, count);
}


void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s -d <directory> -e <file_extension> -f <training_fraction> -m <method> -p <p-value> -k <k-value> -l <pre-processing>\n", program_name);
}

int main(int argc, char *argv[]) {
    CommandLineOptions options = {0};
    int opt;

    while ((opt = getopt(argc, argv, "d:e:f:m:p:k:l:")) != -1) {
        switch (opt) {
            case 'd':
                options.directory = optarg;
                break;
            case 'e':
                options.extension = optarg;
                break;
            case 'f':
                options.trainingFraction = atof(optarg);
                break;
            case 'm':
                options.method = optarg;
                break;
            case 'p':
                options.p = atoi(optarg);
                break;
            case 'k':
                options.k = atoi(optarg);
                break;
            case 'l':
                if (optarg != NULL) {
                    options.preprocessing = optarg;
                } else {
                    // Handle the case where -l is provided without an argument
                    options.preprocessing = ""; // or set a default value
                }
                break;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!options.directory || !options.extension || options.trainingFraction <= 0.0 || !options.method || options.p <= 0 || options.k <= 0 || !options.preprocessing) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(options.method, "knn") == 0) {
        runKnn(&options);
    } else if (strcmp(options.method, "kmeans") == 0) {
        runKmeans(&options);
    } else {
        fprintf(stderr, "Method not supported\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
