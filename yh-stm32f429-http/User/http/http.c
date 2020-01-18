#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"


//请求头部
char upload_head[] =
"POST /? HTTP/1.1\r\n"
"Host: %s:%d\r\n"
"content-type: multipart/form-data; boundary=--------------------------359443142220480708322079\r\n"
"content-length: %d\r\n\r\n";

//请求数据
char upload_data[] =
{
	"----------------------------359443142220480708322079\r\n"
	"Content-Disposition: form-data; name=\"date\"\r\n\r\n"
	"%s"
	"\r\n----------------------------359443142220480708322079\r\n"
	"Content-Disposition: form-data; name=\"text1\"\r\n\r\n"
	"%s"
	"\r\n----------------------------359443142220480708322079\r\n"
	"Content-Disposition: form-data; name=\"place\"\r\n\r\n"
	"%s"
	"\r\n----------------------------359443142220480708322079--\r\n"
};

//打包HTTP头
int Head_Package(const char *server_ip, int port_num, int ContentLength, char *out_data)
{
	int len = 0 ;
	char *up_head = NULL ;
	if (NULL == server_ip || NULL == out_data)
		return -1 ;
	if (ContentLength < 0 || port_num < 0)
		return -2 ;
	up_head = MALLOC(DATE_LEN);
	if (NULL != up_head)
		snprintf(up_head, DATE_LEN, upload_head, server_ip, port_num, ContentLength);
	if (NULL != out_data)
		sprintf(out_data, "%s", up_head);
	else
		return -3 ;
	len = strlen(up_head);
	FREE(up_head);
	return len;
}


//打包HTTP数据包
int Data_Package(const char *date, const char *text1, const char *place, char *out_data)
{
	int len = 0 ;
	char *up_data = NULL ;
	if (NULL == date || NULL == text1 || NULL == place || NULL == out_data)
		return -1 ;
	up_data = MALLOC(DATE_LEN);
	if (NULL != up_data)
		snprintf(up_data, DATE_LEN, upload_data, date, text1, place);
	if (NULL != out_data)
		sprintf(out_data, "%s", up_data);
	else
		return -2 ;
	len = strlen(up_data);
	FREE(up_data);
	return len;
}

//HTTP数据上传
int Http_Data_Upload(char *server_ip, int port_num, char *time, char *text1, char *place, char *out_buffer)
{
	int ContentLength = 0 ;
	char *http_head = NULL ;
	char *http_data = NULL ;
	char *upload_http_data = NULL ;
	http_head = MALLOC(DATE_LEN);
	if (NULL == http_head)
		return -1 ;
	http_data = MALLOC(DATE_LEN);
	if (NULL == http_data)
		return -2 ;
	upload_http_data = MALLOC(TOTAL_DATA_LEN);
	if (NULL == upload_http_data)
		return -3 ;
	ContentLength = Data_Package(time, text1, place, http_data);
	Head_Package(server_ip, port_num, ContentLength, http_head);
	sprintf(upload_http_data, "%s%s", http_head, http_data);
	sprintf(out_buffer, "%s", upload_http_data);
	FREE(http_head);
	FREE(http_data);
	FREE(upload_http_data);
	return 0 ;
}

//获得要上传的HTTP数据
int Get_Hex_Http_Packet_Data(char *upload_http_data,char *http_hex_data){
	/*
	printf("Get_Hex_Http_Packet_Data upload_http_data %s\r\n",upload_http_data);
	printf("Get_Hex_Http_Packet_Data http_hex_data %s\r\n",http_hex_data);
	//*/
	return 0;
}
