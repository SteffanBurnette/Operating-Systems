#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FARMERS 5  // Number of farmers from each village

// Mutex to prevent deadlock and ensure one farmer at a time on the bridge
pthread_mutex_t bridge_lock;
pthread_mutex_t output_lock;  // To control access to the output screen

// Function to simulate the action of a farmer traveling on the bridge
void* farmer_crossing(void* arg) {
    int village = *((int*)arg);  // 0 for North, 1 for South
    int farmer_id = rand() % 100;  // Random ID for the farmer

    // Acquire the lock for the bridge to ensure only one farmer on the bridge at a time
    pthread_mutex_lock(&bridge_lock);

    // Print when the farmer can cross (access the output)
    pthread_mutex_lock(&output_lock);
    if (village == 0) {
        printf("North Tunbridge #%d farmer can cross the bridge\n", farmer_id);
    } else {
        printf("South Tunbridge #%d farmer can cross the bridge\n", farmer_id);
    }
    pthread_mutex_unlock(&output_lock);

    // Farmer is traveling on the bridge, sleep for a random time (up to 3 seconds)
    pthread_mutex_lock(&output_lock);
    if (village == 0) {
        printf("North Tunbridge #%d is traveling on the bridge...\n", farmer_id);
    } else {
        printf("South Tunbridge #%d is traveling on the bridge...\n", farmer_id);
    }
    pthread_mutex_unlock(&output_lock);

    // Sleep for a random period (up to 3 seconds)
    sleep(rand() % 4);

    // Farmer has crossed the bridge, print message and release the lock
    pthread_mutex_lock(&output_lock);
    if (village == 0) {
        printf("North Tunbridge #%d farmer has left the bridge\n", farmer_id);
    } else {
        printf("South Tunbridge #%d farmer has left the bridge\n", farmer_id);
    }
    pthread_mutex_unlock(&output_lock);

    // Release the bridge lock to allow the next farmer
    pthread_mutex_unlock(&bridge_lock);

    return NULL;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&bridge_lock, NULL);
    pthread_mutex_init(&output_lock, NULL);

    // Seed the random number generator for randomness in sleep durations
    srand(time(NULL));

    // Create arrays for threads representing farmers from each village
    pthread_t north_farmers[NUM_FARMERS];
    pthread_t south_farmers[NUM_FARMERS];
    int north_village = 0;
    int south_village = 1;

    // Create threads for northbound farmers
    for (int i = 0; i < NUM_FARMERS; i++) {
        pthread_create(&north_farmers[i], NULL, farmer_crossing, &north_village);
    }

    // Create threads for southbound farmers
    for (int i = 0; i < NUM_FARMERS; i++) {
        pthread_create(&south_farmers[i], NULL, farmer_crossing, &south_village);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_FARMERS; i++) {
        pthread_join(north_farmers[i], NULL);
        pthread_join(south_farmers[i], NULL);
    }

    // Destroy the mutexes
    pthread_mutex_destroy(&bridge_lock);
    pthread_mutex_destroy(&output_lock);

    return 0;
}
