#include "function.h"

/** 功能id和第二层包对象 **/
void function_id_select(int id, json_object *object, m_filefd mfd)
{
    //根据功能id执行相对应的函数
    switch(id)
    {
    case 0x01: //登录
        printf("user login function\n");
        login_menu(mfd, object);
        break;

    case 0x02: //注册
        printf("user register function\n");
        register_menu(mfd, object);
        break;

    case 0x03: //环境
        printf("get enviroment function\n");
        enviroment_menu(mfd, object);
        break;

    case 0x04: //控制设备
        printf("contrl device function\n");
        control_menu(mfd, object);

    default:break;
    }

}

/**登录功能**/
void login_menu(m_filefd mfd, json_object *object)
{
    int ret;
    char state[50];
    ret = login_info(mfd, object);
    if(ret < 0)
        sprintf(state, "{\"stateCode\": 1}");
    else
        sprintf(state, "{\"stateCode\": 0}");

    send(mfd.connect_fd, state, strlen(state), 0);

    printf("state: %s\n", state);
}

/*注册功能*/
void register_menu(m_filefd mfd, json_object *object)
{
    int ret;
    char state[50];
    ret = register_info(mfd, object);
    if(ret < 0)
        sprintf(state, "{\"stateCode\": 1}"); //失败
    else if(ret == 1)
        sprintf(state, "{\"stateCode\": 2}"); //已经注册
    else if(ret == 2)
        sprintf(state, "{\"stateCode\": 4}"); //已经使用该电话号码
    else
        sprintf(state, "{\"stateCode\": 0}"); //成功
    
    send(mfd.connect_fd, state, strlen(state), 0);
    
    printf("state: %s\n", state);
}

/*环境检测*/
void enviroment_menu(m_filefd mfd, json_object *object)
{
    int a;
    float f;
    int ret;
    char state[50];
    unsigned char buf[10];
    ret = enviroment_info(mfd, object);
    if(ret < 0)
    {
        sprintf(state, "{\"stateCode\": 1}");
    }
    else
    {
        ret = uart_recv(mfd.uart_fd, (char *)buf, 5, NULL);
        if(ret < 0)
        {
            sprintf(state,"{\"stateCode\": 1}");
        }
        if(buf[0] == 0xcc)
        {
            switch(buf[1])
            {
                case 0x07:  //温度
                    f = buf[2];
                    sprintf(state,"{\"stateCode\": 0,\"data\":{\"temp\": %.2f}}", f);
                    break;

                case 0x08:  //湿度
                    f = buf[2];
                    sprintf(state,"{\"stateCode\": 0,\"data\":{\"humi\": %.2f}}", f);
                    break;

                case 0x09:  //光照度
                    a = (buf[2]<<0x08) + (buf[3]);
                    sprintf(state,"{\"stateCode\": 0,\"data\":{\"lux\": %d}}", a);
                    break;
            }
        }
        else 
            sprintf(state,"{\"stateCode\": 1}");
    }
     
    send(mfd.connect_fd, state, strlen(state), 0);
    printf("state: %s\n", state);

}

/*控制设备*/
void control_menu(m_filefd mfd, json_object *object)
{
    int ret;
    char state[50]; 
    unsigned char buf[10];
    ret = control_info(mfd, object);
    if(ret < 0)
    {
        sprintf(state, "{\"stateCode\": 1}");
    }
    else
    {
        ret = uart_recv(mfd.uart_fd, (char *)buf, 5, NULL);
        if(ret < 0)
        {
            sprintf(state,"{\"stateCode\": 1}");
        }
        printf("buf: %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
        if(buf[0] == 0xcc)
        {
            sprintf(state,"{\"stateCode\": 0,\"data\":{\"deviceState\": %d}}", buf[3]);
        }
        else 
            sprintf(state,"{\"stateCode\": 1}");
    }
     
    send(mfd.connect_fd, state, strlen(state), 0);
    printf("state: %s\n", state);
}

/*对登录信息进行判断*/
int login_info(m_filefd mfd, json_object *object)
{  
    int ret;
    char name[100],pass[100];

    ret = parse_json_string(object, "username", name);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_string\n");
        return -1;
    }
    
    ret = parse_json_string(object, "password", pass);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_string\n");
        return -1;
    }

    ret = user_login(name, pass);
    if(ret < 0)
    {
        fprintf(stderr, "Failed to user_login\n");
        return -1;
    }

    return 0;
}


/**** 用户注册 *****/
int register_info(m_filefd mfd, json_object *object)
{
    int ret;
    char name[100], pass[100], phone[100];
    ret = parse_json_string(object, "username", name);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_string\n");
        return -1;
    } 
    ret = parse_json_string(object, "password", pass);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_string\n");
        return -1;
    }
    ret = parse_json_string(object, "phone", phone);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_string\n");
        return -1;
    }

    ret = user_register(name, pass, phone);
    
    return ret;
}

int enviroment_info(m_filefd mfd, json_object *object)
{
    int ret;
    int device_num, device_id, dev_code;
    ret = parse_json_int(object,"deviceNumber", &device_num);
    ret = parse_json_int(object,"deviceID", &device_id);
    ret = parse_json_int(object,"deviceCode", &dev_code);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_int\n");
        return -1;
    }

    clear_protocol();
    protocol[0] = 0xDD; //start
    protocol[1] = (unsigned char)device_num; //deviceNumber
    protocol[2] = (unsigned char)device_id;  //deviceID
    protocol[3] = (unsigned char)dev_code; //get
    protocol[4] = 0xFF; //end

    printf("protocol: %x %x %x %x %x\n", protocol[0],protocol[1], protocol[2], protocol[3], protocol[4]);

    ret = uart_send(mfd.uart_fd, (char *)protocol, 5);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to uart_send\n");
        return -1;
    }

    printf("uart_send\nmfd.uart_fd=%d\n",mfd.uart_fd);
    
    return 0;
}

/****/
int control_info(m_filefd mfd, json_object *object)
{
    int ret;
    int device_num, device_id, state_no;
    ret = parse_json_int(object,"deviceNumber", &device_num);
    ret = parse_json_int(object,"deviceID", &device_id);
    ret = parse_json_int(object,"deviceCode", &state_no);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to parse_json_int\n");
        return -1;
    }

    //0xdd 0x04 0x01  0x01 0xFF
    clear_protocol();
    protocol[0] = 0xDD; //start
    protocol[1] = (unsigned char)device_num; //deviceNumber
    protocol[2] = (unsigned char)device_id;  //deviceID
    protocol[3] = (unsigned char)state_no; //open/close
    protocol[4] = 0xFF; //end

    printf("protocol: %x %x %x %x %x\n", protocol[0],protocol[1], protocol[2], protocol[3], protocol[4]);

    ret = uart_send(mfd.uart_fd, (char *)protocol, 5);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to uart_send\n");
        return -1;
    }

    printf("uart_send\nmfd.uart_fd=%d\n",mfd.uart_fd); 

    return 0;
}

void clear_protocol(void)
{
    int i;
    for(i = 0; i < sizeof(protocol)/sizeof(protocol[0]); i ++)
        protocol[i] = 0;
}



