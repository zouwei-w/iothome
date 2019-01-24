#include "bejson.h"

int parse_json_int(json_object *new_object, const char *filed,int *sdata)
{
    if(new_object == NULL || sdata == NULL || filed == NULL)
        return -1;
    struct json_object *object = json_object_object_get(new_object, filed);
    enum json_type object_type = json_object_get_type(object);
    
    printf("json_type_int: %d\n", json_object_get_int(object));

    if(json_type_int == object_type)
        *sdata = json_object_get_int(object);
    else
    {
        json_object_put(object);
        return -1;
    }
    
    json_object_put(object);
    
    return 0;
}

int parse_json_string(json_object *new_object, const char *filed,char *str)
{
    if(new_object == NULL || filed == NULL || str == NULL)
        return -1;
    struct json_object *object = json_object_object_get(new_object, filed);
    enum json_type object_type = json_object_get_type(object);
    
    if(object_type == json_type_string || object_type == json_type_object)
        strcpy(str, json_object_get_string(object));
    else
    {
        json_object_put(object);
        return -1;
    }
    
    json_object_put(object);

    return 0;
}

void print_json_object_member(json_object *new_object,const char *field)
{

	//根据指定对象的成员名，获取这个成员对象的json对象
	struct json_object *object = json_object_object_get(new_object, field);
	//获取json对象的类型
	enum json_type object_type = json_object_get_type(object);
	
	switch(object_type){
		case json_type_int:
			//printf("new_obj.%s json_object_get_type()=%s\n", field,json_type_to_name(object_type));
			printf("new_obj.%s json_object_get_int()=%d\n", field,json_object_get_int(object));
			break;

		case json_type_string:
			//printf("new_obj.%s json_object_get_type()=%s\n", field,json_type_to_name(object_type));
			printf("new_obj.%s json_object_get_string()=%s\n", field,json_object_get_string(object));
			break;
        default:break;
	}
	printf("\n-------------------------------------------------------\n");
	
	//释放json对象
	json_object_put(object);

	return;
}

void print_json_object(json_object *j_object)
{
	print_json_object_member(j_object,"temp");
	print_json_object_member(j_object,"humidity");
	print_json_object_member(j_object,"triaxialX");
	print_json_object_member(j_object,"triaxialY");
	print_json_object_member(j_object,"triaxialZ");
	print_json_object_member(j_object,"userName");
	print_json_object_member(j_object,"passWord");
	
	return;
}


