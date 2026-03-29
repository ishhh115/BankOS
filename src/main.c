#include "account.h"

// Forward declarations
void init_bank();
void print_accounts();
int deposit(int account_id, double amount);
int withdraw(int account_id, double amount);
int transfer(int from_id, int to_id, double amount);
double get_balance(int account_id);
void test_rwlock();
void init_semaphore();
void test_semaphore();
void init_queue();
void test_producer_consumer();
void test_deadlock();
void test_bankers();
void print_dashboard(int active_threads, int queue_size);
void run_dashboard_demo();

int main() {
    printf("\n");
    printf("================================================\n");
    printf("   BankOS — Concurrent Bank Transaction System  \n");
    printf("   Built by Isha Parveen                        \n");
    printf("   OS PBL | C + POSIX Threads | 7 Concepts     \n");
    printf("================================================\n\n");

    // Step 1 — Initialize the bank
    init_bank();
    printf("\n");
    print_accounts();

    // Step 2 — Demonstrate race condition
    printf("\n================================================\n");
    printf("   STEP 1: Race Condition Demo                  \n");
    printf("================================================\n");
    printf("Run './demo' to see race condition live!\n");

    // Step 3 — Reader Writer Lock
    printf("\n================================================\n");
    printf("   STEP 2: Reader-Writer Lock                   \n");
    printf("================================================\n");
    test_rwlock();

    // Step 4 — Semaphore
    printf("\n================================================\n");
    printf("   STEP 3: Semaphore Throttling                 \n");
    printf("================================================\n");
    test_semaphore();

    // Step 5 — Producer Consumer
    printf("\n================================================\n");
    printf("   STEP 4: Producer-Consumer Queue              \n");
    printf("================================================\n");
    test_producer_consumer();

    // Step 6 — Deadlock Prevention
    printf("\n================================================\n");
    printf("   STEP 5: Deadlock Prevention                  \n");
    printf("================================================\n");
    test_deadlock();

    // Step 7 — Banker's Algorithm
    printf("\n================================================\n");
    printf("   STEP 6: Banker's Algorithm                   \n");
    printf("================================================\n");
    test_bankers();

    // Step 8 — Live Dashboard
    printf("\n================================================\n");
    printf("   STEP 7: Live Dashboard                       \n");
    printf("================================================\n");
    run_dashboard_demo();

    // Final state
    printf("\n================================================\n");
    printf("   FINAL BANK STATE                             \n");
    printf("================================================\n");
    print_accounts();

    printf("\n================================================\n");
    printf("   BankOS Complete! All 7 concepts demonstrated \n");
    printf("================================================\n\n");

    return 0;
}


/*#include "account.h"

void run_dashboard_demo();
void test_bankers();
int transfer(int from_id, int to_id, double amount);
void test_deadlock();
void init_queue();
void test_producer_consumer();
void init_semaphore();
void test_semaphore();
void init_bank();
void test_rwlock();
void print_accounts();
int deposit(int account_id, double amount);
int withdraw(int account_id, double amount);
double get_balance(int account_id);


int main() {
    init_bank();
    test_rwlock();
    test_semaphore();
    test_producer_consumer();
    test_deadlock();
    test_bankers();
    run_dashboard_demo();
    
    printf("=== INITIAL BALANCES ===\n");
    print_accounts();
    
    printf("\n=== TRANSACTIONS ===\n");
    deposit(0, 500.0);
    deposit(1, 1000.0);
    withdraw(0, 200.0);
    withdraw(2, 5000.0);
    
    printf("\n=== FINAL BALANCES ===\n");
    print_accounts();
    
    return 0;
}

*/