#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "emojicode_generator.h"

void writeOutputToCSV() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char filename[20];
    strftime(filename, sizeof(filename), "%Y%m%d.csv", tm);

    FILE *csvFile = fopen(filename, "w");
    if (!csvFile) {
        printf("Error opening %s\n", filename);
        return;
    }

    // Compile the Emojicode file
    system("emojicodec data.emojic");

    // Run the compiled Emojicode program and capture its output
    FILE *emojicodeOutput = popen("./data", "r");
    if (!emojicodeOutput) {
        printf("Error running Emojicode program\n");
        fclose(csvFile);
        return;
    }

    char name[256], creditCard[256];
    while (fgets(name, sizeof(name), emojicodeOutput) && fgets(creditCard, sizeof(creditCard), emojicodeOutput)) {
        // Remove newline characters
        name[strcspn(name, "\n")] = 0;
        creditCard[strcspn(creditCard, "\n")] = 0;

        // Only write to CSV if credit card is not empty
        if (creditCard[0] != '\0') {
            fprintf(csvFile, "%s,%s\n", name, creditCard);
        }
    }

    pclose(emojicodeOutput);
    fclose(csvFile);
    printf("Output written to %s\n", filename);
}

int main() {
    // First, generate the Emojicode file
    generateEmojicodeFile("data.json", "data.emojic");

    // Now, write the processed output to a CSV file
    writeOutputToCSV();
    return 0;
}