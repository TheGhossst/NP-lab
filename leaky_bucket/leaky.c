#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#define BUCKET_SIZE 10   
#define OUT_RATE 3      

int bucket_content = 0;

void leaky_bucket(int packet_size) {
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
    int packet_size;
    for (int i = 0; i < 10; i++) {
        packet_size = rand() % 15;  // Generate a random packet size between 0 and 14
        leaky_bucket(packet_size);
        sleep(1); 
    }
    return 0;
}