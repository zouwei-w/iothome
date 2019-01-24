#ifndef _BEJSON_H
#define _BEJSON_H

#include "comm.h"


extern int parse_json_int(json_object *new_object, const char *filed,int *sdata);

extern int parse_json_string(json_object *new_object, const char *filed,char *str);

extern void print_json_object_member(json_object *new_object,const char *field);

extern void print_json_object(json_object *j_object);


#endif
