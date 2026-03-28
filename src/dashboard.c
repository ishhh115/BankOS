#include "account.h"
#include <windows.h>

#define ANSI_GREEN   "\033[32m"
#define ANSI_RED     "\033[31m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_RESET   "\033[0m"
#define CLEAR_SCREEN "\033[2J\033[H"

const char* balance_color(double balance);
int deposit(int account_id, double amount);
int withdraw(int account_id, double amount);
int transfer(int from_id, int to_id, double amount);

const char* balance_color(double balance) {
    if (balance > 3000) return ANSI_GREEN;
    if (balance > 1000) return ANSI_YELLOW;
    return ANSI_RED;
}

void print_dashboard(int active_threads, int queue_size) {
    printf(CLEAR_SCREEN);
    
    printf(ANSI_BOLD ANSI_CYAN);
    printf("╔══════════════════════════════════════════╗\n");
    printf("║           🏦  BankOS Dashboard           ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf(ANSI_RESET);
    
    printf(ANSI_BOLD "\nACCOUNTS:\n" ANSI_RESET);
    printf("─────────────────────────────────────────\n");
    
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        pthread_rwlock_rdlock(&accounts[i].rwlock);
        double balance = accounts[i].balance;
        char name[64];
        strncpy(name, accounts[i].name, 64);
        int tx_count = accounts[i].transaction_count;
        pthread_rwlock_unlock(&accounts[i].rwlock);
        
        printf(" [%02d] %-20s %s Rs.%-10.2f" ANSI_RESET 
               " | Transactions: %d\n",
               accounts[i].id,
               name,
               balance_color(balance),
               balance,
               tx_count);
    }
    
    printf("─────────────────────────────────────────\n");
    printf(ANSI_BOLD "\nSYSTEM STATUS:\n" ANSI_RESET);
    printf(" Active Threads : " ANSI_GREEN "%d\n" ANSI_RESET, active_threads);
    printf(" Queue Size     : " ANSI_YELLOW "%d\n" ANSI_RESET, queue_size);
    printf(" Max Tellers    : " ANSI_CYAN "%d\n" ANSI_RESET, MAX_TELLERS);
    printf("\n" ANSI_BOLD "Press Ctrl+C to stop\n" ANSI_RESET);
}

void run_dashboard_demo() {
    printf("\n=== DASHBOARD DEMO ===\n");
    printf("Showing live dashboard for 3 seconds...\n\n");
    Sleep(1000);
    
    print_dashboard(5, tx_queue.count);
    Sleep(1000);
    
    deposit(0, 2500.0);
    deposit(2, 1500.0);
    withdraw(9, 4000.0);
    
    print_dashboard(3, tx_queue.count);
    Sleep(1000);
    
    transfer(1, 5, 800.0);
    transfer(3, 7, 300.0);
    
    print_dashboard(0, tx_queue.count);
    
    printf("\n=== DASHBOARD DEMO DONE ===\n");
}