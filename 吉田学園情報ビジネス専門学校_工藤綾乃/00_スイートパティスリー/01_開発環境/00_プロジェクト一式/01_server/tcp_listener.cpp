//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.cpp]
// Author : 
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "tcp_client.h"
#include "tcp_listener.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ACCEPT_CONN    (100)			// 接続受付最大数

//=============================================================================
// CTcpListener生成
//=============================================================================
CTcpListener* CTcpListener::Create(int nPortNum)
{
    // TCP接続受付クラスのインスタンス生成
    CTcpListener *pListener = new CTcpListener;

    // 静的メンバ関数内で関数を呼び出すときはオブジェクト名も書く
    pListener->Init(nPortNum, MAX_ACCEPT_CONN);

    return pListener;
}

//=============================================================================
// CTcpListenerコンストラクタ
//=============================================================================
CTcpListener::CTcpListener()
{

}

//=============================================================================
// CTcpListenerデストラクタ
//=============================================================================
CTcpListener::~CTcpListener()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
bool CTcpListener::Init(int nPortNum, int nAcceptConnNum)
{
    struct  sockaddr_in addr;        // 接続先の設定

                                     // 接続受付用ソケットの生成 (socket)
    m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

    // 接続受付情報の設定
    addr.sin_family = AF_INET;// IPv4
    addr.sin_port = htons(nPortNum);// ()内はポート番号 任意の数字
    addr.sin_addr.S_un.S_addr = INADDR_ANY; // すべての接続を受け付ける(特定のIPアドレスのみ接続を受け付けるようにすることも可能)

                                            // ソケットにIPアドレスとポート番号を設定する (bind)
    bind(m_sockServer, (struct sockaddr*) &addr, sizeof(addr));

    // 接続要求受付を開始する (listen)
    listen(m_sockServer, nAcceptConnNum);

    return true;
}

//=============================================================================
// 接続受付処理
//=============================================================================
CTcpClient* CTcpListener::Accept(void)
{
    if (m_sockServer < 0)
    {
        return NULL;
    }

    // 変数宣言
    struct  sockaddr_in client;      // クライアントの情報が入る
    int nLenght = sizeof(client);

    // 接続受付処理
    // ソケット接続を受け付ける(受け付けるまでプログラムはブロックされる)
    //accept関数で返されるソケット(アクセプト後はこちらを使う)
    SOCKET sock = accept(m_sockServer, (struct sockaddr*)&client, &nLenght);
    CTcpClient *pTcpClint = CTcpClient::Create(sock);


    return pTcpClint;
}

//=============================================================================
// 破棄処理
//=============================================================================
void CTcpListener::Release(void)
{
    // 切断処理
    Close();

    // インスタンス削除
    delete this;
}

//=============================================================================
// クローズ処理
//=============================================================================
void CTcpListener::Close(void)
{
    if (m_sockServer < 0)
    {
        return;
    }

    // ソケット接続をクローズ
    closesocket(m_sockServer);

    m_sockServer = 1;
}
