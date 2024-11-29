#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Function to send a character to the server bit by bit
void send_char(pid_t server_pid, unsigned char c) {
    int i = 0; // Bit index
    while (i < 8) {
        if (c & (1 << i)) {
            if (kill(server_pid, SIGUSR1) == -1) {
                perror("kill");
                exit(EXIT_FAILURE);
            }
        } else {
            if (kill(server_pid, SIGUSR2) == -1) {
                perror("kill");
                exit(EXIT_FAILURE);
            }
        }

        // Wait for acknowledgment from the server
        usleep(100);

        i++; // Increment the bit index
    }
}

// Function to send a string to the server
void send_string(pid_t server_pid, const char *str) {
    while (*str) {
        send_char(server_pid, *str);
        str++;
    }
    send_char(server_pid, '\0'); // Null-terminate the string
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_pid> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t server_pid = atoi(argv[1]);
    const char *message = argv[2];

    send_string(server_pid, message);

    return 0;
}