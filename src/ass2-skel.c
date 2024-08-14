/* Program to evaluate candidate routines for Robotic Process Automation.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2020, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton
  file, the  program contained in this submission is completely my own
  individual work, except where explicitly noted by further comments that
  provide details otherwise.  I understand that work that has been developed
  by another student, or by me in collaboration with other students, or by
  non-students as a result of request, solicitation, or payment, may not be
  submitted for assessment in this subject.  I understand that submitting for
  assessment work developed by or in collaboration with other students or
  non-students constitutes Academic Misconduct, and may be penalized by mark
  deductions, or by other penalties determined via the University of
  Melbourne Academic Honesty Policy, as described at
  https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my
  work to other students, regardless of my intention or any undertakings made
  to me by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment
  specification to any form of code authoring or assignment tutoring service,
  or drawing the attention of others to such services and code that may have
  been made available via such a service, may be regarded as Student General
  Misconduct (interfering with the teaching activities of the University
  and/or inciting others to commit Academic Misconduct).  I understand that
  an allegation of Student General Misconduct may arise regardless of whether
  or not I personally make use of such solutions or sought benefit from such
  actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

/* #define's -----------------------------------------------------------------*/

#define ASIZE 26

/* type definitions ----------------------------------------------------------*/

// state (values of the 26 Boolean variables)
typedef unsigned char state_t[ASIZE];

// action
typedef struct action action_t;
struct action {
    char name;        // action name
    state_t precon;   // precondition
    state_t effect;   // effect
};

// step in a trace
typedef struct step step_t;
struct step {
    action_t *action; // pointer to an action performed at this step
    step_t   *next;   // pointer to the next step in this trace
};

// trace (implemented as a linked list)
typedef struct {
    step_t *head;     // pointer to the step in the head of the trace
    step_t *tail;     // pointer to the step in the tail of the trace
} trace_t;

/* function prototypes -------------------------------------------------------*/
trace_t* make_empty_trace(void);
trace_t* insert_at_tail(trace_t*, action_t*);
void free_trace(trace_t*);

/* my function prototypes ----------------------------------------------------*/

// add your function prototypes here ...

/* where it all happens ------------------------------------------------------*/
int
main(int argc, char *argv[]) {
    printf("implement stages 0-2...\n");

    return EXIT_SUCCESS;        // we are done !!! algorithms are fun!!!
}

/* function definitions ------------------------------------------------------*/

// Adapted version of the make_empty_list function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
trace_t
*make_empty_trace(void) {
    trace_t *R;
    R = (trace_t*)malloc(sizeof(*R));
    assert(R!=NULL);
    R->head = R->tail = NULL;
    return R;
}

// Adapted version of the insert_at_foot function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
trace_t 
*insert_at_tail(trace_t* R, action_t* addr) {
    step_t *new;
    new = (step_t*)malloc(sizeof(*new));
    assert(R!=NULL && new!=NULL);
    new->action = addr;
    new->next = NULL;
    if (R->tail==NULL) { /* this is the first insertion into the trace */
        R->head = R->tail = new; 
    } else {
        R->tail->next = new;
        R->tail = new;
    }
    return R;
}

// Adapted version of the free_list function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
void
free_trace(trace_t* R) {
    step_t *curr, *prev;
    assert(R!=NULL);
    curr = R->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(R);
}

/* my function definitions ---------------------------------------------------*/

// add your function definitions here ...

/* ta-da-da-daa!!! -----------------------------------------------------------*/
