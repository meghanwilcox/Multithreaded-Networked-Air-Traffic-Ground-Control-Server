#ifndef _TAKEOFF_LIST_H
#define _TAKEOFF_LIST_H

#include "airplane.h"

void flightid_free(void *p);
void takeoff_list_init(void);
void takeoff_list_add(char *flightid);
void takeoff_list_remove(char *flightid);
char* takeoff_list_get(int index);
void takeoff_list_print(void);
int get_takeoff_position(char *flightid);
char* takeoff_get_ahead(char *flightid);
void plane_takeoff(airplane *plane);
#endif 