#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#define DEFAULT_BUFLEN 512
using namespace std;

class Student
{
private:
	string name, id;

public:
	Student(string id, string name)
	{
		this->name = name;
		this->id = id;
	}

	void setId(string id)
	{
		this->id = id;
	}

	void setName(string name)
	{
		this->name = name;
	}

	Student() {}

	void saveToFile()
	{
		ofstream file("student.txt", ios::app);
		if (file.is_open())
		{
			file << id << ',' << name << endl;
			file.close();
		}
	}
};

int main()
{
	// Khoi tao winsock
	cout << "Gui loi chao";
	int iResult = 0;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "\nWINSOCK: Khoi tao that bai\n";
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "\nWINSOCK: Khoi tao thanh cong\n";
	}

	// Khoi tao socket
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Thiet lap dia chi IP va cong svr
	u_short port = 81;
	const char *IP = "127.0.0.1";
	SOCKADDR_IN svrAddr;
	int svrAddrLen = sizeof(svrAddr);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(port);
	svrAddr.sin_addr.s_addr = inet_addr(IP);

	// Gan socket voi dia chi IP va cong svr
	iResult = bind(ListenSocket, (SOCKADDR *)&svrAddr, svrAddrLen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	cout << "ListenSocket: " << ListenSocket << endl;
	cout << "Listening ... " << endl;

	do
	{
		// Nhan du lieu tu cli
		SOCKADDR_IN cliAddr;
		int cliAddrLen = sizeof(cliAddr);
		char recvbuf[DEFAULT_BUFLEN];
		int iSendResult;
		int recvbuflen = DEFAULT_BUFLEN;
		char buffer[1024];
		iResult = recvfrom(ListenSocket, (char *)&buffer, 1024, 0, (SOCKADDR *)&cliAddr, &cliAddrLen);
		if (iResult == SOCKET_ERROR)
		{
			printf("recvfrom failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// Luu thong tin sinh vien vao file txt
		buffer[iResult] = '\0';
		string studentInfo = buffer;
		Student student;
		int pos = studentInfo.find(',');
		student.setId(studentInfo.substr(0, pos));
		studentInfo.erase(0, pos + 1);
		student.setName(studentInfo);
		student.saveToFile();

		// sua thong tin sinh vien
		cout<<"Da luu thong tin sinh vien vao file student.txt";
		cout << "Sua thong tin sinh vien" << endl;
		cout << "Nhap MaSV: ";
		string newId, newName;
		getline(cin, newId);
		cout << "Nhap tenSV: ";
		getline(cin, newName);
		student.setId(newId);
		student.setName(newName);

		// Xoa sinh vien trong file
		ofstream fout("student.txt", ios::trunc); // mở file để ghi, mode trunc sẽ xóa hết nội dung trong file
		fout.close();							   // đóng file

		// luu thong tin vao file sau khi sua
		student.saveToFile();

		// Tra ket qua ve cli
		string result = "Successfully saved student information.";
		iResult = sendto(ListenSocket, (char *)&result, sizeof(result), 0, (SOCKADDR *)&cliAddr, cliAddrLen);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("sendto failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
	} while (true);

	// cleanup
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}
