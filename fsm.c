#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fsm.h"

#define NUM_STATES 4


bool cause_display() {
    printf("true cause_display\n");
    sleep(1);
    return true;
}

void effect_display() {
    printf("effect_display\n");
}

bool cause_message() {
    printf("false cause_message\n");
    sleep(1);
    return false;
}

void effect_message() {
    printf("effect_message\n");
}


/*
 * c is cause
 * e is effect
 * ns is next state
 *
 * The idea is this:
 *
 * The list is scanned, the function 'cause' is run and returns a boolean.
 * If this is true then the 'arc' is to be traversed, and 'effect' (which returns nothing)
 * is then run.
 *
 * ns is the new state.
 */
struct arc *addArc(struct arc *head, bool (*c)(), void (e)(),uint8_t ns) {
    struct arc *ptr;
    struct arc *tmp;

    ptr = malloc(sizeof(struct arc));
    if ( !ptr ) {
        return ptr;
    }

    (void)memset( ptr,0,sizeof(struct arc));

    ptr->cause = (bool (*)(void *))c;
    ptr->effect = (void (*)(void *))e;
    ptr->state = ns;

    tmp=head;

    if(!tmp) {
        head=ptr;
    } else {
        ptr->next=head;
        head=ptr;
    }

    return ptr;
}

uint8_t runArc(struct arc *head) {
    uint8_t nextState=-1;
    bool res=false;
    bool runFlag=true;

    struct arc *ptr;

    ptr=head;

    do {
        if(ptr != (struct arc *)NULL) {
            res=(head->cause)();
            if( res ) {
                (head->effect)();
                nextState = head->state;
                runFlag=false;
            } else {
                ptr = head->next;
            }
        } else {
            runFlag = false;
        }
    } while(runFlag);

    return nextState;
}


int main() {

    uint8_t i=0;
    uint8_t machineState=0;

    struct arc *machine[NUM_STATES];
    struct arc *ptr;

    for(i=0; i < NUM_STATES;i++ ) {

        machine[i] =0x00;
    }

    ptr = addArc(machine[0], cause_display,effect_display,1);
    machine[0] = ptr;

    ptr = addArc(machine[1], cause_message,effect_message,0);
    machine[1] = ptr;

    while(true) {
        bool res;
        struct arc *ptr;
        uint8_t localState;

        printf("Machine State=%d\n",machineState);
        localState=runArc(machine[machineState]);

        if(localState != 0xff) {
            machineState=localState;
        }
    }
}

