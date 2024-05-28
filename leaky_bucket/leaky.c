#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUCKET_SIZE 10
#define OUT_RATE 3

int bucket_content = 0;

void leaky_bucket(int packet_size) {
    if (packet_size == -1) {
        printf("Idle time. Allowing bucket to leak.\n");
        if (bucket_content > 0) {
            if (bucket_content < OUT_RATE) {
                printf("%d bytes sent from bucket\n", bucket_content);
                bucket_content = 0;
            } else {
                printf("%d bytes sent from bucket\n", OUT_RATE);
                bucket_content -= OUT_RATE;
            }
        } else {
            printf("Bucket is empty.\n");
        }
        printf("Bucket content after leaking: %d\n", bucket_content);
        return;
    }

    printf("Incoming packet size: %d\n", packet_size);
    if (packet_size > BUCKET_SIZE) {
        printf("Packet size (%d) is too large and cannot be accommodated. Dropped.\n", packet_size);
        return;
    }

    if (bucket_content + packet_size > BUCKET_SIZE) {
        printf("Bucket overflow! Packet dropped.\n");
    } else {
        bucket_content += packet_size;
        printf("Packet added to the bucket. Current bucket content: %d\n", bucket_content);
    }

    if (bucket_content > 0) {
        if (bucket_content < OUT_RATE) {
            printf("%d bytes sent from bucket\n", bucket_content);
            bucket_content = 0;
        } else {
            printf("%d bytes sent from bucket\n", OUT_RATE);
            bucket_content -= OUT_RATE;
        }
    } else {
        printf("Bucket is empty.\n");
    }

    printf("Bucket content after leaking: %d\n", bucket_content);
}

int main() {
    int packet_sizes[] = {5, 8, -1, 3, 10, 14, -1, 9, 2, -1, 15, 1, 7, 6, -1};
    int num_packets = sizeof(packet_sizes) / sizeof(int);

    for (int i = 0; i < num_packets; i++) {
        leaky_bucket(packet_sizes[i]);
        sleep(1);
    }
    
    while(bucket_content != 0){
      leaky_bucket(-1);
      sleep(1);  
    }

    return 0;
}