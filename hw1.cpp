// hw1.cpp : Defines the entry point for the console application.

/*
hw 1- part 3
Name: Venkata Satya Kavya Sree Bondapalli
UIN: 725006670
Course: CSCE 612 Networks and Distributed Processing 
Semester: Spring 2018
Instructor: Dmitri Loguinov
*/

#include "stdafx.h"
#include "string.h"
#include <stdio.h>
#include <cstdlib>
#include <winsock2.h>
#include <string>
#include <time.h>
#include <vector>
#include <unordered_set> 
#include <queue>          // std::queue
#include <math.h>

#define DEFAULT_BUFLEN 8192 //8KB

using namespace std;

//p1
//parse url
//doing dns
//connecting on page
//loading
//verifying header
//parsing page

//p2
//check host is unique
//dns lookup
//check ip is unique
//request robots
//check http code
//request page
//check http code
//parse page

//p3
//In computer programming, a mutex(mutual exclusion object) is a program object that is created so that multiple program thread can take turns sharing the same resource, such as access to a file.
//In computer science, a semaphore is a variable or abstract data type used to control access to a common resource by multiple processes in a concurrent system such as a multitasking operating system

#if REFERENCES
https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927(v=vs.85).aspx
https://stackoverflow.com/questions/8393990/interlockedincrement-vs-entercriticalsection-counter-leavecriticalsection
https://msdn.microsoft.com/en-us/library/windows/desktop/ms683597(v=vs.85).aspx
https://stackoverflow.com/questions/18693841/read-contents-of-a-file-as-hex-in-c
#endif

string gethostfromurl(char*url)
{
	
	char abc[] = "none";
	const char hash = '#';
	char* fragment;
	fragment = strchr(url, hash);
	char res[MAX_URL_LEN];
	memcpy(res, url, strlen(url));
	res[strlen(url)] = '\0';
	if (fragment != NULL)
	{
		memcpy(res, res, strlen(res) - strlen(fragment));
		res[strlen(url) - strlen(fragment)] = '\0';
	}

	const char ques = '?';
	char* query;
	query = strchr(res, ques);
	if (query != NULL)
	{
		query = query++;
		memcpy(res, res, strlen(res) - strlen(query) - 1);
		res[strlen(res) - strlen(query) - 1] = '\0';
	}

	const char* temp = "http://";
	char* residue;
	residue = strstr(res, temp);
	char* ret;
	ret = res;

	if (residue)
	{
		ret = ret + strlen("http://");
	}
	else {
		//the scheme is wrong
#if PRINT
		printf("%s failed with invalid scheme\n", url);
#endif
		//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
		return abc;
	}

	const char slash = '/';
	char* path;
	path = strchr(ret, slash);
	if (path != NULL)
	{
		path = path++;
		memcpy(ret, ret, strlen(ret) - strlen(path) - 1);
		ret[strlen(ret) - strlen(path) - 1] = '\0';
	}

	const char colon = ':';
	char* port_str;
	port_str = strchr(ret, colon);
	int port;
	if (port_str == NULL)
	{
		port = 80;
	}
	else
	{
		if (strlen(port_str) == 1)
		{
#if PRINT
			printf("failed with invalid port\n");
#endif
			//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
			return abc;
		}
		port_str++;
		port = atoi(port_str);

		if (port >= 1 && port <= 65535)
		{
			memcpy(ret, ret, strlen(ret) - strlen(port_str) - 1);
			ret[strlen(ret) - strlen(port_str) - 1] = '\0';
		}
		else
		{
#if PRINT
			printf("failed with invalid port\n");
#endif
			return abc;
		}

	}

	if (strlen(ret) >= MAX_HOST_LEN)
	{
#if PRINT
		printf("failed with violation of max host length\n");
#endif
		return abc;
	}

	//char host[MAX_HOST_LEN];
	//memcpy(host, ret, strlen(ret));
	//host[strlen(ret)] = '\0';

	//printf("ret 1 %s \n", ret);
	return string(ret);

}

#define PRINT 0
int hexCharToDecimal(char c)
{
	int res = -1;
	if (c >= '0'&&c <= '9')
	{
		res = c - '0';
	}
	else if (c >= 'A'&&c <= 'F')
	{
		res = c - 'A' + 10;
	}
	else if (c >= 'a'&&c <= 'f')
	{
		res = c - 'a' + 10;
	}
	return res;
}
void p1(char *url)
{
	printf("URL: %s\r\n\t", url);
	printf("  Parsing URL... ");
	//memcpy(url, argv[1], strlen(argv[1]) + 1);

	const char hash = '#';

	char* fragment;
	fragment = strchr(url, hash);

	char res[100];
	memcpy(res, url, strlen(url));
	res[strlen(url)] = '\0';

	if (fragment != NULL)
	{
		memcpy(res, res, strlen(res) - strlen(fragment));
		res[strlen(url) - strlen(fragment)] = '\0';
	}


	const char ques = '?';
	char* query;
	query = strchr(res, ques);

	if (query != NULL)
	{
		query = query++;
		memcpy(res, res, strlen(res) - strlen(query) - 1);
		res[strlen(res) - strlen(query) - 1] = '\0';
	}
	const char* temp = "http://";
	char* residue;
	residue = strstr(res, temp);

	char* ret;
	ret = res;

	if (residue)
	{
		ret = ret + strlen("http://");
	}
	else {
		//the scheme is wrong
		printf("failed with invalid scheme\n");
		//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
		return;
	}

	const char slash = '/';
	char* path;
	path = strchr(ret, slash);

	if (path != NULL)
	{
		path = path++;
		memcpy(ret, ret, strlen(ret) - strlen(path) - 1);
		ret[strlen(ret) - strlen(path) - 1] = '\0';
	}

	const char colon = ':';
	char* port_str;
	port_str = strchr(ret, colon);
	int port;
	if (port_str == NULL)
	{
		port = 80;
	}
	else
	{

		if (strlen(port_str) == 1)
		{
			printf("failed with invalid port\n");
			//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
			return;
		}
		port_str++;
		port = atoi(port_str);

		if (port >= 1 && port <= 65535)
		{
			memcpy(ret, ret, strlen(ret) - strlen(port_str) - 1);
			ret[strlen(ret) - strlen(port_str) - 1] = '\0';
		}
		else
		{
			printf("failed with invalid port\n");
			return;
		}

	}

	char host[100];
	memcpy(host, ret, strlen(ret));
	host[strlen(ret)] = '\0';

	char request[100];
	char* s = "/";
	request[0] = '/';
	unsigned int i = 0;

	if (path != NULL)
	{
		for (i = 1; i <strlen(path) + 1; i++)
		{
			request[i] = path[i - 1];
		}
	}
	else
	{
		i = i + 1;
		request[i] = '\0';
	}

	if (query == NULL)
	{
		request[i] = '\0';
	}
	else
	{
		request[i] = '?';
		unsigned int j = 0;
		for (i = i + 1; j <strlen(query); j++, i++)
		{
			request[i] = query[j];
		}
		request[i] = '\0';
	}

	printf("host %s, port %d, request %s\n\t", host, port, request);

	//Parsing of URL is done
	//HTTP request begins
	int len = strlen(host);
	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// open a TCP socket
	//open or close a TCP socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	printf("  Doing DNS... ");

	// structure used in DNS lookups
	struct hostent *remote;

	// structure for connecting to server
	struct sockaddr_in server;

	// first assume that the string is an IP address
	//determine the IP address of the server in the URL
	//converting into a 4-byte int
	DWORD IP_add = inet_addr(host);
	if (IP_add == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		//DNS lookup performed through a system call
		if ((remote = gethostbyname(host)) == NULL)
		{
			//printf("Invalid string: neither FQDN, nor IP address\n");
			printf("failed with %d\n", WSAGetLastError());
			return;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP_add;
	}
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms

	printf("done in %d ms, found %s\r\n\t", int(cpu_time_used), inet_ntoa(server.sin_addr));

	printf("* Connecting on page... ");
	// setup the port # and protocol type
	//connect socket to server on given port
	server.sin_family = AF_INET; //IPV4
	server.sin_port = htons(port);// htons(80);		// host-to-network flips the byte order

								  // connect to the server on port 80
	start = clock();
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		/*failed with 10060 for http: // csnet.cs.tamu.edu:443?addrbook.php */
		printf("here failed with %d\n", WSAGetLastError());
		return;
	}

	
	

	// send HTTP requests here
	//send request conforming to correct protocol
	int requestLength = DEFAULT_BUFLEN;

	//char *sendBuf = new char[requestLength];
	std::string sendBuf;

	sendBuf = "GET " + std::string(request) + " HTTP/1.1\r\n"
		+ "User-agent: kavyasree.bvs/1.1\r\n"
		+ "Host: " + std::string(host) + "\r\n"
		+ "Connection: close\r\n" + "\r\n";
	// place request into buf

	//printf("\n\nsendBuf \n%s\n", sendBuf.c_str());

	if (send(sock, sendBuf.c_str(), strlen(sendBuf.c_str()), 0) == SOCKET_ERROR)
	{
		printf("failed sending with error %d\n", WSAGetLastError());
		//return 0;
	}
	else
	{
		//
	}
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
	printf("done in %d ms\r\n\t", int(cpu_time_used));
	// close the socket to this server; open again for the next one
	//closesocket(sock);

	printf("  Loading... ");
	

	int iRes;
	//char fileBuf[DEFAULT_BUFLEN];
	char * fileBuf;
	fileBuf = (char *)malloc(DEFAULT_BUFLEN);
	int fileSize = DEFAULT_BUFLEN;

	// shutdown the connection since no more data will be sent
	iRes = shutdown(sock, SD_SEND);
	if (iRes == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		//return 1;
		return;
	}

	int k = 0;
	fd_set holder;
	FD_ZERO(&holder);
	FD_SET(sock, &holder);
	timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	start = clock();
	int rv;
	do
	{
		rv = select(0, &holder, NULL, NULL, &timeout);
		// wait to see if socket has any data (see MSDN) 
		if (rv > 0)
		{
			// new data available; now read the next segment 
			/*
			if (fileBuf + k == 0)
			{
			//
			return 0;
			}*/
			iRes = recv(sock, fileBuf + k, (fileSize - k), 0);

			//iRes = recv(sock, fileBuf + k, fileSize, 0);
			if (iRes > 0)
			{
				k = k + iRes;
			}
			else if (iRes == 0)
			{
				//printf("Connection closed\n");
			}
			else
			{
				/*10014 error on recv for http://128.194.135.72*/
				printf("failed with %d on recv\n", WSAGetLastError());
				return;
			}
			if (fileSize - k < 512)
			{
				fileSize = fileSize << 1;
				char* buf2 = (char*)realloc(fileBuf, fileSize);
				if (buf2 != NULL)
				{
					fileBuf = buf2;
				}
				else
				{
					free(fileBuf);
					break;
				}

			}
		}
		else if (rv == 0)
		{
			printf("timeout error\n");
			break;
		}
		else if (rv == SOCKET_ERROR)
		{
			printf("failed with %d on recv\n", WSAGetLastError());
			return;
		}
		else if (rv < 0)
		{
			printf("failed with %d on recv\n", WSAGetLastError());
			return;
		}
	} while (iRes > 0);
	if (rv == 0)
		return;
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
	printf("done in %d ms with %d bytes\r\n\t", int(cpu_time_used), k);
	closesocket(sock);
	// call cleanup when done with everything and ready to exit program
	WSACleanup();

	printf("  Verifying header... ");
	fileBuf[k] = '\0';
	//printf("\nfilebuf: %s\n", fileBuf);
	char *header_start = strstr(fileBuf, "HTTP/");
	if (header_start == NULL)
	{
		printf_s("failed with non-HTTP header\n");
		return;
	}
	//printf("header_start %s\n", header_start);
	int l = 0;
	char status[4];
	status[0] = header_start[9];
	status[1] = header_start[10];
	status[2] = header_start[11];
	status[3] = '\0';

	int status_code = atoi(status);
	printf("status code %d\r\n\t", status_code);
	char *header_end;
	header_end = strstr(header_start, "\r\n\r\n");
	if (status_code == 200)
	{
		char*tranEncodFound;
		tranEncodFound = strstr(header_start, "Transfer-Encoding:  chunked");
		if (tranEncodFound == NULL)
		{
			//
		}
		else
		{
			if (header_end != NULL)
			{
				printf("  Dechunking... body size was %d, now ", strlen(header_end));
				int chunked = 0;
				char *tranEncodStart;
				tranEncodStart = strstr(tranEncodFound, "\r\n\r\n");
				tranEncodStart = tranEncodStart + strlen("\r\n\r\n");
				//printf("\n tranEncodStart = %s\n", tranEncodStart);
				/*
				char c = '\n';
				printf("nxt line is %d\n", c);
				c = '\r';
				printf("\r is %d\n", c);*/
				int h = 0;
				while (tranEncodStart != NULL)
				{
					h++;
					tranEncodStart = tranEncodStart + strlen("\r\n");
					char*ee;
					ee = strstr(tranEncodStart, "\r\n");

					//printf(" h 1 is %d tranEncodStart |%s|\n", h,tranEncodStart);

					if (ee != NULL)
					{
						//printf("ee is [%s]\n", ee);
						char ww[DEFAULT_BUFLEN];
						memcpy(ww, tranEncodStart, strlen(tranEncodStart) - strlen(ee));
						//printf("lens are %d %d\n", strlen(tranEncodStart), strlen(ee));
						ww[strlen(tranEncodStart) - strlen(ee)] = '\0';
						//printf("here len <%s> %d len %d\n", ww, atoi(ww), strlen(ww));

						tranEncodStart = tranEncodStart + strlen("\r\n") + strlen(ww);// ;

																					  //printf(" 2 tranEncodStart |%s|\n", tranEncodStart);
						int *asciicode;
						long sum = 0;
						for (int i = strlen(ww) - 1; i >= 0; i--)
						{
							int val = hexCharToDecimal(ww[i]);
							//printf("val %d ", val);
							int power = strlen(ww) - (i + 1);
							//printf("pow %d\n",power);
							sum = sum + (val * (pow(16, (power))));
						}
						//printf("data in dec is %ld\n", sum);
						chunked = chunked + strlen(ww);
						if (sum == 0)
							break;
						tranEncodStart = tranEncodStart + sum;// +strlen("\r\n");
															  //printf(" 3 tranEncodStart |%s|\n", tranEncodStart);
															  //printf("here |%s| len %d\n", tranEncodStart,strlen(tranEncodStart));
					}
					else
					{
						char*ff;
						ff = strstr(tranEncodStart, "\r\n");
						if (ff != NULL)
						{
							char ww[DEFAULT_BUFLEN];
							memcpy(ww, tranEncodStart, strlen(tranEncodStart) - strlen(ff));
							ww[strlen(tranEncodStart) - strlen(ff)] = '\0';
							//printf("HERE tranEncodStart |%s|\n", tranEncodStart);
							if (tranEncodStart[0] == '0')
								break;
						}

					}
				}
				printf("%d\r\n\t", strlen(header_end) - chunked);
			}
		}

	
		
		printf("+ Parsing page... ");
		start = clock();
		HTMLParserBase *parser = new HTMLParserBase;
		//char baseUrl[] = url;		// where this page came from; needed for construction of relative links

		int nLinks;
		char *linkBuffer = parser->Parse(fileBuf, fileSize, url, (int)strlen(url), &nLinks);

		// check for errors indicated by negative values
		if (nLinks < 0)
			nLinks = 0;

		//printf("Found %d links:\n", nLinks);

		// print each URL; these are NULL-separated C strings
		for (int i = 0; i < nLinks; i++)
		{
			linkBuffer += strlen(linkBuffer) + 1;
		}

		delete parser;		// this internally deletes linkBuffer
							//delete fileBuf;
		end = clock();
		cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
		printf("done in %d ms with %d links\n", int(cpu_time_used), nLinks);
	}
	else
	{
		printf("\n");
	}

	//printf("header_start %s\n", header_start);
	
	if (header_end == NULL)
	{
		printf("---------------------------------------\n%s\r\n", header_start);
		return;
	}
	char* ch;
	int size_of_header = 0;
	for (ch = header_start; ch != header_end; ch++)
	{
		size_of_header++;
	}
	if (size_of_header == 0)
	{
		printf("---------------------------------------\n%s\r\n", header_start);
		return;
	}
	char header[DEFAULT_BUFLEN];
	//getting an error here
	memcpy(header, header_start, size_of_header);
	header[size_of_header] = '\0';
	printf("---------------------------------------\n%s\r\n", header);
	return;
}
void p2(int threads, char* input_file)
{
	char filename[100] ;
	memcpy(filename, input_file, strlen(input_file) + 1);
	filename[strlen(input_file) + 1] = '\0';
	//printf("\n%d %s\n", threads, filename);
	FILE * pFile;
	errno_t err = fopen_s(&pFile, filename, "rb");
	//printf()
	if (err != 0)
	{
		//printf("HERE 1\n");
		char buf[1000];
		strerror_s(buf, sizeof buf, err);
		fprintf_s(stderr, "cannot open file '%s': %s\n", filename, buf);
		fclose(pFile);
	}
	else
	{
		// open html file
		vector<string> urls;
		int size = 0;
		int i = 0;
		char temp[MAX_URL_LEN];
		while (fgets(temp, MAX_URL_LEN, pFile) != NULL)
		{
			//puts(temp);
			size = size + strlen(temp);
			int index = temp[strlen(temp) - 1];
			//printf("b4 len %d", strlen(temp));
			if (index == 10)
				temp[strlen(temp) - 2] = '\0';
			urls.push_back(temp);
			
			i++;
		}
		//printf("\nhere\n%s\n%d\nhere\n", urls[1].c_str(), strlen(url[1].c_str()));
		printf("Opened %s with size %d\n", filename, size);
		/*
		int tot_urls = i;
		printf("Tot urls = %d size = %d\n", tot_urls, size);
		for( int j =0; j<tot_urls;j++)
		{ 
			printf("%s\n", urls[j].c_str());
		}
		*/
		int tot_urls = i;
		DWORD IP = inet_addr("128.194.135.72");
		unordered_set<DWORD> seenIPs;
		//seenIPs.insert(IP);
		//---------
		unordered_set<string> seenHosts;
		// populate with some initial elements
		//seenHosts.insert("www.google.com");
		//seenHosts.insert("www.tamu.edu");
		//seenHosts.insert("www.cse.tamu.edu");
		for (int j = 0; j < tot_urls; j++)
		{
			char url[MAX_URL_LEN];
			memcpy(url, (urls[j]).c_str(), strlen((urls[j]).c_str()));
			url[strlen((urls[j]).c_str())] = '\0';
			printf("URL: %s\n", url);
			//printf("\nurl is %s \nlen of url %d\n", url, strlen(url));
			
			printf("\t  Parsing URL... ");
			//memcpy(url, argv[1], strlen(argv[1]) + 1);

			const char hash = '#';
			char* fragment;
			fragment = strchr(url, hash);
			char res[MAX_URL_LEN];
			memcpy(res, url, strlen(url));
			res[strlen(url)] = '\0';
			if (fragment != NULL)
			{
				memcpy(res, res, strlen(res) - strlen(fragment));
				res[strlen(url) - strlen(fragment)] = '\0';
			}

			const char ques = '?';
			char* query;
			query = strchr(res, ques);
			if (query != NULL)
			{
				query = query++;
				memcpy(res, res, strlen(res) - strlen(query) - 1);
				res[strlen(res) - strlen(query) - 1] = '\0';
			}
			
			const char* temp = "http://";
			char* residue;
			residue = strstr(res, temp);
			char* ret;
			ret = res;

			if (residue)
			{
				ret = ret + strlen("http://");
			}
			else {
				//the scheme is wrong
				printf("failed with invalid scheme\n");
				//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
				return;
			}

			const char slash = '/';
			char* path;
			path = strchr(ret, slash);
			if (path != NULL)
			{
				path = path++;
				memcpy(ret, ret, strlen(ret) - strlen(path) - 1);
				ret[strlen(ret) - strlen(path) - 1] = '\0';
			}

			const char colon = ':';
			char* port_str;
			port_str = strchr(ret, colon);
			int port;
			if (port_str == NULL)
			{
				port = 80;
			}
			else
			{

				if (strlen(port_str) == 1)
				{
					printf("failed with invalid port\n");
					//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
					continue;
				}
				port_str++;
				port = atoi(port_str);

				if (port >= 1 && port <= 65535)
				{
					memcpy(ret, ret, strlen(ret) - strlen(port_str) - 1);
					ret[strlen(ret) - strlen(port_str) - 1] = '\0';
				}
				else
				{
					printf("failed with invalid port\n");
					continue;
				}

			}

			char host[MAX_HOST_LEN];
			memcpy(host, ret, strlen(ret));
			host[strlen(ret)] = '\0';
			char request[MAX_REQUEST_LEN];
			char* s = "/";
			request[0] = '/';
			unsigned int i = 0;

			if (path != NULL)
			{
				for (i = 1; i <strlen(path) + 1; i++)
				{
					request[i] = path[i - 1];
				}
			}
			else
			{
				i = i + 1;
				request[i] = '\0';
			}

			if (query == NULL)
			{
				request[i] = '\0';
			}
			else
			{
				request[i] = '?';
				unsigned int j = 0;
				for (i = i + 1; j <strlen(query); j++, i++)
				{
					request[i] = query[j];
				}
				request[i] = '\0';
			}
			
			printf("host %s, port %d\n", host, port);
			//printf("host %s, port %d, request %s\n", host, port, request);

			//Parsing of URL is done
			//Check for host uniqueness
			
			int prevSize = seenHosts.size();
			seenHosts.insert(host);
			if (seenHosts.size() > prevSize)
			{
				// unique host
				printf("\t  Checking host uniqueness... passed\n");
			}
			else
			{
				// duplicate host
				printf("\t  Checking host uniqueness... failed\n");
				continue;
			}
			//host uniqueness is done

			//do dns lookup
			int len = strlen(host);
			WSADATA wsaData;
			//Initialize WinSock; once per program run
			WORD wVersionRequested = MAKEWORD(2, 2);
			if (WSAStartup(wVersionRequested, &wsaData) != 0) {
				printf("WSAStartup error %d\n", WSAGetLastError());
				WSACleanup();
				continue;
			}
			// open a TCP socket
			//open or close a TCP socket
			clock_t start, end;
			double cpu_time_used;
			
			start = clock();
			printf("\t  Doing DNS... ");
			// structure used in DNS lookups
			struct hostent *remote;
			// structure for connecting to server
			struct sockaddr_in server;
			// first assume that the string is an IP address
			//determine the IP address of the server in the URL
			//converting into a 4-byte int
			DWORD IP_add = inet_addr(host);
			if (IP_add == INADDR_NONE)
			{
				// if not a valid IP, then do a DNS lookup
				//DNS lookup performed through a system call
				if ((remote = gethostbyname(host)) == NULL)
				{
					//printf("Invalid string: neither FQDN, nor IP address\n");
					printf("failed with %d\n", WSAGetLastError());
					continue;
				}
				else // take the first IP address and copy into sin_addr
					memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
			}
			else
			{
				// if a valid IP, directly drop its binary version into sin_addr
				server.sin_addr.S_un.S_addr = IP_add;
			}
			server.sin_family = AF_INET; //IPV4
			server.sin_port = htons(port);// htons(80);		// host-to-network flips the byte order
			end = clock();
			cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
			printf("done in %d ms, found %s\r\n", int(cpu_time_used), inet_ntoa(server.sin_addr));
			
			int prevSizeIP = seenIPs.size();
			seenIPs.insert(server.sin_addr.S_un.S_addr);

			if (seenIPs.size() > prevSizeIP)
			{
				// unique host
				printf("\t  Checking IP uniqueness... passed\n");
			}
			else
			{
				// duplicate host
				printf("\t  Checking IP uniqueness... failed\n");
				continue;
			}
			
			//initialize a socket
			printf("\t  Connecting on robots... ");
			SOCKET robot = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (robot == INVALID_SOCKET)
			{
				printf("socket() generated error %d\n", WSAGetLastError());
				WSACleanup();
				continue;
			}
			//connect 
			start = clock();
			if (connect(robot, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
			{
				/*failed with 10060 for http: // csnet.cs.tamu.edu:443?addrbook.php */
				printf("failed with %d\n", WSAGetLastError());
				continue;
			}
			//send
			std::string robotsendBuf;
			robotsendBuf = "HEAD /robots.txt HTTP/1.0\r\nUser-agent: kavyasree.bvs/1.2\r\nHost: " + std::string(host) + "\r\n"
				+ "Connection: close\r\n" + "\r\n";
			/*	
			robotsendBuf = "HEAD " + std::string(request) + "/robots.txt HTTP/1.0\r\n"
				+ "User-agent: kavyasree.bvs/1.2\r\n"
				+ "Host: " + std::string(host) + "\r\n"
				+ "Connection: close\r\n" + "\r\n";*/

			// place request into buf
			//printf("\nrobotsendBuf \n%s\n", robotsendBuf.c_str());
			if (send(robot, robotsendBuf.c_str(), strlen(robotsendBuf.c_str()), 0) == SOCKET_ERROR)
			{
				printf("failed sending with error %d\n", WSAGetLastError());
				continue;
			}
			else
			{
				//printf("robot socket send successful\n");
			}
			end = clock();
			cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
			printf("done in %d ms\r\n\t", int(cpu_time_used));
			//receive

			printf("  Loading... ");
			int iRes;
			//char fileBuf[DEFAULT_BUFLEN];
			char * fileBuf;
			fileBuf = (char *)malloc(DEFAULT_BUFLEN);
			int fileSize = DEFAULT_BUFLEN;
			// shutdown the connection since no more data will be sent
			iRes = shutdown(robot, SD_SEND);
			if (iRes == SOCKET_ERROR) {
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(robot);
				WSACleanup();
				//return 1;
				continue;
			}
			int k = 0;
			fd_set holder;
			FD_ZERO(&holder);
			FD_SET(robot, &holder);
			timeval timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			start = clock();
			int rv;
			int first = 0;
			int exi = 0;
			bool failed_with_exceeding_max_flag = false;
			do
			{
				rv = select(0, &holder, NULL, NULL, &timeout);
				/*
				printf("\nrv %d\n", rv);
				// wait to see if socket has any data (see MSDN) 
				
				if (first == 0 && int((clock() - start) * 1000) > 10 * 1000)
				{
					//printf("timeout error\n");
					printf("here failed with slow download\n");
					rv = 0;
					break;
				}
				else if(first == 1&& int((clock() - start) * 1000) > 1 * 1000)
				{
					//printf("timeout error\n");
					printf("here failed with slow download\n");
					rv = 0;
					break;
				}
				*/
				if (rv > 0)
				{
					// new data available; now read the next segment 
					/*
					if (fileBuf + k == 0)
					{
					//
					return 0;
					}*/
					iRes = recv(robot, fileBuf + k, (fileSize - k), 0);
					//iRes = recv(sock, fileBuf + k, fileSize, 0);
					if (iRes > 0)
					{
						k = k + iRes;
						if (k > 16 * 1024)
						{
							printf("failed with exceeding max\n");
							failed_with_exceeding_max_flag = true;
						}
					}
					else if (iRes == 0)
					{
						//printf("Connection closed\n");
					}
					else
					{
						/*10014 error on recv for http://128.194.135.72*/
						printf("failed with %d on recv\n", WSAGetLastError());
						continue;
					}
					if (failed_with_exceeding_max_flag)
						break;
					if (fileSize - k < 512)
					{
						fileSize = fileSize << 1;
						char* buf2 = (char*)realloc(fileBuf, fileSize);
						if (buf2 != NULL)
						{
							fileBuf = buf2;
						}
						else
						{
							free(fileBuf);
							break;
						}

					}
				}
				else if (rv == 0)
				{
					//printf("timeout error\n");
					printf("failed with slow download\n");
					return;
				}
				else if (rv == SOCKET_ERROR)
				{
					printf("failed with %d on recv\n", WSAGetLastError());
					//doubt
					continue;
				}
				else if (rv < 0)
				{
					printf("failed with %d on recv\n", WSAGetLastError());
					//doubt
					continue;
				}
				timeout.tv_sec = 1;
			} while (iRes > 0);
			if (failed_with_exceeding_max_flag)
				continue;
			if (rv == 0)
				continue;
			end = clock();
			cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
			//printf("\nfileBuf %s\n", fileBuf);
			printf("done in %d ms with %d bytes\r\n", int(cpu_time_used), k);
			closesocket(robot);
			// call cleanup when done with everything and ready to exit program
			
			printf("\t  Verifying header... ");
			fileBuf[k] = '\0';
			//printf("\nfileBuf \n%s\n", fileBuf);
			char *header_start = strstr(fileBuf, "HTTP/");
			if (header_start == NULL)
			{
				printf_s("failed with non-HTTP header\n");
				continue;
			}
			//printf("header_start %s\n", header_start);
			int l = 0;
			char status[4];
			status[0] = header_start[9];
			status[1] = header_start[10];
			status[2] = header_start[11];
			status[3] = '\0';
			int status_code = atoi(status);
			printf("status code %d\r\n", status_code);

			if (status_code > 399 && status_code <500)
			{
				//Connect to page
				printf("\t* Connecting on page... ");
				SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (sock == INVALID_SOCKET)
				{
					printf("socket() generated error %d\n", WSAGetLastError());
					WSACleanup();
					return;
				}
				start = clock();
				if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
				{
					/*failed with 10060 for http: // csnet.cs.tamu.edu:443?addrbook.php */
					printf("failed with %d\n", WSAGetLastError());
					continue;
				}
				//send
				std::string sendBuf;
				//printf("\nhere %s here\n", request);
				sendBuf = "GET " + std::string(request) + " HTTP/1.0\r\n"
					+ "User-agent: kavyasree.bvs/1.2\r\n"
					+ "Host: " + std::string(host) + "\r\n"
					+ "Connection: close\r\n" + "\r\n";
				// place request into buf
				//printf("\nsendBuf \n%s\n", sendBuf.c_str());
				if (send(sock, sendBuf.c_str(), strlen(sendBuf.c_str()), 0) == SOCKET_ERROR)
				{
					printf("failed sending with error %d\n", WSAGetLastError());
					continue;
				}
				else
				{
					//printf("robot socket send successful\n");
				}
				end = clock();
				cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
				printf("done in %d ms\r\n\t", int(cpu_time_used));
				
				//receive
				//Load d page
				printf("  Loading... ");
				int iRes;
				//char fileBuf[DEFAULT_BUFLEN];
				char * fileBuf1;
				fileBuf1 = (char *)malloc(DEFAULT_BUFLEN);
				int fileSize1 = DEFAULT_BUFLEN;
				// shutdown the connection since no more data will be sent
				iRes = shutdown(sock, SD_SEND);
				if (iRes == SOCKET_ERROR) {
					printf("shutdown failed with error: %d\n", WSAGetLastError());
					closesocket(sock);
					WSACleanup();
					//return 1;
					return;
				}
				int k = 0;
				fd_set sockholder;
				FD_ZERO(&sockholder);
				FD_SET(sock, &sockholder);
				timeval timeout1;
				timeout1.tv_sec = 10;
				timeout1.tv_usec = 0;
				start = clock();
				do
				{
					if (k > 2 * 1024*1024)
					{
						printf("failed with exceeding max\n");
						failed_with_exceeding_max_flag = true;
					}
					if (failed_with_exceeding_max_flag)
						break;
					int rv = select(0, &sockholder, NULL, NULL, &timeout);
					// wait to see if socket has any data (see MSDN) 
					if (rv > 0)
					{
						// new data available; now read the next segment 
						/*
						if (fileBuf + k == 0)
						{
						//
						return 0;
						}*/
						iRes = recv(robot, fileBuf1 + k, (fileSize1 - k), 0);
						//iRes = recv(sock, fileBuf + k, fileSize, 0);
						if (iRes > 0)
						{
							k = k + iRes;
						}
						else if (iRes == 0)
						{
							//printf("Connection closed\n");
						}
						else
						{
							/*10014 error on recv for http://128.194.135.72*/
							printf("failed with %d on recv\n", WSAGetLastError());
							continue;
						}
						if (fileSize1 - k < 512)
						{
							fileSize1 = fileSize1 << 1;
							char* buf2 = (char*)realloc(fileBuf1, fileSize1);
							if (buf2 != NULL)
							{
								fileBuf1 = buf2;
							}
							else
							{
								free(fileBuf1);
								break;
							}
						}
					}
					else if (rv == 0)
					{
						//printf("timeout error\n");
						printf("failed with slow download\n");
						return;
					}
					else if (rv == SOCKET_ERROR)
					{
						printf("failed with %d on recv\n", WSAGetLastError());
						return;
					}
					else if (rv < 0)
					{
						printf("failed with %d on recv\n", WSAGetLastError());
						return;
					}
				} while (iRes > 0);
				if (failed_with_exceeding_max_flag)
					continue;
				end = clock();
				cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
				//printf("\nfileBuf %s\n", fileBuf);
				printf("done in %d ms with %d bytes\n", int(cpu_time_used), k);
				closesocket(sock);
				
				//verify header
				// call cleanup when done with everything and ready to exit program
				WSACleanup();

				printf("\t  Verifying header... ");
				fileBuf1[k] = '\0';
				//printf("\nfileBuf1 %s\n", fileBuf1);
				char *header_start = strstr(fileBuf1, "HTTP/");
				if (header_start == NULL)
				{
					printf_s("failed with non-HTTP header\n");
					continue;
				}
				//printf("header_start %s\n", header_start);
				int l = 0;
				char status[4];
				status[0] = header_start[9];
				status[1] = header_start[10];
				status[2] = header_start[11];
				status[3] = '\0';

				int status_code = atoi(status);
				printf("status code %d\n", status_code);
				//parse d page
				if (status_code == 200)
				{
					printf("\t+ Parsing page... ");
					start = clock();
					HTMLParserBase *parser = new HTMLParserBase;
					//char baseUrl[] = url;		// where this page came from; needed for construction of relative links

					int nLinks;
					char *linkBuffer = parser->Parse(fileBuf1, fileSize1, url, (int)strlen(url), &nLinks);

					// check for errors indicated by negative values
					if (nLinks < 0)
						nLinks = 0;

					//printf("Found %d links:\n", nLinks);

					// print each URL; these are NULL-separated C strings
					for (int i = 0; i < nLinks; i++)
					{
						//printf("%s\n", linkBuffer);
						linkBuffer += strlen(linkBuffer) + 1;
					}

					delete parser;		// this internally deletes linkBuffer
										//delete fileBuf;
					end = clock();
					cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
					printf("done in %d ms with %d links\n", int(cpu_time_used), nLinks);
				}
				else
				{
					//printf("\n");
				}
				
			}
			else
			{
				continue;
			}
			WSACleanup();
			//printf("\n");
			fclose(pFile);
		}
	}
	return;
}

// this class is passed to all threads, acts as shared memory
class Parameters {
public:
	HANDLE	mutex;
	HANDLE	finished;
	HANDLE	eventQuit;
	
	queue<string> Q;
	std::string inputFile;

	volatile long  E; // number of extracted URLs from the queue
	volatile long  H; // number of URLs that have passed host uniqueness
	volatile long  D; // number of successful DNS lookups
	volatile long  I; // number of URLs that have passed IP uniqueness
	volatile long  R; // number of URLs that have passed robots checks
	volatile long  C; // number of successfully crawled URLs (those with a valid HTTP code)
	volatile long  L; // total links found 
	volatile long noOfBytes;
	volatile long noOfActiveThreads = 0;
	volatile long C_two = 0;
	volatile long C_three = 0;
	volatile long C_four = 0;
	volatile long C_five = 0;
	volatile long C_other = 0;
	volatile long hyperlinkToTamuDomain;
	volatile long outsideTamu;

	unordered_set<string> seenIPsDB;
	unordered_set<string> seenHostsDB;
	bool IsItUniqueIP(string IP)
	{
		WaitForSingleObject(mutex, INFINITE);
		int prevsize = seenIPsDB.size();
		seenIPsDB.insert(IP);
		//printf("prevsize %d, seenHostsIP.size() %d", prevsize, seenHostsIP.size());
		int currsize = seenIPsDB.size();
		ReleaseMutex(mutex);
		if (currsize>prevsize)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	bool IsItUniqueHost(string host)
	{
		WaitForSingleObject(mutex, INFINITE);
		int prevsize = seenHostsDB.size();
		seenHostsDB.insert(host);
		//printf("prevsize %d, seenHostsDB.size() %d", prevsize, seenHostsDB.size());
		int currsize = seenHostsDB.size();
		ReleaseMutex(mutex);
		if(currsize>prevsize)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
};
UINT StatsRun(LPVOID pParam)
{
	Parameters *p = (Parameters*)pParam; // shared parameters
	static int i = 0;
	long prev_c = 0;
	long prev_bytes = 0;
	while (WaitForSingleObject(p->eventQuit, 2000) == WAIT_TIMEOUT)
	{
		i = i + 2;
		// print
		WaitForSingleObject(p->mutex, INFINITE);
		printf("[%3d] %4d Q %6d E %7d H %6d D %6d I %5d R %5d C %5d L %4dK\n", 
			i, 
			p->noOfActiveThreads,
			p->Q.size(), //should be the no. of active threads
			p->E, //extracted urls
			p->H, //unique host
			p->D, //dns lookup
			p->I, //unique ip
			p->R, //robot checks
			p->C, //crawled urls
			p->L/1000);// links
		printf("\t*** crawling %.1f pps @ %.1f Mbps\n", (float)(p->C-prev_c)/2, (float)(p->noOfBytes-prev_bytes)*8/(2*1000*1000));
		prev_c = p->C;
		prev_bytes = p->noOfBytes;
#if 1
		if (p->noOfActiveThreads == 0 )
		{
			ReleaseMutex(p->mutex);
			break;
		}
#endif
		ReleaseMutex(p->mutex);
	}
	WaitForSingleObject(p->mutex, INFINITE);
	printf("Extracted %d URLs @ %d/s\n", p->E, p->E / i);
	//the hosts that have passed host uniqueness are looked up for dns
	printf("Looked up %d DNS names @ %d/s\n", p->H, p->H / i);
	//the robots are checked for those have passed ip uniqueness
	printf("Downloaded %d robots @ %d/s\n", p->I, p->I / i); 
	//the pages that have been crawled
	printf("Crawled %d pages @ %d/s (%.2f MB)\n", p->C, p->C / i, (float)p->noOfBytes / ( 1000 * 1000));
	printf("Parsed %d links @ %d/s\n", p->L, p->L / i);
	printf("HTTP codes: 2xx = %d, 3xx = %d, 4xx = %d, 5xx = %d, other = %d\n", 
		p->C_two,
		p->C_three,
		p->C_four,
		p->C_five,
		p->C_other);
	/*
	//printf("2- Avg no of links / html page with 2xx code = %d\n", p->L)
	printf("3- Avg page size in bytes (across all http codes) = %d bytes\n", p->noOfBytes / p->C);
	printf("3- Bandwidth needed for bing to crawl 10B pages per day = %.2f Gbps\n", float(p->noOfBytes*8 / p->C) * 1000 * 1000 * 1000 / (24 * 3600 * 1000 * 1000));
	printf("4- Prob that a link in the I/P file contains a unique host = %.2f\n", (float)p->H / p->E);
	printf("4- Prob that a unique host has a valid dns record = %.2f\n", (float)p->D / p->H);
	printf("4- Percentage of contacted sites that had a 4xx robots file = %.2f\n", (float)p->R * 100 / p->I);
	*/
	//printf("5- No of crawled 2xx pages that contain a hyperlink to tamu.edu = %d, And %d of them originated from outside of TAMU\n", p->hyperlinkToTamuDomain, p->outsideTamu);
	
	ReleaseMutex(p->mutex);
	return 0;
}
int crawlURL(string x, LPVOID pParam, HTMLParserBase *parser)
{
	Parameters *p = ((Parameters*)pParam);
	char url[MAX_URL_LEN];
	//memcpy(url, (x).c_str(), strlen((x).c_str()) + 1);
	//url[strlen((x).c_str()) - 2] = '\0';
	
	memcpy(url, (x).c_str(), strlen((x).c_str()));
	url[strlen((x).c_str())] = '\0';


#if PRINT
	printf("URL: %s here %d \n", url, strlen(url));
	printf("\t  Parsing URL... ");
#endif
#if 1
	const char hash = '#';
	char* fragment;
	fragment = strchr(url, hash);
	char res[MAX_URL_LEN];
	memcpy(res, url, strlen(url));
	res[strlen(url)] = '\0';
	if (fragment != NULL)
	{
		memcpy(res, res, strlen(res) - strlen(fragment));
		res[strlen(url) - strlen(fragment)] = '\0';
	}

	const char ques = '?';
	char* query;
	query = strchr(res, ques);
	if (query != NULL)
	{
		query = query++;
		memcpy(res, res, strlen(res) - strlen(query) - 1);
		res[strlen(res) - strlen(query) - 1] = '\0';
	}

	const char* temp = "http://";
	char* residue;
	residue = strstr(res, temp);
	char* ret;
	ret = res;
	//printf("ret 1 %s len is %d\n", ret, strlen(ret));
	if (residue)
	{
		ret = ret + strlen("http://");
		//printf("ret 2 %s len is %d\n", ret, strlen(ret));
	}

	else {
		//the scheme is wrong
#if PRINT
		printf("%s failed with invalid scheme\n", url);
#endif
		//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
		return 1;
	}

	const char slash = '/';
	char* path;
	path = strchr(ret, slash);
	if (path != NULL)
	{
		path = path++;
		memcpy(ret, ret, strlen(ret) - strlen(path) - 1);
		ret[strlen(ret) - strlen(path) - 1] = '\0';
	}

	const char colon = ':';
	char* port_str;
	port_str = strchr(ret, colon);
	int port;
	if (port_str == NULL)
	{
		port = 80;
	}
	else
	{
		if (strlen(port_str) == 1)
		{
#if PRINT
			printf("%s failed with invalid port\n", url);
#endif
			//scheme://[user:pass@]host[:port][/path][?query][#fragment] 
			return 1;
		}
		port_str++;
		port = atoi(port_str);

		if (port >= 1 && port <= 65535)
		{
			memcpy(ret, ret, strlen(ret) - strlen(port_str) - 1);
			ret[strlen(ret) - strlen(port_str) - 1] = '\0';
		}
		else
		{
#if PRINT
			printf("%s failed with invalid port\n", url);
#endif
			return 1;
		}

	}

	if (strlen(ret) >= MAX_HOST_LEN)
	{
#if PRINT
		printf("%s failed with violation of max host length\n", url);
#endif
		return 0;
	}

	char host[MAX_HOST_LEN];
	//printf("ret is %s len is %d\n", ret, strlen(ret));
	memcpy(host, ret, strlen(ret));
	host[strlen(ret)] = '\0';

	char request[MAX_REQUEST_LEN];
	char* s = "/";
	request[0] = '/';
	unsigned int i = 0;
	
	if (path != NULL)
	{
		//printf("path %s\n", path);
		if (strlen(request) + strlen(path) + 1 >= MAX_REQUEST_LEN)
		{
#if PRINT
			printf("%s failed with violation of max request length\n", url);
#endif
			return 0;
		}/**/
		for (i = 1; i < strlen(path) + 1; i++)
		{
			request[i] = path[i - 1];
		}
	}
	else
	{
		i = i + 1;
		request[i] = '\0';
	}

	if (query == NULL)
	{
		request[i] = '\0';
	}
	else
	{
		if (strlen(request) + strlen(query) + 1 >= MAX_REQUEST_LEN)
		{
#if PRINT
			printf("%s failed with violation of max request length\n", url);
#endif
			return 0;
		}/**/
		request[i] = '?';
		unsigned int j = 0;
		for (i = i + 1; j < strlen(query); j++, i++)
		{
			request[i] = query[j];
		}
		request[i] = '\0';
	}

#if PRINT
	printf("\t  host %s, port %d\n", host, port);
	printf("\t  request %s\n", request);
#endif
	//Parsing of URL is done
	//Check for host uniqueness
	LONG tidUnique;
	WaitForSingleObject(p->mutex, INFINITE);
	//printf("here 2 \n");
	if (p->IsItUniqueHost(host))
	{
#if PRINT
		printf("\t  Checking IP uniqueness... passed\n");
#endif
		tidUnique = InterlockedIncrement(&(p->H));
		ReleaseMutex(p->mutex);
	}
	else
	{
#if PRINT
		printf("\t  Checking IP uniqueness... failed\n");
#endif
		ReleaseMutex(p->mutex);
		return 0;
	}
	//host uniqueness is done
	//printf("here 1\n");
	//do dns lookup
	int len = strlen(host);
	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
#if PRINT
		printf("%s WSAStartup error %d\n", url,WSAGetLastError());
#endif
		WSACleanup();
		return 1;
	}
	// open a TCP socket
	//open or close a TCP socket
#if PRINT
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	printf("\t  Doing DNS... ");
#endif
	// structure used in DNS lookups
	struct hostent *remote;
	// structure for connecting to server
	struct sockaddr_in server;
	// first assume that the string is an IP address
	//determine the IP address of the server in the URL
	//converting into a 4-byte int
	DWORD IP_add = inet_addr(host);
	if (IP_add == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		//DNS lookup performed through a system call
		if ((remote = gethostbyname(host)) == NULL)
		{
			//printf("Invalid string: neither FQDN, nor IP address\n");
#if PRINT
			printf("Invalid string:%s %s failed with %d\n", url,host,WSAGetLastError());
#endif
			return 1;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP_add;
	}
	server.sin_family = AF_INET; //IPV4
	server.sin_port = htons(port);// htons(80);		// host-to-network flips the byte order
#if PRINT//
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
#endif
	WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
	tidUnique = InterlockedIncrement(&(p->D)); ReleaseMutex(p->mutex);
#if PRINT//#endif
	printf("done in %d ms, found %s\r\n", int(cpu_time_used), inet_ntoa(server.sin_addr));
#endif
	WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
	if (p->IsItUniqueIP(inet_ntoa(server.sin_addr)))//server.sin_addr.S_un.S_addr))
	{
#if PRINT
		printf("\t  Checking IP uniqueness... passed\n");
#endif
		tidUnique = InterlockedIncrement(&(p->I));
		ReleaseMutex(p->mutex);
	}
	else
	{
		ReleaseMutex(p->mutex);
		return 0;
	}
	

	//initialize a socket
#if PRINT
	printf("\t  Connecting on robots... ");
#endif
	SOCKET robot = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (robot == INVALID_SOCKET)
	{
#if PRINT
		printf("%s socket() generated error %d\n", url,WSAGetLastError());
#endif
		WSACleanup();
		return 1;
	}
	//connect 
#if PRINT//#endif
	start = clock();
#endif
	if (connect(robot, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		/*failed with 10060 for http: // csnet.cs.tamu.edu:443?addrbook.php */
#if PRINT
		printf("%s failed with %d\n",url, WSAGetLastError());
#endif
		return 1;
	}
	//send
	std::string robotsendBuf;
	robotsendBuf = "HEAD /robots.txt HTTP/1.0\r\nUser-agent: kavyasree.bvs/1.2\r\nHost: " + std::string(host) + "\r\n"
		+ "Connection: close\r\n" + "\r\n";
	if (send(robot, robotsendBuf.c_str(), strlen(robotsendBuf.c_str()), 0) == SOCKET_ERROR)
	{
#if PRINT
		printf("%s, failed sending with error %d\n", url,WSAGetLastError());
#endif
		return 1;
	}
	else
	{
		//printf("robot socket send successful\n");
	}
#if PRINT
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
	printf("done in %d ms\r\n\t", int(cpu_time_used));
#endif
	//receive
#if PRINT
	printf("  Loading... ");
#endif
	int iRes;
	//char fileBuf[DEFAULT_BUFLEN];
	char * fileBuf;
	fileBuf = (char *)malloc(DEFAULT_BUFLEN);
	int fileSize = DEFAULT_BUFLEN;
	// shutdown the connection since no more data will be sent
	iRes = shutdown(robot, SD_SEND);
	if (iRes == SOCKET_ERROR) {
#if PRINT
		printf("%s shutdown failed with error: %d\n",url, WSAGetLastError());
#endif
		closesocket(robot);
		WSACleanup();
		//return 1;
		return 1;
	}
	int k = 0;
	fd_set holder;
	FD_ZERO(&holder);
	FD_SET(robot, &holder);
	timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
#if PRINT
	start = clock();
#endif
	int rv;
	int first = 0;
	int exi = 0;
	do
	{
		rv = select(0, &holder, NULL, NULL, &timeout);
		if (rv > 0)
		{
			// new data available; now read the next segment
			iRes = recv(robot, fileBuf + k, (fileSize - k), 0);
			if (iRes > 0)
			{
				k = k + iRes;
				if (k > 16 * 1024)
				{
#if PRINT
					printf("%s failed with exceeding max\n",url);
#endif
					WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
					tidUnique = InterlockedExchangeAdd(&(p->noOfBytes), k); ReleaseMutex(p->mutex);
					return 1;
				}
			}
			else if (iRes == 0)
			{
				//printf("Connection closed\n");
			}
			else
			{
				/*10014 error on recv for http://128.194.135.72*/
#if PRINT
				printf("%s failed with %d on recv\n",url, WSAGetLastError());
#endif
				return 1;
			}
			if (fileSize - k < 512)
			{
				fileSize = fileSize << 1;
				char* buf2 = (char*)realloc(fileBuf, fileSize);
				if (buf2 != NULL)
				{
					fileBuf = buf2;
				}
				else
				{
					free(fileBuf);
					break;
				}

			}
		}
		else if (rv == 0)
		{
			//printf("timeout error\n");
#if PRINT
			printf("%s failed with slow download\n", url);
#endif
			return 1;
		}
		else if (rv == SOCKET_ERROR)
		{
#if PRINT
			printf("%s failed with %d on recv\n",url, WSAGetLastError());
			//doubt
#endif
			return 1;
		}
		else if (rv < 0)
		{
#if PRINT
			printf("%s failed with %d on recv\n", url,WSAGetLastError());
#endif
			//doubt
			return 1;
		}
		timeout.tv_sec = 1;
	} while (iRes > 0);
	if (rv == 0)
		return 1;
#if PRINT
	end = clock();
	cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
	//printf("\nfileBuf %s\n", fileBuf);
	printf("done in %d ms with %d bytes\r\n", int(cpu_time_used), k);
#endif
	WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
	tidUnique = InterlockedExchangeAdd(&(p->noOfBytes), k); ReleaseMutex(p->mutex);
	closesocket(robot);
	// call cleanup when done with everything and ready to exit program
#if PRINT
	printf("\t  Verifying header... ");
#endif
	fileBuf[k] = '\0';
	//printf("\n%s fileBuf \n%s\n", url,fileBuf);
	char *header_start = strstr(fileBuf, "HTTP/");
	if (header_start == NULL)
	{
#if PRINT
		printf_s("%s failed with non-HTTP header\n", url);
#endif
		return 1;
	}
	//printf("header_start %s\n", header_start);
	int l = 0;
	char status[4];
	status[0] = header_start[9];
	status[1] = header_start[10];
	status[2] = header_start[11];
	status[3] = '\0';
	int status_code = atoi(status);
#if PRINT
	printf("%s status code %d\r\n", url,status_code);
#endif
	//valid http code for robots is 4xx
	if (status_code > 399 && status_code < 500)
	{
		WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
		tidUnique = InterlockedIncrement(&(p->R));
		ReleaseMutex(p->mutex);
		//Connect to page
#if PRINT
		printf("\t* Connecting on page... ");
#endif
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			printf("%s socket() generated error %d\n", url,WSAGetLastError());
			WSACleanup();
			return 1;
		}
#if PRINT
		start = clock();
#endif
		if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			/*failed with 10060 for http: // csnet.cs.tamu.edu:443?addrbook.php */
#if PRINT
			printf("%s failed with %d\n",url, WSAGetLastError());
#endif
			return 1;
		}
		//send
		std::string sendBuf;
		//printf("\nhere %s here\n", request);
		sendBuf = "GET " + std::string(request) + " HTTP/1.0\r\n"
			+ "User-agent: kavyasree.bvs/1.3\r\n"
			+ "Host: " + std::string(host) + "\r\n"
			+ "Connection: close\r\n" + "\r\n";
		// place request into buf
		//printf("\nsendBuf \n%s\n", sendBuf.c_str());
		if (send(sock, sendBuf.c_str(), strlen(sendBuf.c_str()), 0) == SOCKET_ERROR)
		{
#if PRINT
			printf("%s failed sending with error %d\n",url, WSAGetLastError());
#endif
			return 1;
		}
		else
		{
			//printf("robot socket send successful\n");
		}
#if PRINT
		end = clock();
		cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
		printf("done in %d ms\r\n\t", int(cpu_time_used));
#endif

		//receive
		//Load d page
#if PRINT
		printf("  Loading... ");
#endif
		int iRes;
		//char fileBuf[DEFAULT_BUFLEN];
		char * fileBuf1;
		fileBuf1 = (char *)malloc(DEFAULT_BUFLEN);
		int fileSize1 = DEFAULT_BUFLEN;
		// shutdown the connection since no more data will be sent
		iRes = shutdown(sock, SD_SEND);
		if (iRes == SOCKET_ERROR) {
#if PRINT
			printf("%s shutdown failed with error: %d\n", url,WSAGetLastError());
#endif
			closesocket(sock);
			WSACleanup();
			//return 1;
			return 1;
		}
		int k = 0;
		fd_set sockholder;
		FD_ZERO(&sockholder);
		FD_SET(sock, &sockholder);
		timeval timeout1;
		timeout1.tv_sec = 10;
		timeout1.tv_usec = 0;
#if PRINT
		start = clock();
#endif
		do
		{
			if (k > 2 * 1024 * 1024)
			{
#if PRINT
				printf("%s failed with exceeding max\n", url);
#endif
				WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
				tidUnique = InterlockedExchangeAdd(&(p->noOfBytes), k); ReleaseMutex(p->mutex);
				return 1;
			}
			int rv = select(0, &sockholder, NULL, NULL, &timeout);
			// wait to see if socket has any data (see MSDN) 
			if (rv > 0)
			{
				// new data available; now read the next segment 
				iRes = recv(robot, fileBuf1 + k, (fileSize1 - k), 0);
				if (iRes > 0)
				{
					k = k + iRes;
				}
				else if (iRes == 0)
				{
					//printf("Connection closed\n");
				}
				else
				{
					/*10014 error on recv for http://128.194.135.72*/
#if PRINT
					printf("%s failed with %d on recv\n", url,WSAGetLastError());
#endif
					return 1;
				}
				if (fileSize1 - k < 512)
				{
					fileSize1 = fileSize1 << 1;
					char* buf2 = (char*)realloc(fileBuf1, fileSize1);
					if (buf2 != NULL)
					{
						fileBuf1 = buf2;
					}
					else
					{
						free(fileBuf1);
						break;
					}
				}
			}
			else if (rv == 0)
			{
				//printf("timeout error\n");
#if PRINT
				printf("%s failed with slow download\n",url);
#endif
				return 1;
			}
			else if (rv == SOCKET_ERROR)
			{
#if PRINT
				printf("%s failed with %d on recv\n", url,WSAGetLastError());
#endif
				return 1;
			}
			else if (rv < 0)
			{
#if PRINT
				printf("%s failed with %d on recv\n",url, WSAGetLastError());
#endif
				return 1;
			}
		} while (iRes > 0);
#if PRINT
		end = clock();
		cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
																		//printf("\nfileBuf %s\n", fileBuf);
		printf("done in %d ms with %d bytes\n", int(cpu_time_used), k);
#endif
		WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
		tidUnique = InterlockedExchangeAdd(&(p->noOfBytes), k); ReleaseMutex(p->mutex);
		closesocket(sock);
		//verify header
		// call cleanup when done with everything and ready to exit program
		WSACleanup();
#if PRINT
		printf("\t  Verifying header... ");
#endif
		fileBuf1[k] = '\0';
		//printf("\nfileBuf1 %s\n", fileBuf1);
		char *header_start = strstr(fileBuf1, "HTTP/");
		if (header_start == NULL)
		{
#if PRINT
			printf_s("%s failed with non-HTTP header\n", url);
#endif
			return 1;
		}

		//printf("header_start %s\n", header_start);
		int l = 0;
		char status[4];
		status[0] = header_start[9];
		status[1] = header_start[10];
		status[2] = header_start[11];
		status[3] = '\0';

		int status_code = atoi(status);
#if PRINT
		printf("%s status code %d\n", url,status_code);
#endif
		//parse d page

		WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
		tidUnique = InterlockedIncrement(&(p->C)); 
		if(status_code % 200 <= 99 && status_code / 200==1)
		{
			//printf("\nhere 200\n");
			tidUnique = InterlockedIncrement(&(p->C_two));
		}
		else if (status_code % 300 <= 99 && status_code / 300 == 1)
			tidUnique = InterlockedIncrement(&(p->C_three));
		else if (status_code % 400 <= 99 && status_code / 400 == 1)
			tidUnique = InterlockedIncrement(&(p->C_four));
		else if (status_code % 500 <= 99 && status_code / 500 == 1)
			tidUnique = InterlockedIncrement(&(p->C_five));
		else
			tidUnique = InterlockedIncrement(&(p->C_other));
		ReleaseMutex(p->mutex);
			
#if PRINT
			printf("\t+ Parsing page... ");
#endif
		
		
		const char* tamu_domain = "tamu.edu";
		char* resulting_string;
		resulting_string = strstr(host, tamu_domain);

		bool did_it_originate_outside_tamu = true;

		int len;
		if (resulting_string == NULL)
			len = 0;
		else
			len = strlen(resulting_string);
		if (len == strlen(tamu_domain))
		{
			did_it_originate_outside_tamu = false;
		}
		bool the_page_contains_a_hyperlink_to_tamu = false;
		int reslen;
		//valid http code for pages is 2xx
		if (status_code > 199 && status_code <300)
		{
			char* ret;
			ret = res;
#if PRINT
			start = clock();
#endif
			//char baseUrl[] = url;		// where this page came from; needed for construction of relative links

			int nLinks;
			char *linkBuffer = parser->Parse(fileBuf1, fileSize1, url, (int)strlen(url), &nLinks);

			// check for errors indicated by negative values
			if (nLinks < 0)
				nLinks = 0;

			//printf("Found %d links:\n", nLinks);

			// print each URL; these are NULL-separated C strings
			for (int i = 0; i < nLinks; i++)
			{
				//printf("%s\n", linkBuffer);
				//printf("\nhost of link frm parsing d page is = %s\n",gethostfromurl(linkBuffer));
				//put max length condition

				char host_frm_link[MAX_HOST_LEN];
				if (strlen(gethostfromurl(linkBuffer).c_str()) < MAX_HOST_LEN)
				{

					memcpy(host_frm_link, gethostfromurl(linkBuffer).c_str(), strlen(gethostfromurl(linkBuffer).c_str()));
					host_frm_link[strlen(gethostfromurl(linkBuffer).c_str())] = '\0';

					//printf("\n host_frm_link = %s\n", host_frm_link);
					char* residue_tamu_domain;
					residue_tamu_domain = strstr(host_frm_link, tamu_domain);


					if (residue_tamu_domain == NULL)
						reslen = 0;
					else
						reslen = strlen(residue_tamu_domain);

					if (reslen == strlen(tamu_domain))
					{
						the_page_contains_a_hyperlink_to_tamu = true;
					}
					//printf("here %s\n", residue_tamu_domain);
				}
				linkBuffer += strlen(linkBuffer) + 1;
			}
			WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
			if (the_page_contains_a_hyperlink_to_tamu)
			{
				tidUnique = InterlockedIncrement(&(p->hyperlinkToTamuDomain));
				if (did_it_originate_outside_tamu)
					tidUnique = InterlockedIncrement(&(p->outsideTamu));
			}
			ReleaseMutex(p->mutex);
			//delete fileBuf;
#if PRINT
			end = clock();
			cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
			printf("done in %d ms with %d links\n", int(cpu_time_used), nLinks);
#endif
			WaitForSingleObject(p->mutex, INFINITE);//ReleaseMutex(p->mutex);
			tidUnique = InterlockedExchangeAdd(&(p->L), nLinks); ReleaseMutex(p->mutex);
		}
		else
		{
			//printf("\n");
		}
	}
	else
	{
		//delete parser;		// this internally deletes linkBuffer
		return 0;
	}
	//delete parser;		// this internally deletes linkBuffer
	WSACleanup();
	//printf("\n");
#endif
}
UINT CrawlerRun(LPVOID pParam)
{
	Parameters *p = ((Parameters*)pParam);
	LONG tidUnique;
	WaitForSingleObject(p->mutex, INFINITE);
	tidUnique = InterlockedIncrement(&(p->noOfActiveThreads));
	ReleaseMutex(p->mutex);

	HTMLParserBase *parser = new HTMLParserBase;
	while (true)
	{
		WaitForSingleObject(p->mutex, INFINITE);					// lock mutex
		if (p->Q.size() != 0)
		{
			string x;
			x = p->Q.front();
			p->Q.pop();
			tidUnique = InterlockedIncrement(&(p->E));
			ReleaseMutex(p->mutex);									// release critical section
			int ret = crawlURL(x, p, parser);								//crawl x
			// signal that this thread has finished its job
			//ReleaseSemaphore(p->finished, 1, NULL);
		}
		else
		{
			//ReleaseSemaphore(p->finished, 1, NULL);
			//ReleaseMutex(p->mutex);
			//delete parser;
			//ReleaseSemaphore(p->finished, 1, NULL);
			//tidUnique = InterlockedDecrement(&(p->noOfActiveThreads));
			ReleaseMutex(p->mutex);
			break;
		}
		//ReleaseMutex(p->mutex);
	}

	delete parser;
	WaitForSingleObject(p->mutex, INFINITE);
	tidUnique = InterlockedDecrement(&(p->noOfActiveThreads));
	ReleaseMutex(p->mutex);
	// signal that this thread has finished its job
	ReleaseSemaphore(p->finished, 1, NULL);
	return 0;
}
#include <fstream>
#include <sstream>
void p3(int threads, char* input_file)
{
	// parse command line args
	if (threads > 1)
	{
		//
	}
	else 
	{
		printf("invalid number of threads. Should be > 1 \n");
		return;
	}
	//printf("input file is %s and no of threads is %d\n", input_file, threads);

	// initialize shared data structures & parameters sent to threads
	Parameters p;
	// create a mutex for accessing critical sections (including printf); initial state = not locked
	p.mutex = CreateMutex(NULL, 0, NULL);
	if (p.mutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return;
	}
	// create a semaphore that counts the number of active threads; initial value = 0, max = 2
	p.finished = CreateSemaphore(NULL, 0, threads, NULL);
	// create a quit event; manual reset, initial state = not signaled
	p.eventQuit = CreateEvent(NULL, true, false, NULL);
	// thread handles are stored here; they can be used to check status of threads, or kill them
	p.inputFile = string(input_file);

	//printf("Initialization done\n");

	HANDLE *handles = new HANDLE[threads+1];
	HANDLE stats_handle;
	HANDLE file_handle;
#if 0
	std::ifstream infile(input_file);
	std::string line;
	while (infile >> line)
	{
		printf("\nline is %s\n", line);
	}
#else
	char filename[100];
	memcpy(filename, input_file, strlen(input_file) + 1);
	filename[strlen(input_file) + 1] = '\0';
	FILE * pFile;
	errno_t err = fopen_s(&pFile, filename, "rb");
	if (err != 0)
	{
		char buf[1000];
		strerror_s(buf, sizeof buf, err);
		fprintf_s(stderr, "cannot open file '%s': %s\n", filename, buf);
		fclose(pFile);
		return;
	}
	else
	{
		int size = 0;
		char temp[MAX_URL_LEN+100];
		while (fgets(temp, MAX_URL_LEN+100, pFile) != NULL)
		{
			size = size + strlen(temp);
			//printf("b4url is %s\n", temp);
			int index = temp[strlen(temp) - 1];
			//printf("b4 len %d", strlen(temp));
			if (index == 10)
				temp[strlen(temp) - 2] = '\0';
			//printf(" af len %d \n", strlen(temp));
			//printf("afurl is %s\n", temp);
			/*
			for (int j = 0; j < strlen(temp); j++)
			{
				printf("%d ", temp[j]);
			}
			//printf("\n");
			/**/
			//printf(" af len %d \n", strlen(temp));
			p.Q.push(temp);
		}

		printf("Opened %s with size %d\n", filename, size);
	}
#endif
#if 1
	// start stats thread
	stats_handle= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StatsRun, &p, 0, NULL);		// start threadA (instance #1) 
	if (stats_handle == NULL)
	{
		printf("stats CreateThread error: %d\n", GetLastError());
		return;
	}
	//printf("stats thread created\n");
			
	// start N crawling threads
	/*
	LONG tidUnique;
	WaitForSingleObject(p.mutex, INFINITE);
	tidUnique = InterlockedExchangeAdd(&(p.noOfActiveThreads), threads);
	ReleaseMutex(p.mutex);
	*/
	int i;
	for (i = 0; i < threads; i++)
	{
		handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CrawlerRun, &p, 0, NULL);		// start threadA (instance #1) 
		if (handles[i] == NULL)
		{
			printf("crawling CreateThread error: %d\n", GetLastError());
			return;
		}
	}
	//printf("crawling threads created\n");

	// wait for N crawling threads to finish
	WaitForMultipleObjects(threads, handles, TRUE, INFINITE);
	for (i = 0; i < threads; i++)
		CloseHandle(handles[i]);

	// signal stats thread to quit; wait for it to terminate
#if 0
	WaitForSingleObject(p.mutex, INFINITE);
	SetEvent(p.eventQuit);
	ReleaseMutex(p.mutex);
#endif
	WaitForSingleObject(stats_handle, INFINITE);
	CloseHandle(stats_handle);
#endif
	// cleanup 
	return;

}
int main(int argc, char** argv)
{
	clock_t start, end;
	double cpu_time_used;
	
	if(argc ==2)
	{
		p1(argv[1]);
	}
	else if (argc == 3)
	{
		//start = clock();
		if(atoi(argv[1]) == 1)
			p2(atoi(argv[1]), argv[2]);
		else
			p3(atoi(argv[1]), argv[2]);
		//end = clock();
	}
	else
	{
		//printf("Usage error. Use the format: hw1.exe scheme://host[:port][/path][?query][#fragment]\n");
		printf("incorrect number of arguments\n");
	}
	//printf("\ntime taken = %s\n", end - start);
	//cpu_time_used = 1000 * ((double)(end - start)) / CLOCKS_PER_SEC;//in ms
	//printf("finally done in %d ms\r\n\t", int(cpu_time_used));
    return 0;
}