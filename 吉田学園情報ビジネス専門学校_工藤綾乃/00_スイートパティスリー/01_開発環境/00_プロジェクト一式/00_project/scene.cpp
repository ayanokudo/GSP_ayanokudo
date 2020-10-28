//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "scene.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
CScene* CScene::m_apScene[MAX_POLYGON] = {};
int CScene::m_nNumAll = 0;

//=============================================================================
// [CScene] コンストラクタ
//=============================================================================
CScene::CScene()
{
    for (int nNumPolygon = 0; nNumPolygon < MAX_POLYGON; nNumPolygon++)
    {
        if (m_apScene[nNumPolygon] == NULL)
        {
            m_apScene[nNumPolygon] = this;

            m_nIndex = nNumPolygon;//番号を保存

            m_nNumAll++; // 総数を増やす

            break;// 1体ずつ設定するためにループ抜ける
        }
    }
}

//=============================================================================
// [CScene] デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// [UpdateAll] すべてのオブジェクトを更新
//=============================================================================
void CScene::UpdateAll(void)
{
    for (int nNumPolygon = 0; nNumPolygon < MAX_POLYGON; nNumPolygon++)
    {
        if (m_apScene[nNumPolygon] != NULL)
        {
            m_apScene[nNumPolygon]->Update();
        }
    }
}

//=============================================================================
// [DrawAll] すべてのオブジェクトを描画
//=============================================================================
void CScene::DrawAll(void)
{
    for (int nCountType = 0; nCountType < OBJTYPE_MAX; nCountType++)
    {
        for (int nNumPolygon = 0; nNumPolygon < MAX_POLYGON; nNumPolygon++)
        {
            if (m_apScene[nNumPolygon] != NULL)
            {
                if (m_apScene[nNumPolygon]->m_objtype == nCountType)
                {
                    m_apScene[nNumPolygon]->Draw();
                }

            }
        }
    }
}

//=============================================================================
// [Release] メモリの開放
//=============================================================================
void CScene::Release(void)
{
    if (m_apScene[m_nIndex] != NULL)
    {
        int nID = m_nIndex;// delete後にm_nIndexが使えなくなるので番号を保存

        // メモリを開放してポインタの中身をNULLにする
        delete m_apScene[m_nIndex];
        m_apScene[nID] = NULL;

    }
}

//=============================================================================
// [ReleaseAll] すべてのメモリを解放
//=============================================================================
void CScene::ReleaseAll(void)
{
    for (int nNumPolygon = 0; nNumPolygon < MAX_POLYGON; nNumPolygon++)
    {
        if (m_apScene[nNumPolygon] != NULL)
        {
            m_apScene[nNumPolygon]->Release();
        }
    }
}

