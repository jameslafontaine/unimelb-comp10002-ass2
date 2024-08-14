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

   Signed by: JAMES LA FONTAINE    1079860
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>


/* #define's -----------------------------------------------------------------*/

#define DEBUG 0
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.6f\n", __LINE__, #x, x)
#define DUMP_INT(x) printf("line %d: %s = %d\n", __LINE__, #x, x)
#define DUMP_CHR(x) printf("line %d: %s = %c\n", __LINE__, #x, x)
#define DUMP_STR(x) printf("line %d: %s = %s\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#define DUMP_INT(x)
#define DUMP_STR(x)
#endif


#define ASIZE 26
#define LOWERCASE_OFFSET 97   /* used to line up state array with ASCII codes */
#define UPPERCASE_OFFSET 65   /* used to line up state array with ASCII codes */
#define TRUE 1
#define FALSE 0
#define HASH 35               /* ASCII code for # symbol */
#define COLON 58              /* ASCII code for : symbol */
#define ACTIONSTRLEN 20
#define TRACESTRLEN 10
#define VALIDTRACE 1
#define INVALIDTRACE 0

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

trace_t* stage0(void);         /* handles the flow of the program for stage 0 */
void stage1(void);             /* handles the flow of the program for stage 1 */
void stage2(void);             /* handles the flow of the program for stage 2 */

int check_precondition(step_t* currentstep, unsigned char currentstate[]);
void apply_effect(step_t* currentstep, unsigned char currentstate[]);
unsigned char* initialise_current_state(unsigned char initial[]);

/* where it all happens ------------------------------------------------------*/
int
main(int argc, char *argv[]) {
	trace_t *trace;
	
    printf("implement stages 0-2...\n");
    trace = stage0();
    
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

trace_t
*stage0() {
	trace_t *trace = make_empty_trace();          /* handle to the trace list */
	trace_t *definition = make_empty_trace();    /* handle to definition list */
	unsigned char initial[ASIZE] = {FALSE};            /* array storing initial conditions */
	action_t newaction = {'\0',FALSE,FALSE}; /* build action struct with this */
	step_t *currentstep;                  /* the current step being simulated */
	unsigned char *currentstate;
	char actionstr[ACTIONSTRLEN];           /* array that stores input action */
	char tracestr[TRACESTRLEN];
	unsigned char distinct[ASIZE] = {FALSE}; /* array that tracks action names */
	unsigned char ch;
	int numdistinct = 0;
	int numstepspassed = 0;
	int tracestatus;
	int i,j;
	
	
	
	/* read the initial conditions and record them appropriately in the initial
	state array until a hash character is input */
	while ((ch=getchar())!=HASH) {
		initial[ch-LOWERCASE_OFFSET] = TRUE;
	}
	
	/* read each action one at a time, record it as a string, and build the 
	action struct for each until a hash character is input */
	while ((ch=getchar())!=HASH) {
		scanf("%s", actionstr);
		
		/* set all precon values and effect values to FALSE before building */
		for (i=0; i<ASIZE; i++) {
			newaction.precon[i] = FALSE;
			newaction.effect[i] = FALSE;
		}
		
		/* adjust the precondition of newaction first */
		for (i=0; (ch=actionstr[i])!=COLON; i++) {
			newaction.precon[ch-LOWERCASE_OFFSET] = TRUE;
		}
			
		for (i+=1; (ch=actionstr[i])!=COLON; i++) {
			newaction.precon[ch-LOWERCASE_OFFSET] = FALSE;
		}
		
		/* this is the name of the action */
		i+=1;
		ch=actionstr[i];
		newaction.name = ch; 
		
		/* now adjust the effect */
		for (i+=1; (ch=actionstr[i])!=COLON; i++) {
			newaction.effect[ch-LOWERCASE_OFFSET] = TRUE;;
		}
		
		for (i+=1; (ch=actionstr[i])!='\0'; i++) {
			newaction.effect[ch-LOWERCASE_OFFSET] = FALSE;
		}
		
		/* now we can insert this new action into a new step in the definition 
		linked list */
		definition = insert_at_tail(definition, &newaction);
	}	
	
	/* now read the trace and create the trace linked list with all the steps 
	in the order specified by the input trace */
	while ((ch=getchar())!=HASH || ch!=EOF) {
		(scanf("%s", tracestr));
		printf("now entering for loop");
		
		for (i=0; i<strlen(tracestr); i++) {
			currentstep = definition->head;
			printf("current step assigned to definition->head successfully ");
			while (currentstep) {
				/* we have found the action named by the current letter in the 
				trace inside the definition list so insert this action into trace */
				if (tracestr[i] == currentstep->action->name) {
					trace = insert_at_tail(trace, currentstep->action);
					printf("new step inserted into trace list");
				}
			}
			/* check if this action name has appeared before in the trace */
			ch = tracestr[i];
			if (distinct[ch-UPPERCASE_OFFSET]==FALSE) {
			distinct[ch-UPPERCASE_OFFSET] = TRUE;
			numdistinct += 1;
			}
		}
	}
	/* now we will validate the trace */
	
	/* create an array which will store the current state as the trace is
	simulated, and track the current step that is being performed */
	currentstate = initialise_current_state(initial);
	currentstep = trace->head;
	
	/* simulate every step and thus action until we reach the end of the trace
	linked list and have verified the trace */
	while (currentstep) {
		tracestatus = check_precondition(currentstep, currentstate);
		
		if (!tracestatus) {
			break;
		}
		numstepspassed += 1;	
		
		apply_effect(currentstep, currentstate);
		currentstep = currentstep->next;
	}		
	free(currentstate);
	currentstate = NULL;
	
	/* now we will display the results */	
	printf("==STAGE 0===============================\n");
	printf("Number of distinct actions:%2d\n", numdistinct);
	printf("Length of the input trace:%2d\n", (char)strlen(tracestr));
	if (tracestatus) {
		printf("Trace status: valid\n");
	} else {
		printf("Trace status: invalid\n");
	}
	printf("----------------------------------------\n");
	printf(" abcdefghijklmnopqrstuvwxyz\n");
	printf("> ");
	for (i=0; i<ASIZE; i++) {
		printf("%d", initial[i]);
	}
	printf("\n");
	
	currentstate = initialise_current_state(initial);
	currentstep = trace->head;
	/* now print the effect of each step in the trace list 
	(up until the invalid trace was found if it was found) */
	for (i=0; i<numstepspassed; i++) {
		printf("%c ", currentstep->action->name);
		apply_effect(currentstep, currentstate);
		for (j=0; j<ASIZE; j++) {
			printf("%d", currentstate[j]);
		}
		printf("\n");
	}
	free(currentstate);
	currentstate = NULL;
	
	return trace;
	
}	
	

int
check_precondition(step_t* currentstep, unsigned char currentstate[]) {
	int i;
	
	for (i=0; i<ASIZE; i++) {
		if (currentstate[i]!=currentstep->action->precon[i]) {
			return INVALIDTRACE;
		}
	}
	
	return VALIDTRACE;
}		
		
void
apply_effect(step_t* currentstep, unsigned char currentstate[]) {
	int i;
	
	for (i=0; i<ASIZE; i++) { 
			currentstate[i] = currentstep->action->effect[i];
		}
	return;
}

unsigned char
*initialise_current_state(unsigned char initial[]) {
	unsigned char *currentstate;
	int i;
	
	currentstate = (unsigned char*)malloc(ASIZE*sizeof(*currentstate));
	assert(currentstate!=NULL);
	
	
	for (i=0; i<ASIZE; i++) {
		currentstate[i] = initial[i];
	}	

	return currentstate;
}
/* ta-da-da-daa!!! -----------------------------------------------------------*/
