#include <stdio.h>
#include <string.h>
#define N 5
typedef struct {
    char plates[N][10]; 
    int current;        
} ParkingLot;

typedef struct {
    char plates[N][10]; 
    int front, rear;   
    int count;         
} WaitQueue;

void init_parking(ParkingLot *park) {
    park->current = 0;
    memset(park->plates, 0, sizeof(park->plates)); 
}

void init_wait_queue(WaitQueue *q) {
    q->front = q->rear = 0;
    q->count = 0;
    memset(q->plates, 0, sizeof(q->plates)); 
}

int enqueue(WaitQueue *q, const char *plate) {
    if (q->count >= N) {
        return 0; 
    }
    strcpy(q->plates[q->rear], plate);
    q->rear = (q->rear + 1) % N;
    q->count++;
    return 1;
}

int dequeue(WaitQueue *q, char *plate) {
    if (q->count <= 0) {
        return 0; 
    }
    strcpy(plate, q->plates[q->front]);
    q->front = (q->front + 1) % N;
    q->count--;
    return 1;
}

int is_wait_queue_empty(WaitQueue *q) {
    return q->count == 0;
}

int remove_car(ParkingLot *park, WaitQueue *wait, const char *plate) {
    int pos = -1;
    int i;
    for ( i = 0; i < park->current; i++) {
        if (strcmp(park->plates[i], plate) == 0) {
            pos = i;
            break;
        }
    }
    
    if (pos == -1) {
        return 0; 
    }

    for (i = pos; i < park->current - 1; i++) {
        strcpy(park->plates[i], park->plates[i + 1]);
    }
    park->current--;

    char car[10];
    while (park->current < N && !is_wait_queue_empty(wait)) {
        dequeue(wait, car);
        strcpy(park->plates[park->current], car);
        park->current++;
    }

    return 1;
}

int main() {
    ParkingLot park;
    init_parking(&park);

    WaitQueue wait;
    init_wait_queue(&wait);

    int cmd;
    char plate[10];
    int i, idx; 

    do {
        printf("\n输入指令:(1:进场 2:出场 3:显示停车场 4:显示候车场 0:退出)\n");
        scanf("%d", &cmd);

        switch (cmd) {
            case 1:
                printf("请输入入场车的车牌号: ");
                scanf("%s", plate);
                if (park.current < N) {
                    strcpy(park.plates[park.current], plate);
                    park.current++;
                    printf("车辆 %s 已停入停车场\n", plate);
                } else {
                    if (enqueue(&wait, plate)) {
                        printf("车辆 %s 已进入候车场\n", plate);
                    } else {
                        printf("候车场已满，无法停车\n");
                    }
                }
                break;
            case 2:
                printf("请输入离场车的车牌号: ");
                scanf("%s", plate);
                if (remove_car(&park, &wait, plate)) {
                    printf("车辆 %s 已驶离停车场\n", plate);
                } else {
                    printf("停车场内没有找到车辆 %s\n", plate);
                }
                break;
            case 3:
                if (park.current == 0) {
                    printf("停车场目前没有车辆停放\n");
                } else {
                    printf("停车场停放车辆：");
                    for (i = 0; i < park.current; i++) {
                        printf("%s ", park.plates[i]);
                    }
                    printf("\n");
                }
                break;
            case 4:
                if (wait.count == 0) {
                    printf("候车场目前没有车辆\n");
                } else {
                    printf("候车场车辆：");
                    idx = wait.front;
                    for (i = 0; i < wait.count; i++) {
                        printf("%s ", wait.plates[idx]);
                        idx = (idx + 1) % N;
                    }
                    printf("\n");
                }
                break;
            case 0:
                printf("程序已退出\n");
                break;
            default:
                printf("无效指令，请重新输入\n");
        }
    } while (cmd != 0);

    return 0;
}
