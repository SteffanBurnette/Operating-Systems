// The Link of the raw data log file: https://github.com/logpai/loghub/blob/master/Windows/Windows_2k.log_structured.csv
// Feature of the dataset that I will be working with:
// Is a .csv
// Has 2000 rows of data
// 6 columns per row
// column categories: LineId, Date, Time, Level, Component, Content

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Structure to hold the log entry
typedef struct {
    int LineId;
    char Date[11];   // YYYY-MM-DD
    char Time[9];    // HH:MM:SS
    char Level[10];  // Log level like INFO, ERROR
    char Component[50]; // Component name
    char Content[500]; // Content of the log
} LogEntry;

// Function to parse a CSV line
int parse_log_line(char *line, LogEntry *log_entry) {
    char *token;

    // Get LineId
    token = strtok(line, ",");
    if (token == NULL) return 0; // Error in parsing
    log_entry->LineId = atoi(token);  // Convert to integer

    // Get Date
    token = strtok(NULL, ",");
    if (token == NULL) return 0; // Error in parsing
    strncpy(log_entry->Date, token, sizeof(log_entry->Date));

    // Get Time
    token = strtok(NULL, ",");
    if (token == NULL) return 0; // Error in parsing
    strncpy(log_entry->Time, token, sizeof(log_entry->Time));

    // Get Level
    token = strtok(NULL, ",");
    if (token == NULL) return 0; // Error in parsing
    strncpy(log_entry->Level, token, sizeof(log_entry->Level));

    // Get Component
    token = strtok(NULL, ",");
    if (token == NULL) return 0; // Error in parsing
    strncpy(log_entry->Component, token, sizeof(log_entry->Component));

    // Get Content (remaining part of the line)
    token = strtok(NULL, ",");
    if (token == NULL) return 0; // Error in parsing
    strncpy(log_entry->Content, token, sizeof(log_entry->Content));

    return 1; // Successfully parsed
}

// Function to process the CSV file
void process_log_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    LogEntry log_entry;

    // Read file line by line
    while (fgets(line, sizeof(line), file)) {
        // Strip newline character if it exists
        line[strcspn(line, "\n")] = '\0';

        // Parse the line into LogEntry structure
        if (parse_log_line(line, &log_entry)) {
            // Process the log entry (for example, display it)
            printf("LineId: %d\n", log_entry.LineId);
            printf("Date: %s\n", log_entry.Date);
            printf("Time: %s\n", log_entry.Time);
            printf("Level: %s\n", log_entry.Level);
            printf("Component: %s\n", log_entry.Component);
            printf("Content: %s\n\n", log_entry.Content);
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }

    fclose(file);
}

int main() {
    const char *log_file = "c:/Users/Burne/Downloads/Windows_logs.log_structured.csv";  // Replace with your actual file path
    process_log_file(log_file);
    return 0;
}
