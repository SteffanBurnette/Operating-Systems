import random
import argparse
import sys

def fcfs(requests, head):
    """First-Come, First-Served: service requests in order of arrival:""" 
    total = abs(head - requests[0])
    for i in range(len(requests)-1):
        total += abs(requests[i] - requests[i+1])
    return total

def scan(requests, head, max_cyl=9999):
    """
    SCAN (Elevator): head moves toward the lowest cylinder (0), servicing requests,
    then reverses direction and services remaining requests.
    """
    left = sorted([r for r in requests if r <= head], reverse=True)
    right = sorted([r for r in requests if r > head])
    seq = left + right
    total = abs(head - seq[0]) if seq else 0
    for i in range(len(seq)-1):
        total += abs(seq[i] - seq[i+1])
    return total

def cscan(requests, head, max_cyl=9999):
    """
    C-SCAN: Head moves toward 0, servicing requests, then jumps
    to the max cylinder without servicing, and continues servicing in the same direction.
    The jump itself is not counted in head movement.
    """
    left = sorted([r for r in requests if r <= head], reverse=True)
    right = sorted([r for r in requests if r > head], reverse=True)
    total = 0
    # Service left side
    if left:
        total += abs(head - left[0])
        for i in range(len(left)-1):
            total += abs(left[i] - left[i+1])
        total += abs(left[-1] - 0)  # move to cylinder 0
    else:
        total += abs(head - 0)
    # Jump to max cylinder (not counted), then service right side
    if right:
        total += abs(max_cyl - right[0])
        for i in range(len(right)-1):
            total += abs(right[i] - right[i+1])
    return total

def main():
    # Simulate command-line argument for demonstration
    sys.argv = ['disk_sched.py', '5000']
    parser = argparse.ArgumentParser(description="Disk scheduling algorithms")
    parser.add_argument("initial", type=int, help="Initial head position (0-9999)")
    args = parser.parse_args()
    head = args.initial

    # Generate reproducible random requests
    random.seed(0)
    requests = [random.randint(0, 9999) for _ in range(1000)]

    # Calculate head movements
    fcfs_mov = fcfs(requests, head)
    scan_mov = scan(requests, head)
    cscan_mov = cscan(requests, head)

    # Display results in a table
    print(f"{'Algorithm':<10} | {'Total Head Movement'}")
    print("-" * 33)
    print(f"{'FCFS':<10} | {fcfs_mov}")
    print(f"{'SCAN':<10} | {scan_mov}")
    print(f"{'C-SCAN':<10} | {cscan_mov}")

if __name__ == "__main__":
    main()

