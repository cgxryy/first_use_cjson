/*
 * =====================================================================================
 *
 *       Filename:  my_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年01月14日 16时04分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  changgongxiaorong, cgxryy@gmail.com
 *        Company:  Class 1203 of Network Engineering
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "cJSON.h"

char citycode[10];

int deal_info(char *info)
{
	FILE *file;
	char buf[50];
	char *name = malloc(strlen(info)+1);
	char end;
	char *ptr;
	int i, j;

	file = fopen("CityCode.txt", "rb");
	
	while (!feof(file))
	{	
		i = 0;
		while ((end = fgetc(file)) != '\n')
		{
			buf[i] = end;
			i++;
		}
		buf[i] = '\0';

		ptr = buf;
		while (*(ptr-1) != ' ')
		      ptr++;
		for( j = 0, i = ptr-buf; (buf[i] != '\0') && (j < strlen(info)+1); i++, j++)	
		      name[j] = buf[i];
		name[j] = '\0';
		if (strncmp(name, info, strlen(info)) == 0)
		      break;
	}

	if (feof(file))
	{
		printf("there is not such a cityname in China.\n");
		return 0;
	}

	strncpy(citycode, buf, 9);
	citycode[9] = '\0';

//	printf("citycode %s\n", citycode);

	fclose(file);
	free(name);
	return 1;
}

void send_info()
{
	char cmd[100] = "wget http://www.weather.com.cn/data/sk/";
	
	sprintf(cmd, "wget http://www.weather.com.cn/data/sk/%s.html", citycode);
	
	system(cmd);
}

void check_info()
{
	FILE *check;
	char pre[16];
	char cmd[20];

	sprintf(pre, "%s.html", citycode);

	if ((check = fopen(pre, "rb")) != NULL)
	{
		sprintf(cmd, "rm %s.html", citycode);
		system(cmd);
		fclose(check);
	}
	      

}

void doit(char *text)
{
	char *out;
	cJSON *json;

	json = cJSON_Parse(text);
	if (!json)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	else 
	{
		out = cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n", out);
		free(out);
	}
}

void dofile(char *filename)
{
	FILE *file;
	long len;
	char *data;

	file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = malloc(len+1);
	fread(data, 1, len, file);
	fclose(file);
	doit(data);
	free(data);
}

void deal_json()
{
	char filename[20];

	sprintf(filename, "%s.html", citycode);

	dofile(filename);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage :\n\t%s cityname\n", argv[0]);
		exit(1);
	}

	deal_info(argv[1]);
	//检查之前有无json文件
	check_info();
	send_info();

	//分析json,输出
	deal_json();

	return 1;
}

