
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "alist.h"
#include "takeoff_list.h"
#include "airs_protocol.h"

// The array list of all planes

static alist planes_to_takeoff;

// A global lock, to ensure that the list doesn't chnage when being accessed

static pthread_rwlock_t listlock;

void flightid_free(void *p) {
    // char *id = (char *)p;
    // //airplane_destroy(ap);
    // free(id);
}

void takeoff_list_init(void) {
    alist_init(&planes_to_takeoff, flightid_free);
    pthread_rwlock_init(&listlock, NULL);
}

void takeoff_list_add(char *flightid) {
    pthread_rwlock_wrlock(&listlock);
    alist_add(&planes_to_takeoff, flightid);
    pthread_rwlock_unlock(&listlock);
}

void takeoff_list_remove(char *flightid) {
    pthread_rwlock_wrlock(&listlock);
    for (int i=0; i<alist_size(&planes_to_takeoff); i++) {
        if (alist_get(&planes_to_takeoff, i) == flightid) {
            alist_remove(&planes_to_takeoff, i);
            //printf("remove here\n");
            pthread_rwlock_unlock(&listlock);
            return;
        }
    }

    printf("Couldn't find plane to remove - this shouldn't happen\n");
    pthread_rwlock_unlock(&listlock);
}

char* takeoff_list_get(int index) {
    pthread_rwlock_rdlock(&listlock);
    char* flightid = NULL;

    if (index >= 0 && index < alist_size(&planes_to_takeoff)) {
        flightid = (char*)alist_get(&planes_to_takeoff, index);
    }

    pthread_rwlock_unlock(&listlock);
    return flightid;
}

void takeoff_list_print(void) {
    pthread_rwlock_rdlock(&listlock);

    printf("Takeoff List Contents:\n");

    for (int i = 0; i < alist_size(&planes_to_takeoff); i++) {
        char* flightid = (char*)alist_get(&planes_to_takeoff, i);
        printf("%d. %s\n", i + 1, flightid);
    }

    pthread_rwlock_unlock(&listlock);
}

int get_takeoff_position(char *flightid){
    pthread_rwlock_rdlock(&listlock);
    int position = alist_index_of(&planes_to_takeoff, flightid) + 1;
    pthread_rwlock_unlock(&listlock);

    return position;
}

char* takeoff_get_ahead(char *flightid){
    int position = get_takeoff_position(flightid);

    char* result = (char*)malloc(((position - 1) * (PLANE_MAXID + 2)) * sizeof(char));
    result[0] = '\0';
    int i;

    for(i = position - 2; i >= 0; i--){
        char currentString[PLANE_MAXID + 2];
        if(i == 0){
            sprintf(currentString, "%s", (char*)alist_get(&planes_to_takeoff, i));
        }
        else{
            sprintf(currentString, "%s, ", (char*)alist_get(&planes_to_takeoff, i));
        }
        strcat(result, currentString);
    }

    return result;

}

void plane_takeoff(airplane *plane){
    send_takeoff(plane);
}