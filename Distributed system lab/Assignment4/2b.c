#include <stdio.h>
#include <stdlib.h>

int logical_clock = 0;

void send_request(int process_id) {
    logical_clock++;
    printf("Process %d sends request at time %d\n", process_id, logical_clock);
}

void receive_request(int process_id, int sender_id, int sender_time) {
    logical_clock = (sender_time > logical_clock) ? sender_time + 1 : logical_clock + 1;
    printf("Process %d receives request from Process %d at time %d\n", process_id, sender_id, logical_clock);
}

void grant_permission(int process_id, int requester_id) {
    logical_clock++;
    printf("Process %d grants permission to Process %d at time %d\n", process_id, requester_id, logical_clock);
}

void enter_critical_section(int process_id) {
    printf("Process %d enters critical section at time %d\n", process_id, logical_clock);
}

void exit_critical_section(int process_id) {
    printf("Process %d exits critical section at time %d\n", process_id, logical_clock);
}

int main() {
    // Simulate two processes
    int process_1 = 1;
    int process_2 = 2;

    send_request(process_1);
    receive_request(process_2, process_1, logical_clock);
    grant_permission(process_2, process_1);
    enter_critical_section(process_1);
    exit_critical_section(process_1);

    send_request(process_2);
    receive_request(process_1, process_2, logical_clock);
    grant_permission(process_1, process_2);
    enter_critical_section(process_2);
    exit_critical_section(process_2);

    return 0;
}
