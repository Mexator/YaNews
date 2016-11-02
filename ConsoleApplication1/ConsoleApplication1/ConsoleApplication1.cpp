//master
#include "stdafx.h"
#include "curl\curl.h"
#include <iostream>
#include <fstream>
#include <codecvt>
#include <conio.h>
#include <Windows.h>
#pragma comment(lib,"libcurl.lib")
using namespace std;
//TODO ¬рем€ обновлени€, циклическое выполнение(?)
//ќконную версию
//5-th news
int write_file(char *ptr, size_t size, size_t nmemb, FILE *data);
void get_page();
void cl_buf(wchar_t *buffer);
void find_tag(wifstream &p, wchar_t *buffer);
void write_news(wifstream &p, wchar_t *buffer);
wchar_t *write_time(wifstream &p,wchar_t *buffer);

int main()
{
	wchar_t output[1032] = { 0 };
	do
	{
		get_page();

		wifstream page("body.html"); //file with html
		page.imbue(locale(locale(), new codecvt_utf8<wchar_t>)); //Presets for russian text output
		wcout.imbue(locale("rus_RUS.866"));
		write_time(page,output);
		find_tag(page,output);
		system("cls");
		wcout << output;
		page.close();
		cl_buf(output);
		if (_kbhit() && _getch() == 27) break;
	} while (1);
	//
	return 0;
}
void cl_buf(wchar_t *buffer)
{
	int a = wcslen(buffer);
	for (int i = 0; i < a;i++)
	{
		buffer[i] = '\0';
	}
}
void get_page()
{
	CURL *handle = NULL;
	FILE *write=NULL;
	fopen_s(&write, "body.html", "w");
	CURLcode res;
	char URL[] = { "https://www.yandex.ru" };
	handle = curl_easy_init();
	if (handle)
	{
		// задаем  url адрес
		curl_easy_setopt(handle, CURLOPT_URL, URL);

		curl_easy_setopt(handle, CURLOPT_WRITEDATA, write);
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_file);

		// выполн€ем запрос
		res = curl_easy_perform(handle);
		if (res != CURLE_OK) curl_easy_strerror(res);
	}
	fclose(write);
	curl_easy_cleanup(handle);
}
int write_file(char *ptr, size_t size, size_t nmemb, FILE *data)
{
	int i = fwrite(ptr, size, nmemb, data);
	return i;
}
void find_tag(wifstream &p, wchar_t *buffer)
{
	wchar_t findstr[] = { L"<li class=\"list__item\">" };
	int char_count = 0;
	while (!p.eof())
	{
		wchar_t ch = '\0';
		p >> ch;
		while (ch == findstr[char_count])
		{
			p.get(ch);
			if (char_count == wcslen(findstr) - 1)
			{
				write_news(p,buffer);
			}
			char_count++;
		}
		char_count = 0;
	}
}
wchar_t *write_time(wifstream &p, wchar_t *buffer)
{
	wchar_t findstr[] = { L"timestamp&quot;:&quot;" };
	wchar_t ch = NULL;
	int char_count = 0;
	while (!p.eof())
	{
		p.get(ch);
		while (ch == findstr[char_count])
		{
			if (char_count == wcslen(findstr) - 1)
			{
				int wr_count = 0;
				while ((ch = p.get()) != L'&')
				{
					buffer[wr_count] = ch;
					wr_count++;
				}
				buffer[wr_count] = L'\n';
				buffer[wr_count+1] = L'\0';
				return buffer;
			}
			p.get(ch);
			char_count++;
		}
		char_count = 0;
	}
}
void write_news(wifstream &p, wchar_t *buffer)
{
	wchar_t findstr[] = { L"aria-label=\"" };
	wchar_t ch = NULL;
	int char_count = 0;
	while (ch != L'>')
	{
		p.get(ch);
		while (ch == findstr[char_count])
		{

			if (char_count == wcslen(findstr) - 1)
			{
				while ((ch = p.get()) != L'\"')
				{
					buffer[wcslen(buffer)] = ch;
				}
				buffer[wcslen(buffer)] = L'\n';
			}
			p.get(ch);
			char_count++;
		}
		char_count = 0;
	}
}
