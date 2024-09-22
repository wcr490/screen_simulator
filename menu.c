#include "draw.h"

#define LIST_LEN 4

int process_flag = 1, direction = 1;
uint16_t x = 5, y = 0, trg = 10;
SOCKET sock = INVALID_SOCKET;

void process();
void render();
void run_to_close(uint16_t* cur_y, uint16_t* cur_trg);

int main() {
    if (sock_init()) {
        printf("Failed to initialize Winsock");
        return 1;
    } 
    if (connect_server("127.0.0.1", 8080, &sock)) {
        printf("Failed to connect");
        return 2;
    }
    printf("Connected to server!\n");

    while (1) {
        process();
        Sleep(5000);
    }

    sock_delete(sock);
    return 0;
}

void process() {
    if (process_flag) {
        process_flag = 0;
        if (direction) {
            trg += 10;
        } else {
            if (trg >= 10) 
                trg -= 10;
        }
    }
    render();
}

int pace = 15;
void render() {
    cleanup_buffer(sock);
    for (int i = 0; i < LIST_LEN; i++) {
        if (y + i*pace + 3 > 127 || y + i*pace <=0)
        {
            cleanup_buffer(sock);
            direction = ~direction;
            break;
        }
        
        draw_rectangle_buffer(sock, x, y + i * 15, 3, 10);
    }
    MSG_T msg[5];
    update_buffer_msg(msg);
    __send_msg(sock, msg);
    
    run_to_close(&y, &trg);

    process_flag = 1;  // Reset flag to allow process to run again
}

void run_to_close(uint16_t* cur_y, uint16_t* cur_trg) {
    if (*cur_y > *cur_trg) {
        *cur_y -= 2;  // Adjust movement speed
    } else if (*cur_y < *cur_trg) {
        *cur_y += 2;  // Adjust movement speed
    }
}
