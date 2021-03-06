typedef struct
{
    char	name [21] ;
    long	time ;
    double	mass ;
    double	xdot ;
    double	ydot ;
}
Puck ;

/* This IO protocol automatically generated by "structIO", version 0.12 */
/* by Brian Beckman, JPL, Caltech, 11 Oct 1988                            */
/* IO protocol implementation stored in the file PuckIO.c */

void printPuck (/* Puck */) ;
Puck scanPuck () ;

void writePuck (/* Puck */) ;
Puck readPuck () ;

void vprintPuck (/* Puck */) ;
Puck vscanPuck () ;


void fprintPuck (/* FILE*, Puck */) ;
Puck fscanPuck (/* FILE* */) ;

void fwritePuck (/* FILE*, Puck */) ;
Puck freadPuck (/* FILE* */) ;

void fvprintPuck (/* FILE*, Puck */) ;
Puck fvscanPuck (/* FILE* */) ;
