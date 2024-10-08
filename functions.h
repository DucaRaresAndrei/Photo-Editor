//Duca Andrei-Rares
//311CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct colors {
	int **gr, **r, **g, **b;
	int h, l, ok, col, x1, x2, y1, y2;
	char mgw[3];
};

//function for allocating an array
//it is checked if the allocation is done correctly
int **alloc_m(int m, int n);

//function for freeing the memory allocated to an array
void free_m(int **mat, int h);

//function that skips the lines containing comments
void skip_comm(FILE *in);

//the following 4 functions perform each type of reading from the file
void p2(FILE *in, int **gr, int h, int l);
void p5(FILE *in, int **gr, int h, int l);
void p3(FILE *in, int **r, int **g, int **b, int h, int l);
void p6(FILE *in, int **r, int **g, int **b, int h, int l);

//function that frees the memory of dynamically allocated arrays
void free_all(struct colors *st);

//function that performs the operation of loading the image into memory
void load(struct colors *st, char *zone);

//function to select a coordinate area
void select_z(struct colors *st, char *zone);

//function to select the coordinates of the entire picture
void select_all(struct colors *st);

//function that calculates the histogram of the picture
int histogram(struct colors *st, char *zone, int nrr);

//function that performs the equalization of the picture
int equalize(struct colors *st, int nrr);

//function that checks whether the whole picture
//or only an area is selected
int check_selection(struct colors *st);

//rotation +-180 degrees
int r_180(struct colors *st);

//rotation +90/-270 degrees of the whole picture
int r_90_all(struct colors *st);

//rotation +90/-270 degrees of the selected area
int r_90_sel(struct colors *st);

//rotation +270/-90 degrees of the whole picture
int r_270_all(struct colors *st);

//rotation +270/-90 degrees of the selected area
int r_270_sel(struct colors *st);

//function that performs the rotation operation
int rotate(struct colors *st, char *zone, int nrr);

//function that performs crop operation
int crop(struct colors *st, int nrr);

//function that performs the application of the parameter on the matrix
//that stores the values for the red color
int a_red(struct colors *st, int i, int j, int p);

//function that performs the application of the parameter on the matrix
//that stores the values for the green color
int a_green(struct colors *st, int i, int j, int p);

//function that performs the application of the parameter on the matrix
//that stores the values for the blue color
int a_blue(struct colors *st, int i, int j, int p);

//function that sets the margins that can be modified in the apply command
void help_apply(int *a1, int *a2, int *b1, int *b2, struct colors *st);

//function that performs the apply command
int apply(struct colors *st, char *zone, int nrr);

//function that saves the edited picture
int save(struct colors *st, char *zone, int nrr);
