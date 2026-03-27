#include "account.h"
#define N_THREADS 10
#define N_RESOURCES 3

int available[N_RESOURCES];
int maximum[N_THREADS][N_RESOURCES];
int allocation[N_THREADS][N_RESOURCES];
int need[N_THREADS][N_RESOURCES];

void init_banker() {
    available[0] = 3;
    available[1] = 3;
    available[2] = 2;
    
    int max_data[N_THREADS][N_RESOURCES] = {
        {7, 5, 3}, {3, 2, 2}, {9, 0, 2},
        {2, 2, 2}, {4, 3, 3}, {3, 3, 2},
        {5, 4, 3}, {2, 2, 1}, {4, 3, 2},
        {3, 2, 2}
    };
    
    int alloc_data[N_THREADS][N_RESOURCES] = {
        {0, 1, 0}, {2, 0, 0}, {3, 0, 2},
        {2, 1, 1}, {0, 0, 2}, {0, 0, 1},
        {2, 1, 1}, {0, 0, 0}, {0, 1, 0},
        {1, 0, 0}
    };
    
    for (int i = 0; i < N_THREADS; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            maximum[i][j] = max_data[i][j];
            allocation[i][j] = alloc_data[i][j];
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
    printf("[BANKER] Initialized with %d threads, %d resource types\n",
           N_THREADS, N_RESOURCES);
}

int is_safe_state() {
    int work[N_RESOURCES];
    int finish[N_THREADS];
    
    for (int i = 0; i < N_RESOURCES; i++)
        work[i] = available[i];
    
    for (int i = 0; i < N_THREADS; i++)
        finish[i] = 0;
    
    int safe_sequence[N_THREADS];
    int count = 0;
    
    int found = 1;
    while (found) {
        found = 0;
        for (int i = 0; i < N_THREADS; i++) {
            if (finish[i]) continue;
            
            int can_satisfy = 1;
            for (int j = 0; j < N_RESOURCES; j++) {
                if (need[i][j] > work[j]) {
                    can_satisfy = 0;
                    break;
                }
            }
            
            if (can_satisfy) {
                for (int j = 0; j < N_RESOURCES; j++)
                    work[j] += allocation[i][j];
                finish[i] = 1;
                safe_sequence[count++] = i;
                found = 1;
            }
        }
    }
    
    if (count == N_THREADS) {
        printf("[BANKER] SAFE STATE! Safe sequence: ");
        for (int i = 0; i < N_THREADS; i++)
            printf("T%d ", safe_sequence[i]);
        printf("\n");
        return 1;
    }
    
    printf("[BANKER] UNSAFE STATE! No safe sequence exists.\n");
    return 0;
}

int request_resources(int thread_id, int request[]) {
    printf("[BANKER] Thread %d requesting resources: [%d %d %d]\n",
           thread_id, request[0], request[1], request[2]);
    
    for (int j = 0; j < N_RESOURCES; j++) {
        if (request[j] > need[thread_id][j]) {
            printf("[BANKER] ERROR: Request exceeds maximum need!\n");
            return -1;
        }
    }
    
    for (int j = 0; j < N_RESOURCES; j++) {
        if (request[j] > available[j]) {
            printf("[BANKER] Thread %d must wait — not enough resources\n",
                   thread_id);
            return 0;
        }
    }
    
    for (int j = 0; j < N_RESOURCES; j++) {
        available[j]          -= request[j];
        allocation[thread_id][j] += request[j];
        need[thread_id][j]    -= request[j];
    }
    
    if (is_safe_state()) {
        printf("[BANKER] Request GRANTED to Thread %d\n", thread_id);
        return 1;
    } else {
        for (int j = 0; j < N_RESOURCES; j++) {
            available[j]          += request[j];
            allocation[thread_id][j] -= request[j];
            need[thread_id][j]    += request[j];
        }
        printf("[BANKER] Request DENIED — would cause unsafe state!\n");
        return 0;
    }
}
void test_bankers() {
    printf("\n=== BANKER'S ALGORITHM TEST ===\n\n");
    
    init_banker();
    
    printf("\n--- Initial Safety Check ---\n");
    is_safe_state();
    
    printf("\n--- Test 1: Safe Request ---\n");
    int request1[] = {0, 1, 0};
    request_resources(1, request1);
    
    printf("\n--- Test 2: Unsafe Request ---\n");
    int request2[] = {3, 3, 0};
    request_resources(4, request2);
    
    printf("\n=== BANKER'S ALGORITHM TEST DONE ===\n");
}