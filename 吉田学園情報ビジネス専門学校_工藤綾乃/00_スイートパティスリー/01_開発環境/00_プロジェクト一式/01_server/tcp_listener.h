//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <WinSock2.h>
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
// こういうクラスがあるよという定義
// 再コンパイルしなくてよくなる
class CTcpClient;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
//TCP接続受付クラス
class CTcpListener
{
public:
    CTcpListener();
    ~CTcpListener();

    static CTcpListener* Create(int nPortNum);
    void Release(void);

    CTcpClient* Accept(void);

private:
    bool Init(int nPortNum, int nAcceptConnNum);
    void Close(void);

    SOCKET m_sockServer;// 接続待ち受けソケット

};

#endif // !_TCP_SERVER_H_