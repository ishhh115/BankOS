#include "account.h"

void init_bank();
void print_accounts();
int deposit(int account_id, double amount);
int withdraw(int account_id, double amount);
double get_balance(int account_id);

int main() {
    init_bank();
    
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