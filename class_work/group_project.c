// The Link of the raw data log file: https://github.com/logpai/loghub/blob/master/Windows/Windows_2k.log_structured.csv
// Feature of the dataset that I will be working with:
// Is a .csv
// Has 2000 rows of data
// 6 columns per rows
// column categories: LineId, Date, Time, Level, Component, Content

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define MAX_UNIQUE      100  // Adjusted to account for a higher number of unique items (dates, times, components, levels)

// Structure to hold the log entries
typedef struct {
    int  LineId;
    char Date[11];      // YYYY-MM-DD
    char Time[9];       // HH:MM:SS
    char Level[10];     // Log level like INFO, ERROR
    char Component[50]; // Component name
    char Content[500];  // Content of the log
} LogEntry;

// Simple name→count bucket
typedef struct {
    char  name[50]; //atmost the name can be 50 characters
    size_t count;
} Counter;


// Parse one CSV line into log_entry; returns 1 on success
int parse_log_line(char *line, LogEntry *log_entry) {
    char *token;

    // Getting LineId
    //breaks the input string line into smaller tokens (substrings) based on the delimiter ,
    //After each strtok call, the code checks if token is NULL. If it is NULL, it means 
    //that either the CSV line is malformed or that there are missing fields. In this case, 
    //the function returns 0 to indicate failure.
    token = strtok(line, ",");
    if (token == NULL) return 0;
    log_entry->LineId = atoi(token);

    // Getting Date
    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(log_entry->Date, token, sizeof(log_entry->Date)-1);

    // Getting Time
    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(log_entry->Time, token, sizeof(log_entry->Time)-1);

    // Getting Level
    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(log_entry->Level, token, sizeof(log_entry->Level)-1);

    // Getting Component
    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(log_entry->Component, token, sizeof(log_entry->Component)-1);

    // Get Content (remaining part of the line)
    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(log_entry->Content, token, sizeof(log_entry->Content)-1);

    return 1; // Successfully parsed
}


// Look up key in counters[0..*n), bump its count, or add a new bucket
//The incr_counter function checks if a given key (e.g., date, time, level) exists
//in an array of Counter structs. If the key exists, it increments its count. If the key 
//doesn't exist, it adds a new counter with a count of 1
void incr_counter(Counter *counters, size_t *n, size_t max, const char *key) {
    for (size_t i = 0; i < *n; i++) {
        if (strcmp(counters[i].name, key) == 0) {
            counters[i].count++;
            return;
        }
    }
    if (*n < max) {
        strncpy(counters[*n].name, key, sizeof(counters[*n].name)-1);
        counters[*n].name[sizeof(counters[*n].name)-1] = '\0';
        counters[*n].count = 1;
        (*n)++;
    }
}

// Function to process the CSV file using mmap
void process_log_file(const char *filename) {
    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    // Get the file size
    // moves the file pointer to the end of the file and returns the size of the file.
    //If this fails, it prints an error and exits.
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Error getting file size");
        close(fd);
        return;
    }
    lseek(fd, 0, SEEK_SET);  // Reset to the beginning

    // Map the file into memory
    //utilizing mmap
    //maps the entire file into memory so that it can be accessed directly as a large 
    //block of memory.
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return;
    }
    close(fd); //Closes the file

    // Prepare counters for date, time, level, and component counts
    Counter date_counts[MAX_UNIQUE] = {0};
    Counter time_counts[MAX_UNIQUE] = {0};
    Counter level_counts[MAX_UNIQUE] = {0};
    Counter component_counts[MAX_UNIQUE] = {0};

    size_t n_dates = 0, n_times = 0, n_levels = 0, n_components = 0;
    size_t total = 0;

    char *line_start = file_data;
    char *line_end = strchr(line_start, '\n');
    LogEntry log_entry;
    char line_buffer[MAX_LINE_LENGTH];

    // Process the file line by line
    while (line_end != NULL) {
        size_t len = line_end - line_start;
        if (len >= sizeof(line_buffer)) len = sizeof(line_buffer)-1;
        strncpy(line_buffer, line_start, len);
        line_buffer[len] = '\0';  // Safe null-termination

        if (parse_log_line(line_buffer, &log_entry)) {
            total++;
            incr_counter(date_counts, &n_dates, MAX_UNIQUE, log_entry.Date);
            incr_counter(time_counts, &n_times, MAX_UNIQUE, log_entry.Time);
            incr_counter(level_counts, &n_levels, MAX_UNIQUE, log_entry.Level);
            incr_counter(component_counts, &n_components, MAX_UNIQUE, log_entry.Component);

            // Output the full row information
            printf("LineId: %d, Date: %s, Time: %s, Level: %s, Component: %s, Content: %s\n",
                log_entry.LineId, log_entry.Date, log_entry.Time, log_entry.Level, log_entry.Component, log_entry.Content);
        } else {
            fprintf(stderr, "Error parsing line %zu\n", total+1);
        }

        // Move to the next line
        line_start = line_end + 1;
        line_end = strchr(line_start, '\n');
    }

    // Print the summary statistics
    printf("\nProcessed %zu log entries.\n\n", total);

    printf("=== Date Counts ===\n");
    for (size_t i = 0; i < n_dates; i++) {
        printf("  %-10s: %zu\n", date_counts[i].name, date_counts[i].count);
    }

    printf("\n=== Time Counts ===\n");
    for (size_t i = 0; i < n_times; i++) {
        printf("  %-8s: %zu\n", time_counts[i].name, time_counts[i].count);
    }

    printf("\n=== Log Level Counts ===\n");
    for (size_t i = 0; i < n_levels; i++) {
        printf("  %-10s: %zu\n", level_counts[i].name, level_counts[i].count);
    }

    printf("\n=== Component Counts ===\n");
    for (size_t i = 0; i < n_components; i++) {
        printf("  %-20s: %zu\n", component_counts[i].name, component_counts[i].count);
    }

    // Clean up mmap
    munmap(file_data, file_size);
}




int main() {
    const char *log_file = "/home/kali/Downloads/Windows_2k.log_structured.csv";  
    process_log_file(log_file);
    return 0;
}

//This function processes a CSV log file efficiently using memory mapping (mmap), 
//parses each line to extract data (like Date, Time, etc.), counts the occurrences of 
//unique entries for those fields, and outputs the results. It uses memory-mapping to improve
//performance when handling large files, avoiding the need to load the entire file into memory 
//at once.