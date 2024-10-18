#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <vector>

#pragma comment (lib, "WSock32.lib")

int main() {

	WSADATA wsaData;

	//WinSockの初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//初期化できなかったら...
		std::cout << "WinSockの初期化に失敗しました" << std::endl;
		return 1;
	}

	//ポート番号の指定
	unsigned short portNum = 0;
	std::cout << "使用するポート番号を指定してくださいーー＞";
	std::cin >> portNum;

	///ソケットの作成
	SOCKET listenSocket;
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	//失敗したら終了
	if (listenSocket == INVALID_SOCKET) {
		std::cout << "ソケットの作成に失敗しました" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "リスンソケットをオープンしました" << std::endl;
	///

	///bindでソケットに名前を付ける
	//bindに使うSOCKADDR_IN構造体の設定
	SOCKADDR_IN saddr;
	//一旦全部０に
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//bind
	if (bind(listenSocket, reinterpret_cast<SOCKADDR*>(&saddr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		//失敗したら終了
		std::cout << "bindに失敗しました" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << "bindに成功しました" << std::endl;
	///

	///listenで接続の待機状態にする
	//listen
	if (listen(listenSocket, 0) == SOCKET_ERROR) {
		//失敗したら終了
		std::cout << "listenに失敗しました" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << "listenに成功しました" << std::endl;
	///

	///acceptで接続を受け入れる
	SOCKET acceptSocket;
	SOCKADDR_IN from;
	memset(&from, 0, sizeof(SOCKADDR_IN));
	int fromSize;
	fromSize = static_cast<int>(sizeof(from));

	std::cout << "acceptで待機しています" << std::endl;

	acceptSocket = accept(listenSocket, reinterpret_cast<SOCKADDR*>(&from), &fromSize);
	//失敗したら終了
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "acceptに失敗しました" << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	std::cout << inet_ntoa(from.sin_addr) << "が接続してきました" << std::endl;
	std::cout << "acceptに成功しました" << std::endl;

	//listen用ソケットはもう必要ないので解放
	closesocket(listenSocket);
	///

	///会話開始
	std::cout << "会話開始" << std::endl;

	while (true) {

		int nRcv;
		std::vector<char> szBuf(1024);

		nRcv = recv(acceptSocket, szBuf.data(), szBuf.size() - 1, 0);
		szBuf[nRcv] = '\0';
		std::cout << "受信ーー＞" << szBuf.data() << std::endl;

		std::cout << "送信ーー＞";
		std::cin >> szBuf.data();
		send(acceptSocket, szBuf.data(), static_cast<int>(strlen(szBuf.data())), 0);

	}
	///

	//ソケットの解放
	closesocket(acceptSocket);
	//WinSockの終了
	WSACleanup();

	return 0;
}