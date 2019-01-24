#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "comm.h"

unsigned char protocol[10];

extern void clear_protocol(void);

extern void function_id_select(int id, json_object *object, m_filefd mfd);

extern void login_menu(m_filefd mfd, json_object *object);

extern void register_menu(m_filefd mfd, json_object *object);

extern void enviroment_menu(m_filefd mfd, json_object *object);

extern void control_menu(m_filefd mfd, json_object *object);

extern int login_info(m_filefd mfd, json_object *object);

extern int register_info(m_filefd mfd, json_object *object);

extern int enviroment_info(m_filefd mfd, json_object *object);

extern int control_info(m_filefd mfd, json_object *object);


#endif
