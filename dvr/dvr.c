#include <stdio.h>

#define MAX_NODES 50
#define INFINITY 999

int main(){
    int cost[MAX_NODES][MAX_NODES];
    int distance[MAX_NODES][MAX_NODES];
    int hop_count[MAX_NODES][MAX_NODES];

    int n,i,j,k;

    printf("Enter the number of nodes : ");
    scanf("%d",&n);

    printf("\nEnter the cost matrix\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            scanf("%d",&cost[i][j]);
            
            if (i != j && cost[i][j] == 0)  cost[i][j] = INFINITY;
        }
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            distance[i][j] = cost[i][j];

            if (i != j && distance[i][j] != INFINITY)   hop_count[i][j] = j;
            else                                         hop_count[i][j] = -1;
        }
    }
    
    for(k = 0; k < n; k++){
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                if (distance[i][j] > distance[i][k] + distance[k][j]){
                    distance[i][j] = distance[i][k] + distance[k][j];
                    hop_count[i][j] = hop_count[i][k];
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        printf("\nRouting Table for Node %d:\n", i);
        printf("Destination\tNext Hop\tCost\n");
        for (j = 0; j < n; j++) {
            if (i != j) {
                printf("%d\t\t%d\t\t%d\n", j, hop_count[i][j], distance[i][j]);
            }
        }
    }
    

}