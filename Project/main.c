/**
 * ./bin/iot 192.168.0.156 7878
 *
 * */
#include "./include/comm.h"

/***监听线程处理函数***/
void *listen_function(void *p)
{
    int id;
    int ret;
    char re_data[1024];
    char buf[1024];
    json_object *obj, *two_layers_obj;
    m_filefd mfd = *(m_filefd *)p;
    
    while(1)
    {
        ret = recv(mfd.connect_fd, buf, sizeof(buf)-1, 0);
        if(ret <= 0)
        {
            perror("Failed to recv");
            close(mfd.connect_fd);
            pthread_exit(NULL);
        }
        printf("client is connected\n");
        //将接收到的TCP数据转换位json对象
        obj = json_tokener_parse((const char *)buf);
        
        //解析功能ID
        ret = parse_json_int(obj, "id", &id);
        if(ret < 0)
        {
            fprintf(stderr,"Failed to parse_json_int\n");
            pthread_exit(NULL);
        }

        //解析数据data
        ret = parse_json_string(obj, "data", re_data);
        if(ret < 0)
        {
            fprintf(stderr, "Failed to parse_json_string\n");
            pthread_exit(NULL);
        }

        //第二层json对象
        two_layers_obj = json_tokener_parse((const char *)re_data);

        //进入到功能层处理
        function_id_select(id, two_layers_obj, mfd);

        json_object_put(two_layers_obj);
        json_object_put(obj);
    }

    close(mfd.connect_fd);
    pthread_exit(NULL);
}

/******* 主函数 ********/
int main(int argc, const char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage <Filename><IP><PORT>\n");
        return -1;
    }

    int ret;
    m_filefd mfd;
    int listen_fd;
    int *pconnect_fd;
    pthread_t thread;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
#if 1 
    ret = uart_dev_option(&mfd.uart_fd, 115200);
    if(ret < 0)
    {
        fprintf(stderr,"Failed to uart_dev_option\n");
        return -1;
    }
#endif

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        perror("Failed to socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret < 0)
    {
        perror("Failed to bind");
        return -1;
    }

    ret = listen(listen_fd, 128);
    if(ret < 0)
    {
        perror("Failed to listen");
        return -1;
    }
    printf("Listen.....\n");

    while(1)
    {
        pconnect_fd = (int *)malloc(sizeof(int));
        *pconnect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
        if(*pconnect_fd < 0)
        {
            perror("Failed to connect_fd");
            return -1;
        }

        mfd.connect_fd = *pconnect_fd;
        free(pconnect_fd);
        ret = pthread_create(&thread, NULL, listen_function, &mfd);
        if(ret < 0)
        {
            perror("Failed to pthread_create");
            return -1;
        }

        pthread_detach(thread);
    }

    return 0;
}

