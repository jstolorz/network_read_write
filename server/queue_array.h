//
// Created by janusz on 04.02.2022.
//

#pragma once

#include <iostream>


typedef struct {
    int *values;
    int head,tail, size;
}queue;


void init_queue(queue *q, int max_size){
    q->size = max_size;
    q->values = (int*)malloc(sizeof(int) * q->size);
    q->head = -1;
    q->tail = -1;
}

bool queue_empty(queue *q){
   return q->tail == q->head;
}

bool queue_full(queue *q){
   return (q->head == (q->tail % q->size) + 1)
   || (q->head == 0 && q->tail == (q->size -1 ));
}

void queue_destroy(queue *q){
    free(q->values);
}

void add_queue(queue *q, int value){

    if(queue_full(q)){
          std::cout << "Queue id full \n";
    }else{
        q->tail = (q->tail + 1) % q->size;
        q->values[q->tail] = value;

        if(q->head == -1){
            q->head = q->tail;
        }
    }
}

int delete_from_queue(queue *q){
    int result = -1;
    if(queue_empty(q)){
        std::cout << "Queue is empty \n";
    }else{
        result = q->values[q->head];
        q->head = (q->head + 1) % q->size;
    }
    return result;
}

void clear(queue *q){
    q->head = -1;
    q->tail = -1;
}

void print_queue(queue *q){
    for (int i = 0; i < q->size; ++i) {
        std::cout << q->values[i] << " ";
    }
}



