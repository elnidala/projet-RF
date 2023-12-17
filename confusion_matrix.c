#include "confusion_matrix.h"

ConfusionMatrix createConfusionMatrix(int classCount) {
    ConfusionMatrix cm;
    cm.classCount = classCount;
    cm.matrix = malloc(classCount * sizeof(int *));
    for (int i = 0; i < classCount; i++) {
        cm.matrix[i] = calloc(classCount, sizeof(int));
    }
    return cm;
}

void updateConfusionMatrix(ConfusionMatrix *cm, int actualClass, int predictedClass) {
    // Adjust for 0-based indexing
    int matrixRow = actualClass - 1; // Adjusting for 0-based indexing
    int matrixColumn = predictedClass - 1; // Adjusting for 0-based indexing

    // Ensure the adjusted indices are within the matrix bounds
    if (matrixRow >= 0 && matrixRow < cm->classCount && 
        matrixColumn >= 0 && matrixColumn < cm->classCount) {
        cm->matrix[matrixRow][matrixColumn]++;
    }
}


void freeConfusionMatrix(ConfusionMatrix *cm) {
    if (cm && cm->matrix) {
        for (int i = 0; i < cm->classCount; i++) {
            free(cm->matrix[i]);
        }
        free(cm->matrix);
    }
}

void printMatrix(const ConfusionMatrix cm) {
    printf("\nConfusion Matrix:\n");

    // Print column headers with appropriate spacing
    printf("          "); // Space for row label
    for (int i = 0; i < cm.classCount; i++) {
        printf("Class%-2d", i + 1);
    }
    printf("\n");

    // Print matrix rows
    for (int i = 0; i < cm.classCount; i++) {
        printf("Class%-4d", i + 1); // Print row label
        for (int j = 0; j < cm.classCount; j++) {
            printf("%7d", cm.matrix[i][j]); // Ensure each matrix cell is aligned
        }
        printf("\n");
    }
}


void printStatistics(const ConfusionMatrix cm) {
    printf("\nStatistics by Class:\n");

    int totalTP = 0, totalFP = 0, totalFN = 0, totalTN = 0;

    for (int i = 0; i < cm.classCount; i++) {
        int TP = cm.matrix[i][i];
        int FP = 0, FN = 0, TN = 0;

        for (int j = 0; j < cm.classCount; j++) {
            if (i != j) {
                FN += cm.matrix[j][i]; // Predicted as i, actual is j
                FP += cm.matrix[i][j]; // Predicted as j, actual is i
            }
        }
        for (int x = 0; x < cm.classCount; x++) {
            for (int y = 0; y < cm.classCount; y++) {
                if (x != i && y != i) {
                    TN += cm.matrix[x][y]; // True negatives
                }
            }
        }

        totalTP += TP;
        totalFP += FP;
        totalFN += FN;
        totalTN += TN;

        double precision = (TP + FP) != 0 ? (double)TP / (TP + FP) : 0;
        double recall = (TP + FN) != 0 ? (double)TP / (TP + FN) : 0;
        double specificity = (TN + FP) != 0 ? (double)TN / (TN + FP) : 0;
        double f1Score = (precision + recall) != 0 ? 2 * (precision * recall) / (precision + recall) : 0;
        double fpr = (FP + TN) != 0 ? (double)FP / (FP + TN) : 0;
        double accuracy = (TP + FP + FN + TN) != 0 ? (double)(TP + TN) / (TP + FP + FN + TN) : 0;

        printf("Class %d: TP = %d, TN = %d, FP = %d, FN = %d, Precision = %.2f, Recall = %.2f, Specificity = %.2f, F1 Score = %.2f, FPR = %.2f, Accuracy = %.2f%%\n",
               i + 1, TP, TN, FP, FN, precision, recall, specificity, f1Score, fpr, accuracy * 100);
    }

    // Overall metrics
    double overallPrecision = (totalTP + totalFP) != 0 ? (double)totalTP / (totalTP + totalFP) : 0;
    double overallRecall = (totalTP + totalFN) != 0 ? (double)totalTP / (totalTP + totalFN) : 0;
    double overallSpecificity = (totalTN + totalFP) != 0 ? (double)totalTN / (totalTN + totalFP) : 0;
    double overallF1Score = (overallPrecision + overallRecall) != 0 ? 2 * (overallPrecision * overallRecall) / (overallPrecision + overallRecall) : 0;
    double overallFPR = (totalFP + totalTN) != 0 ? (double)totalFP / (totalFP + totalTN) : 0;
    double overallAccuracy = (totalTP + totalFP + totalFN + totalTN) != 0 ? (double)(totalTP + totalTN) / (totalTP + totalFP + totalFN + totalTN) : 0;

    printf("\nOverall Metrics:\n");
    printf("Overall Precision = %.2f%%\n", overallPrecision);
    printf("Overall Recall = %.2f\n", overallRecall);
    printf("Overall Specificity = %.2f\n", overallSpecificity);
    printf("Overall F1 Score = %.2f\n", overallF1Score);
    printf("Overall FPR = %.2f\n", overallFPR);
    printf("Overall Accuracy = %.2f%%\n", overallAccuracy * 100);
}


void printDetailedConfusionMatrix(const ConfusionMatrix cm) {
    printMatrix(cm);
    printStatistics(cm);
}

void writeMatrixToFile(const ConfusionMatrix cm, FILE *file) {
    fprintf(file, "\nConfusion Matrix:\n");
    for (int i = 0; i < cm.classCount; i++) {
        for (int j = 0; j < cm.classCount; j++) {
            fprintf(file, "%7d ", cm.matrix[i][j]);
        }
        fprintf(file, "\n");
    }
}

void writeStatisticsToFile(const ConfusionMatrix cm, FILE *file) {
    fprintf(file, "\nStatistics by Class:\n");
    for (int i = 0; i < cm.classCount; i++) {
        // Similar calculations as in printStatistics
        int TP = cm.matrix[i][i];
        int FN = 0, FP = 0, TN = 0;
        for (int j = 0; j < cm.classCount; j++) {
            if (i != j) {
                FN += cm.matrix[i][j];
                FP += cm.matrix[j][i];
            }
            for (int k = 0; k < cm.classCount; k++) {
                if (j != i && k != i) {
                    TN += cm.matrix[j][k];
                }
            }
        }

        double precision = TP + FP == 0 ? 0 : (double)TP / (TP + FP);
        double recall = TP + FN == 0 ? 0 : (double)TP / (TP + FN);
        double f1Score = precision + recall == 0 ? 0 : 2 * (precision * recall) / (precision + recall);

        fprintf(file, "Class %d: TP = %d, TN = %d, FP = %d, FN = %d, Precision = %.2f, Recall = %.2f, F1 Score = %.2f\n",
                i, TP, TN, FP, FN, precision, recall, f1Score);
    }
}

void saveDetailedConfusionMatrixToFile(const ConfusionMatrix cm, const char *filename, const char *title) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    time_t now;
    time(&now);
    char *date = ctime(&now);

    fprintf(file, "Experiment: %s\n", title);
    fprintf(file, "Date: %s\n", date);
    writeMatrixToFile(cm, file);
    writeStatisticsToFile(cm, file);

    fclose(file);
}
