//=============================================================================
//
// シールドの処理 [Shield.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "shield.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CShield::m_pTexture = NULL;
char *CShield::m_pTextureName = "data/TEXTURE/effect002.png";

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define  MAX_DURABLEVALUE (3)       // 耐久値の最大値
#define SHIELD_SIZE_X (80.0f)       // シールドのサイズ
#define SHIELD_SIZE_Y (80.0f)       // シールドのサイズ

//=============================================================================
// [CShield] コンストラクタ
//=============================================================================
CShield::CShield()
{
    m_nDurableValue=MAX_DURABLEVALUE;
    SetObjtype(OBJTYPE_SHIELD);
}


//=============================================================================
// [~CShield] デストラクタ
//=============================================================================
CShield::~CShield()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CShield * CShield::Create(D3DXVECTOR3 pos, CPlayer* pPlayer)
{
    CShield *pShield = NULL;
    if (!pShield)
    {
        pShield = new CShield;
        pShield->Init(pos, pPlayer);
    }
    return pShield;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CShield::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CShield::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CShield::Init(D3DXVECTOR3 pos, CPlayer* pPlayer)
{
    // 各メンバ変数初期化
    CScene2D::Init();
    SetPosition(pos);
    SetSize(D3DXVECTOR3(SHIELD_SIZE_X, SHIELD_SIZE_Y,0.0f));
    SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
    m_pPlayer = pPlayer;
    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CShield::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CShield::Update(void)
{
    // プレイヤーの位置に追従させる
    CScene2D::Update();
    D3DXVECTOR3 pos = GetPosition();
    CPlayer *pPlayer = CGame::GetPlayer();

    pos = pPlayer->GetPosition();
    SetPosition(pos);
    // 敵との当たり判定
    CScene* pScene = JudgeFittingRectangle(CScene::OBJTYPE_ENEMY);
    if (pScene!=NULL)
    {
        ((CEnemy*)pScene)->Damage();
        pScene = NULL;
        Damage();
        pPlayer->SetShield(false);
    }
    // 地形との当たり判定
    pScene = JudgeFittingRectangle(CScene::OBJTYPE_TERRAIN);
    if (pScene)
    {
        Damage();
        // プレイヤーを一定時間無敵状態にする
        pPlayer->SetShield(false);
    }

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CShield::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}

//=============================================================================
// [Damage] ダメージ
//=============================================================================
void CShield::Damage(void)
{
    m_nDurableValue--;


    if (m_nDurableValue <= MAX_DURABLEVALUE / 2.0f)
    {
        SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
    }
    // シールドの耐久値が0以下になった時
    if (m_nDurableValue <= 0)
    {
        m_pPlayer->SetShieldObjct(NULL);
        Uninit();
    }
}


