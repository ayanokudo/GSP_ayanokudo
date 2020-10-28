//=============================================================================
//
// 星の処理 [star.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "star.h"
#include "scene2d.h"
#include "item.h"

//=============================================================================
// [CStar] デフォルトコンストラクタ
//=============================================================================
CStar::CStar()
{
    memset(m_pTriangle, NULL, sizeof(m_pTriangle));
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
    SetObjtype(OBJTYPE_STAR);
}

//=============================================================================
// [CStar] コンストラクタ
// 引数
// pos  : 位置
// move : 移動量
//=============================================================================
CStar::CStar(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
    memset(m_pTriangle, NULL, sizeof(m_pTriangle));
    m_pos = pos;
    m_move = move;
    m_size = D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
    IsItemAppeared = false;
    SetObjtype(OBJTYPE_STAR);
}

//=============================================================================
// [~CStar] デストラクタ
//=============================================================================
CStar::~CStar()
{

}

//=============================================================================
// [Create] 星のオブジェクト生成
//=============================================================================
CStar * CStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
    CStar *pStar = NULL;

    if (pStar == NULL)
    {
        pStar = new CStar(pos, move);
        pStar->Init();
    }
    return pStar;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CStar::Init(void)
{
    for (int nCount = 0; nCount < MAX_TRIANGLE; nCount++)
    {
        m_pTriangle[nCount] = CTriangle::Create(m_pos);
        SetObjtype(OBJTYPE_STAR);
        m_pTriangle[nCount]->SetPosition(m_pos);
    }
    // 三角形の向きを互い違いにする
    m_pTriangle[1]->SetRadian(1.0f);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CStar::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_TRIANGLE; nCount++)
    {
        if (m_pTriangle[nCount])
        {
            m_pTriangle[nCount]->Uninit();
            m_pTriangle[nCount] = NULL;
        }
    }

    Release();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CStar::Update(void)
{
    m_pos += m_move;
    for (int nCount = 0; nCount < MAX_TRIANGLE; nCount++)
    {
        m_pTriangle[nCount]->Rotation(0.01f);
        m_pTriangle[nCount]->SetPosition(m_pos);
    }

    // ショットとの当たり判定
    CScene* pScene = JudgeFittingRectangle(CScene::OBJTYPE_BULLET);
    if (pScene != NULL)
    {
        if (IsItemAppeared == false)
        {
            for (int nCount = 0; nCount < MAX_TRIANGLE; nCount++)
            {
                m_pTriangle[nCount]->SetColor(D3DXCOLOR(1.0f,0.5f,0.5f,0.5f));
            }
            
            // エネルギー回復アイテム生成
            CItem::Create(m_pos, D3DXVECTOR3(-2.0f, 0.0f, 0.0f),CItem::TYPE_001,false);
            IsItemAppeared = true;
        }
    }

    // 画面外に出たら削除
    if (m_pos.x <= 0.0f - m_size.x ||
        m_pos.x >= 3000 ||
        m_pos.y <= 0.0f - m_size.y ||
        m_pos.y >= SCREEN_HEIGHT + m_size.y
        )
    {
        Uninit();
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CStar::Draw(void)
{
    for (int nCount = 0; nCount < MAX_TRIANGLE; nCount++)
    {
        m_pTriangle[nCount]->Draw();
    }
}

//=============================================================================
// [JudgeFittingRectangle] 矩形の当たり判定
// 返り値 : 当たっているオブジェクトのポインタ
//=============================================================================
CScene * CStar::JudgeFittingRectangle(CScene::OBJTYPE type)
{
    // 変数宣言
    CScene *pScene = NULL;

    // オブジェクトの数分ループ
    for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
    {
        pScene = GetSceneObject(nCount);
        if (pScene != NULL)
        {
            if (pScene->GetObjectType() == type)
            {
                D3DXVECTOR3 objPos = ((CScene2D*)pScene)->GetPosition();
                D3DXVECTOR3 objSize = ((CScene2D*)pScene)->GetSize();

                // 当たっていたら
                if (m_pos.x - m_size.x / 2 <= objPos.x + objSize.x / 2 &&
                    m_pos.x + m_size.x / 2 >= objPos.x - objSize.x / 2 &&
                    m_pos.y - m_size.y / 2 <= objPos.y + objSize.y / 2 &&
                    m_pos.y + m_size.y / 2 >= objPos.y - objSize.y / 2)
                {
                    return pScene;  // 当たっていたらオブジェクトのアドレスを返す
                }
            }
        }
    }
    return pScene;// 当たっていないとNULLを返す
}

//=============================================================================
// [JudgeFittingRectangle] 円の当たり判定
// 返り値 : 当たっているオブジェクトのポインタ
//=============================================================================
CScene * CStar::CheckCircleCollision(CScene::OBJTYPE type)
{
    return nullptr;
    // 変数宣言
    CScene *pScene = NULL;

    // オブジェクトの数分ループ
    for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
    {
        pScene = GetSceneObject(nCount);
        if (pScene != NULL)
        {
            if (pScene->GetObjectType() == type)
            {
                D3DXVECTOR3 objPos = ((CScene2D*)pScene)->GetPosition();
                D3DXVECTOR3 objSize = ((CScene2D*)pScene)->GetSize();

                // 当たっていたら
                float distance = powf(m_pos.x - objPos.x, 2.0f) + powf(m_pos.y - objPos.y, 2.0f) + 0.0f;      //位置
                float radius = powf(m_size.x + objSize.x, 2.0f); //半径
                if (distance <= radius)//当たっているかどうか
                {
                    return pScene;  // 当たっていたらオブジェクトのアドレスを返す
                }
            }
        }
    }
    return pScene;// 当たっていないとNULLを返す
}
