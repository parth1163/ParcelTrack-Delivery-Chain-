//PA3
//Parth Patel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

typedef struct hub {
    char name[MAX_NAME_LEN]; //city or location name 
    int distance; //distance from previous hub
    int hour; //simplified timestamp (0-23)
    struct hub* next;
} Hub;

int is_empty(struct hub* head) {
    return head == NULL;
}
//add a hub to the front of the list
void insert_at_head(Hub** head, const char* name, int distance, int hour) {
    Hub* new_hub = (Hub*)malloc(sizeof(Hub));
    strncpy(new_hub->name, name, MAX_NAME_LEN);
    new_hub->distance = distance;
    new_hub->hour = hour;
    new_hub->next = *head;
    *head = new_hub;
}
//sdds a new hub to the end of the list
void insert_at_tail(Hub** head, const char* name, int distance, int hour) {
    if (*head == NULL) {
        insert_at_head(head, name, distance, hour);
        return;
    }
    insert_at_tail(&((*head)->next), name, distance, hour);
}
//prints delivery chain from start to finish
void print_chain(Hub* head) {
    while (head) {
        printf("%s - %d km, %dh\n", head->name, head->distance, head->hour);
        head = head->next;
    }
}

int total_distance(Hub* head) {
    int total = 0;
    while (head) {
        total += head->distance;
        head = head->next;
    }
    return total;
}
//Prints chain in reverse order
void print_chain_reverse(Hub* head) {
    if (head == NULL) return;
    print_chain_reverse(head->next);
    printf("%s - %d km, %dh\n", head->name, head->distance, head->hour);
}

Hub search_hub(Hub* head, const char* keyword) {
    Hub result;
    strcpy(result.name, "None");
    result.distance = -1;
    result.hour = -1;
    result.next = NULL;

    while (head) {
        if (strstr(head->name, keyword)) {
            strncpy(result.name, head->name, MAX_NAME_LEN);
            result.distance = head->distance;
            result.hour = head->hour;
            break;
        }
        head = head->next;
    }
    return result;
}

void remove_first_match(Hub** head, const char* keyword) {
    Hub* current = *head;
    Hub* prev = NULL;

    while (current) {
        if (strstr(current->name, keyword)) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
//Displays the largest time gap between two hubs
void display_bottleneck(Hub* head) {
    if (!head || !head->next) return;
    Hub* maxStart = head;
    int maxDiff = -1;

    while (head && head->next) {
        int diff = head->next->hour - head->hour;
        if (diff > maxDiff) {
            maxDiff = diff;
            maxStart = head;
        }
        head = head->next;
    }
    printf("Bottleneck from %s to %s: %d hours\n", maxStart->name, maxStart->next->name, maxDiff);
}

int main() {
    printf("=== ParcelTrack Delivery Chain Test ===\n\n");

    Hub* chain = NULL;

    // Test insert_at_head and insert_at_tail
    insert_at_head(&chain, "Orlando", 0, 9);
    insert_at_tail(&chain, "Atlanta", 400, 14);
    insert_at_tail(&chain, "Charlotte", 350, 17);

    printf("Delivery chain:\n");
    print_chain(chain);
    printf("\n");

    // Test total_distance
    int total = total_distance(chain);
    printf("Total distance: %d km\n\n", total);

    // Test display_bottleneck
    display_bottleneck(chain);

    // Test search_hub
    Hub found = search_hub(chain, "lanta");
    if (strcmp(found.name, "None") != 0) {
        printf("Search result: Found ");
        printf("\"{%s, %d, %d}\"\n\n", found.name, found.distance, found.hour);
    } else {
        printf("Search result: No match found\n\n");
    }

    // Test print_chain_reverse
    printf("Reverse delivery chain:\n");
    print_chain_reverse(chain);
    printf("\n");

    // Test remove_first_match
    remove_first_match(&chain, "lanta");
    printf("After removing first match for \"lanta\":\n");
    print_chain(chain);
    printf("\n");

    return 0;
}
