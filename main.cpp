#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <vector>

#pragma comment (lib, "WSock32.lib")

int main() {

	WSADATA wsaData;

	//WinSock�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//�������ł��Ȃ�������...
		std::cout << "WinSock�̏������Ɏ��s���܂���" << std::endl;
		return 1;
	}

	//�|�[�g�ԍ��̎w��
	unsigned short portNum = 0;
	std::cout << "�g�p����|�[�g�ԍ����w�肵�Ă��������[�[��";
	std::cin >> portNum;

	///�\�P�b�g�̍쐬
	SOCKET listenSocket;
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	//���s������I��
	if (listenSocket == INVALID_SOCKET) {
		std::cout << "�\�P�b�g�̍쐬�Ɏ��s���܂���" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "���X���\�P�b�g���I�[�v�����܂���" << std::endl;
	///

	///bind�Ń\�P�b�g�ɖ��O��t����
	//bind�Ɏg��SOCKADDR_IN�\���̂̐ݒ�
	SOCKADDR_IN saddr;
	//��U�S���O��
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//bind
	if (bind(listenSocket, reinterpret_cast<SOCKADDR*>(&saddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		//���s������I��
		std::cout << "bind�Ɏ��s���܂���" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << "bind�ɐ������܂���" << std::endl;
	///

	///listen�Őڑ��̑ҋ@��Ԃɂ���
	//listen
	if (listen(listenSocket, 0) == SOCKET_ERROR) {
		//���s������I��
		std::cout << "listen�Ɏ��s���܂���" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << "listen�ɐ������܂���" << std::endl;
	///

	///accept�Őڑ����󂯓����
	SOCKET acceptSocket;
	SOCKADDR_IN from;
	memset(&from, 0, sizeof(SOCKADDR_IN));
	int fromSize;
	fromSize = static_cast<int>(sizeof(from));

	std::cout << "accept�őҋ@���Ă��܂�" << std::endl;

	acceptSocket = accept(listenSocket, reinterpret_cast<SOCKADDR*>(&from), &fromSize);
	//���s������I��
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept�Ɏ��s���܂���" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << inet_ntoa(from.sin_addr) << "���ڑ����Ă��܂���" << std::endl;
	std::cout << "accept�ɐ������܂���" << std::endl;

	//listen�p�\�P�b�g�͂����K�v�Ȃ��̂ŉ��
	closesocket(listenSocket);
	///

	///��b�J�n
	std::cout << "��b�J�n" << std::endl;

	while (true) {

		int nRcv;
		std::vector<char> szBuf(1024);

		nRcv = recv(acceptSocket, szBuf.data(), szBuf.size() - 1, 0);
		szBuf[nRcv] = '\0';
		std::cout << "��M�[�[��" << szBuf.data() << std::endl;

		std::cout << "���M�[�[��";
		std::cin >> szBuf.data();
		send(acceptSocket, szBuf.data(), static_cast<int>(strlen(szBuf.data())), 0);

	}
	///

	//�\�P�b�g�̉��
	closesocket(acceptSocket);
	//WinSock�̏I��
	WSACleanup();

	return 0;
}