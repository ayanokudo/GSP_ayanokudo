//=================================================================
//
// タッチナンバー処理　サーバー
// Author : AYANO KUDO
//
//=================================================================
#include <stdio.h>
#include <WinSock2.h>
#include <thread>       // マルチスレッド対応
#include "main.h"
#include "tcp_listener.h"
#include "tcp_client.h"

//*****************************************************************
// マクロ定義
//*****************************************************************
#define FILE_NAME "data/test.txt"        // 読み込むファイル名
#define SERVER_PORT_NUM (12345)             // ポート番号

//*****************************************************************
// プロトタイプ宣言
//*****************************************************************
void ReadFile(void);
void WriteFile(void);
void WorkerThread(CTcpClient *pTcpClient);
int SetRanking(int nTime, char *pName);

//*****************************************************************
// グローバル変数宣言
//*****************************************************************
RankingData g_aRankinData[MAX_RANKING];

//=================================================================
// [main] メイン関数
//=================================================================
void main(void)
{
    // 変数宣言
    WSADATA WsaData;

    // winsock初期化
    int nErr = WSAStartup(WINSOCK_VERSION, &WsaData);
    if (nErr)
    {// 初期化に失敗した
        printf("Winsockの初期化に失敗しました\n");

        // キー入力待ち
        rewind(stdin);
        getchar();
        return;
    }

    // グローバル変数初期化
    memset(g_aRankinData,0,sizeof(g_aRankinData));

        // ランキングファイル読み込み
        ReadFile();

        CTcpListener *pTcpListener = CTcpListener::Create(SERVER_PORT_NUM);

        while (1)
        {
            // クライアントの接続を受け付ける
            CTcpClient *pTcpClient = pTcpListener->Accept();

            // チャット用のスレッド生成
            std::thread thChat(WorkerThread, pTcpClient);

            // チャット用スレッドを切り離す
            thChat.detach();

        }

        if (pTcpListener!=NULL)
        {
            pTcpListener->Release();
            pTcpListener = NULL;
        }

        // winsock終了処理
        WSACleanup();
}

//=================================================================
// [ReadFile] ファイルの読み込み
//=================================================================
void ReadFile(void)
{
    FILE *pFile;
    pFile = fopen(FILE_NAME, "r");// 読み込みモード "r" でファイルオープン
    if (pFile!=NULL)
    {
        for (int nCount = 0; nCount < MAX_RANKING; nCount++)
        {
            fscanf(pFile, "%d", &g_aRankinData[nCount].nClearTime);
            //fscanf(pFile, "%s", &g_aRankinData[nCount].aName);

        }
        fclose(pFile);
    }

}

//=================================================================
// [WriteFile] ファイルの書き込み
//=================================================================
void WriteFile(void)
{
    FILE *pFile;
    pFile = fopen(FILE_NAME, "w");// 読み込みモード "w" でファイルオープン
    if (pFile != NULL)
    {
        for (int nCount = 0; nCount < MAX_RANKING; nCount++)
        {
            fprintf(pFile, "%d\n", g_aRankinData[nCount].nClearTime);
            //fprintf(pFile, "%s\n", g_aRankinData[nCount].aName);
        }
        fclose(pFile);
    }
}

//=================================================================
// [WorkerThread] 
//=================================================================
void WorkerThread(CTcpClient *pTcpClient)
{
    // 変数宣言
    char aRecvBuf[256];
    char aSendBuf[256];
    int nTime;//今回のクリアタイム
    char aName[MAX_NAME];

    // 接続を待ち受ける
    // バッファを0クリアする
    // メッセージを受け取る
    pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

    COMMAND_TYPE command = (COMMAND_TYPE)aRecvBuf[0];
     
    // 先頭の1バイトの内容で処理を変える
    switch (command)
    {
        // ランキング一覧取得リクエスト処理
    case COMMAND_TYPE_GET_RANKING:
        // ランキング取得
        ReadFile();
        printf("ランキング送信\n");
        // 人数分ループ
        for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
        {
            // pRankingData[nCntRank]にaRecvBufのデータ設定
            // タイムを保存
            int nData = htonl(g_aRankinData[nCntRank].nClearTime);
            memcpy(&aSendBuf[nCntRank * sizeof(int)], &nData, sizeof(int));
            //// 名前を保存
            //g_aRankinData[nCntRank].aName[MAX_NAME - 1] = '\0';// 名前の最後にNULL文字を入れる
            //memcpy(&aSendBuf[nCntRank * sizeof(RankingData) + sizeof(int)], &g_aRankinData[nCntRank].aName, MAX_NAME);
        }
        pTcpClient->Send(aSendBuf,sizeof(aSendBuf));

        break;

    case COMMAND_TYPE_SET_RANKING:
        // ランキングリクエスト
        //2～5バイト目がら受信したスコアを取得
        int nTime = (int)(*(int*)&aRecvBuf[1]);            //エンディアン変換
        
            //// 6～13バイト目から名前を取得
            //char aName[MAX_NAME];
            //memcpy(aName, &aRecvBuf[5], MAX_NAME);
            //aName[MAX_NAME - 1] = '\0';         // 文字列の最後にNULL文字を入れる

        // ランキングを更新
        int nRank = SetRanking(nTime,aName);// 今回の順位を変数に保存

        // 順位を送信
        aSendBuf[0] = (char)nRank;
            pTcpClient->Send(aSendBuf, sizeof(char));

        break;

    }
 }

//=================================================================
// [SetRanking] 
//=================================================================
int SetRanking(int nTime, char *pName)
{
    int nCount;
    for ( nCount = 0; nCount < MAX_RANKING; nCount++)
    {
        // ランキングを更新する場所の判定
        if (nTime>g_aRankinData[nCount].nClearTime||
            g_aRankinData[nCount].nClearTime == 0)
        {
            //以降のランキングデータを後ろに保存
            for (int nCntMove = MAX_RANKING-1; nCntMove > nCount ; nCntMove--)
            {
                g_aRankinData[nCntMove].nClearTime = g_aRankinData[nCntMove - 1].nClearTime;

            }
            g_aRankinData[nCount].nClearTime = nTime;
            //strcpy(g_aRankinData[nCount].aName, pName);
            break;
        }
    }
        WriteFile();
        return nCount + 1;
}