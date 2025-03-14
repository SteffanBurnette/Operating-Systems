import threading
import time
import math
import os

# Function to count the number of divisors for a given integer
def count_divisors(n):
    count = 0
    # We only need to iterate to sqrt(n) to count factors in pairs
    for i in range(1, int(math.sqrt(n)) + 1):
        if n % i == 0:
            count += 1 if (i * i == n) else 2  # Count square roots only once
    return count

# Worker function for each thread
def worker(start, end, result_list, index):
    # For each integer in the assigned subrange,
    # count the divisors and keep track of the number with the maximum count.
    max_div_count = 0
    number_with_max = start
    for num in range(start, end + 1):
        d = count_divisors(num)
        if d > max_div_count:
            max_div_count = d
            number_with_max = num
    # Save the result in the shared list at the index for this thread
    result_list[index] = (number_with_max, max_div_count)

# Main function to run the calculation
def find_number_with_max_divisors(num_threads=1):
    # Define the range
    lower_bound = 1
    upper_bound = 10000

    # Calculate subrange size for each thread
    total_numbers = upper_bound - lower_bound + 1
    chunk_size = total_numbers // num_threads

    # Create a list to store results from each thread
    results = [None] * num_threads
    threads = []

    # Record the start time
    start_time = time.time()

    # Start each thread with its chunk of numbers
    for i in range(num_threads):
        start_range = lower_bound + i * chunk_size
        # Make sure the last thread covers any remainder in the range
        end_range = lower_bound + (i + 1) * chunk_size - 1 if i < num_threads - 1 else upper_bound
        t = threading.Thread(target=worker, args=(start_range, end_range, results, i))
        threads.append(t)
        t.start()

    # Wait for all threads to complete
    for t in threads:
        t.join()

    # Find the overall number with the maximum divisor count from all threads
    overall_number = None
    overall_max_divisors = 0
    for number, div_count in results:
        if div_count > overall_max_divisors:
            overall_max_divisors = div_count
            overall_number = number

    # Calculate elapsed time
    elapsed_time = time.time() - start_time

    # Return the result and the elapsed time
    return overall_number, overall_max_divisors, elapsed_time

# Entry point: change num_threads parameter to test with different number of threads
if __name__ == '__main__':
    # For testing purposes, run with 1 thread and then with all available threads.
    # You may modify 'available_threads' as per your machine's capacity.
    
    # Option 1: Run with 1 thread
    result_single, div_count_single, time_single = find_number_with_max_divisors(num_threads=1)
    print("Single-threaded:")
    print("Number with most divisors:", result_single)
    print("Divisor count:", div_count_single)
    print("Time taken: {:.6f} seconds".format(time_single))
    print("-" * 40)
    
    # Option 2: Run with multiple threads (e.g., 8 threads)
    # Replace 8 with the number of available threads on your machine if needed.
    available_threads = 8
    result_multi, div_count_multi, time_multi = find_number_with_max_divisors(num_threads=available_threads)
    print("Multi-threaded ({} threads):".format(available_threads))
    print("Number with most divisors:", result_multi)
    print("Divisor count:", div_count_multi)
    print("Time taken: {:.6f} seconds".format(time_multi))


print(f"This is my cpu count: {os.cpu_count()}")