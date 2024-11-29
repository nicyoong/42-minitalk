#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static volatile sig_atomic_t g_received_bit = 0; // Global variable to track received bit

// Signal handler for receiving bits
void handle_signal(int signo, siginfo_t *info, void *context) {
    (void)context;
    static unsigned char c = 0; // Current character being constructed
    static int bit_count = 0;   // Bit count for the current character
    static pid_t client_pid = 0;

    if (client_pid == 0) // Set the client PID on the first signal
        client_pid = info->si_pid;

    // Accumulate the bits received
    if (signo == SIGUSR1)
        c |= (1 << bit_count);
    bit_count++;

    // If 8 bits (1 byte) are received
    if (bit_count == 8) {
        if (c == '\0') { // End of message
            write(1, "\n", 1); // Print a newline
            kill(client_pid, SIGUSR1); // Acknowledge end of message
            client_pid = 0; // Reset for the next message
        } else {
            write(1, &c, 1); // Print the character
        }
        c = 0;
        bit_count = 0;
    }

    // Acknowledge receipt of the bit
    kill(client_pid, SIGUSR1);
}

int main(void) {
    struct sigaction sa;

    // Print server PID
    pid_t pid = getpid();
    printf("Server PID: %d\n", pid);

    // Set up the signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);

    // Register signals
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Loop indefinitely to wait for signals
    while (1)
        pause(); // Suspend until a signal is caught

    return 0;
}