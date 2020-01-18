#ifndef  __HTTP_H
#define	 __HTTP_H


//宏定义 MALLOC、FREE、DATE_LEN
#define MALLOC(x) malloc(x)
#define FREE(x) free(x)
#define TOTAL_DATA_LEN 64
#define MAX_DATA_LEN  128
#define DATE_LEN  128

#define SERVER_IP "47.98.119.123"
#define SERVER_PORT 1883

//打包HTTP头
int Head_Package(const char *server_ip, int port_num, int ContentLength, char *out_data);

//打包HTTP数据包
int Data_Package(const char *date, const char *text1, const char *place, char *out_data);

//HTTP数据上传
int Http_Data_Upload(char *server_ip, int port_num, char *time, char *text1, char *place, char *out_buffer);

//获得要上传的HTTP数据
int Get_Hex_Http_Packet_Data(char *upload_http_data,char *http_hex_data);

#endif
