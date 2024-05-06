#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    char *name;
    struct Node *children;
    struct Node *next;
} Node;

Node *createNode(char *name) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->name = strdup(name);
    newNode->children = NULL;
    newNode->next = NULL;
    return newNode;
}

void addChild(Node *parent, Node *child) {
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        Node *curr = parent->children;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = child;
    }
}

Node *buildTree() {
    Node *root = createNode("root");
    Node *com = createNode("com");
    Node *in = createNode("in");
    Node *google = createNode("google");
    Node *facebook = createNode("facebook");
    Node *example = createNode("example");
    Node *google_ip = createNode("142.250.196.78");

    addChild(root, com);
    addChild(root, in);
    addChild(com, google);
    addChild(com, facebook);
    addChild(com, example);
    addChild(google,google_ip);

    return root;
}

void tokenize(char *input_str, char **token1, char **token2) {
    char *delimiter = ".";
    char *token;
    int i = 0;

    token = strtok(input_str, delimiter);
    if (token != NULL) {
        *token1 = malloc(strlen(token) + 1);
        strcpy(*token1, token);
    } else {
        *token1 = NULL;
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        *token2 = malloc(strlen(token) + 1);
        strcpy(*token2, token);
    } else {
        *token2 = NULL;
    }
}

bool isDomainPresent(Node *root, char *domain,char **ip) {
    if (root == NULL || domain == NULL) {
        return false;
    }

    Node *curr = root->children;

    char *token1,*token2;

    tokenize(domain, &token1, &token2);
    printf("\ntoken1 -> %s\ntoken2 -> %s\n",token1,token2);

    int f = 0;
    while (curr != NULL) {
        printf("curr -> %s\n",curr->name);
        if (strcmp(curr->name, token2) == 0){
            printf("\nTrue\n");
            f = 1;
            curr = curr->children;
            break;
        }
        curr = curr->next;
    }

    while (curr!= NULL && f == 1){
        printf("curr -> %s\n",curr->name);
        if (strcmp(curr->name, token1) == 0){
            printf("\nTrue\n");
            curr = curr->children;
            strcpy(*ip,curr->name);
            return true;
            break;
        }
        curr = curr-> next;
    }
    return false;

}

int main(){
    Node *root = buildTree();

    printf("\nDNS Server\n");

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char write_buffer[1024], read_buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0){
        perror("BIND error");
        return -1;
    }

    listen(server_socket, 5);

    while(1){
        socklen_t len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr,&len);
        
        printf("\nDNS Client has connected\n");

        while(1){
            recv(client_socket,read_buffer,sizeof(read_buffer),0);

            if (strcmp("END\n", read_buffer) == 0 || strcmp("END", read_buffer) == 0){
                printf("\nClient has left\n");
                close(client_socket);
                break;
            }
            
            int len = strcspn(read_buffer, "\n");  
            read_buffer[len] = '\0';
            printf("\nhostname -> %s\n",read_buffer);

            char *ip;
            isDomainPresent(root, read_buffer,&ip);

            if (ip != NULL) {
                printf("Domain '%s' is present in the tree.\n", read_buffer);
                printf("IP Address: %s\n", ip);
                send(client_socket, ip, strlen(ip), 0);
            } 
            else {
                printf("Domain '%s' is not present in the tree.\n", read_buffer);
                char* message = "Domain not found\n";
                send(client_socket, message, strlen(message), 0);
            }    
            
            printf("\nDo you want to continue\n");
            printf("Type END to exit\n");
            fgets(write_buffer, sizeof(write_buffer), stdin);

            if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
                printf("\nServer is exiting....\n");
                close(client_socket);
                close(server_socket);
                return 0;
            }  
        }
        if (strcmp("END\n", write_buffer) == 0 || strcmp("END", write_buffer) == 0){
                printf("\nServer is exiting....\n");
                close(server_socket);
                return 0;
        }  
    }
}