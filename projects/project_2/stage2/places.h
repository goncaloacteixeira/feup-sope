//
// Created by skdgt on 13/05/20.
//

#ifndef PLACES_PLACES_H
#define PLACES_PLACES_H

#include <stdlib.h>

typedef struct {
    int front;
    int back;
    int size;
    int max_size;
    int* queue;
} places_t;

places_t places_new(int max_size) {
    places_t places;
    places.size = 0;
    places.max_size = max_size;
    places.front = 0;
    places.back = max_size - 1;
    places.queue = (int*) malloc(max_size * sizeof(int));
    return places;
}

int full(places_t* places) {
    return places->size == places->max_size;
}

int empty(places_t* places) {
    return places->size == 0;
}

int push(places_t* places, int value) {
    if (!full(places)) {
        places->back = (places->back + 1) % places->max_size;
        places->queue[places->back] = value;
        places->size++;
        return 0;
    }
    return 1;
}

int pop_front(places_t* places) {
    int value = -1;
    if (!empty(places)) {
        value = places->queue[places->front];
        places->front = (places->front + 1) % places->max_size;
        places->size--;
    }
    return value;
}

int fill(places_t* places) {
    for (int i = 1; i <= places->max_size; i++) {
        if (push(places, i) != 0)
            return 1;
    }
    return 0;
}


#endif //PLACES_PLACES_H
