#include "account.h"

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