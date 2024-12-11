#include <stdio.h>
#include <jansson.h>
#include "emojicode_generator.h"

void generateEmojicodeFile(const char* jsonFileName, const char* emojicodeFileName) {
    FILE *jsonFile = fopen(jsonFileName, "r");
    FILE *emojicodeFile = fopen(emojicodeFileName, "w");

    if (!jsonFile || !emojicodeFile) {
        printf("Error opening file\n");
        return;
    }

    json_error_t error;
    json_t *jsonData = json_loadf(jsonFile, 0, &error);
    if (!jsonData) {
        printf("Error parsing JSON: %s\n", error.text);
        return;
    }

    // Write the Emojicode header
    fprintf(emojicodeFile, "🏁 🍇\n");

    // Process the JSON array, Save the name and credit card number to the Emojicode 
    // file after filtering out records without credit card numbers
    size_t index;
    json_t *value;
    json_array_foreach(jsonData, index, value) {
        const char *name = json_string_value(json_object_get(value, "name"));
        const char *creditCard = json_string_value(json_object_get(value, "creditcard"));
        
        // Only include records with credit card numbers in the Emojicode source
        if (creditCard) {
            fprintf(emojicodeFile, "  😀 🔤%s🔤❗️\n", name);
            fprintf(emojicodeFile, "  😀 🔤%s🔤❗️\n", creditCard);
        }
    }

    // Write the closing for the Emojicode file
    fprintf(emojicodeFile, "🍉\n");

    fclose(jsonFile);
    fclose(emojicodeFile);
    json_decref(jsonData);
    printf("Emojicode source file generated: %s\n", emojicodeFileName);
}