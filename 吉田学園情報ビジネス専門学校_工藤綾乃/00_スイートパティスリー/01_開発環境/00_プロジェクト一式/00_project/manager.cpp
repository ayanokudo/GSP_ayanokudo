//=============================================================================
//
// マネージャー [manager.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"
#include "result.h"
#include "fade.h"

//*****************************************************************************
// 静的メンバ変数の初期化
//*****************************************************************************
CManager::MODE    CManager::m_mode            = MODE_NONE;
CRenderer        *CManager::m_pRenderer       = NULL;
CInputKeyboard   *CManager::m_pInputKeyboard  = NULL;
CInputController *CManager::m_InputController = NULL;
CSound           *CManager::m_pSound          = NULL;
CTitle           *CManager::m_pTitle          = NULL;       // タイトル画面のポインタ
CTutorial        *CManager::m_pTutorial       = NULL;       // チュートリアル画面のポインタ
CGame            *CManager::m_pGame           = NULL;
CRanking         *CManager::m_pRanking        = NULL;
CResult          *CManager::m_pResult         = NULL;
bool              CManager::bIsClearedGame;                 // ゲームクリアしたかどうか

//=============================================================================
// [CManager] コンストラクタ
//=============================================================================
CManager::CManager()
{
    m_mode = MODE_NONE;
    bIsClearedGame = false;
}

//=============================================================================
// [~CManager] デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// [Init] 初期化処理
// hInstance : キーボードの入力に必用
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
    if (m_pInputKeyboard == NULL)
    {
        // メモリの確保
        m_pInputKeyboard = new CInputKeyboard;

        if (m_pInputKeyboard != NULL)
        {
            m_pInputKeyboard->Init(hInstance, hWnd);
        }
    }
    if (m_InputController == NULL)
    {
        // メモリの確保
        m_InputController = new CInputController;

        if (m_InputController != NULL)
        {
            m_InputController->Init(hInstance, hWnd);
        }
    }

    // メモリの確保
    m_pRenderer = new CRenderer;
    // 初期化処理
    m_pRenderer->Init(hWnd, bWindow);

    if (m_pSound == NULL)
    {
        m_pSound = new CSound;
    // サウンドの初期化
    m_pSound->Init(hWnd);
    }

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CManager::Uninit(void)
{
    // コントローラの終了処理
    if (m_InputController)
    {
        m_InputController->Uninit();
        delete m_InputController;
        m_InputController = NULL;
    }

    // キーボードの終了処理
    if (m_pInputKeyboard)
    {
        m_pInputKeyboard->Uninit();
        delete m_pInputKeyboard;
        m_pInputKeyboard = NULL;
    }

    // 各モードの終了処理
    switch (m_mode)
    {

    case MODE_TITLE:
        if (m_pTitle)
        {
            m_pTitle->Uninit();
            delete m_pTitle;
            m_pTitle = NULL;
        }
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Uninit();
        delete m_pTutorial;
        m_pTutorial = NULL;
        break;

    case MODE_GAME:
        if (m_pGame)
        {
            m_pGame->Uninit();
            delete m_pGame;
            m_pGame = NULL;
        }
        break;

    case MODE_RESULT:
        if (m_pResult)
        {
            m_pResult->Uninit();
            delete m_pResult;
            m_pResult = NULL;
        }
        break;

    case MODE_RANKING:
        if (m_pRanking)
        {
        m_pRanking->Uninit();
        delete m_pRanking;
            m_pRanking = NULL;
        }
        break;
    }

    if (m_pRenderer)
    {
        // 終了処理
        m_pRenderer->Uninit();

        // メモりを開放してポインタの中身をNULLにする
        delete m_pRenderer;
        m_pRenderer = NULL;
    }

    //サウンドの終了処理
    if (m_pSound)
    {
        m_pSound->Uninit();
        // メモりを開放してポインタの中身をNULLにする
        delete m_pSound;
        m_pSound = NULL;
    }
}

//=============================================================================
// [Updete] 更新処理
//=============================================================================
void CManager::Update(void)
{
    if (CGame::GetPause() == false)
    {
        if (m_pRenderer != NULL)
        {
            // 更新処理
            m_pRenderer->Update();
        }
    }

    // 現在のモードの更新処理を行う
    switch (m_mode)
    {

    case MODE_TITLE:    // タイトル
        m_pTitle->Update();
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Update();
        break;

    case MODE_GAME:     // ゲーム
        m_pGame->Update();
        break;

    case MODE_RESULT:   // リザルト
        m_pResult->Update();
        break;

    case MODE_RANKING:  // ランキング
        m_pRanking->Update();
        break;
    }

    m_pInputKeyboard->Update();
    m_InputController->Update();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CManager::Draw(void)
{

    if (m_pRenderer != NULL)
    {
        // オブジェクトの描画処理
        m_pRenderer->Draw();
    }


}

//=============================================================================
// [SetMode] モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
    // 現在のモードを終了
    switch (m_mode)
    {
    case MODE_TITLE:       // タイトル
        if (m_pTitle)
        {
            m_pTitle->Uninit();
            delete m_pTitle;
            m_pTitle = NULL;
        }
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Uninit();
        delete m_pTutorial;
        m_pTutorial = NULL;
        break;

    case MODE_GAME:     // ゲーム
        if (m_pGame != NULL)
        {
            m_pGame->Uninit();
            delete m_pGame;
            m_pGame = NULL;
        }
        break;

    case MODE_RESULT:   // リザルト
        if (m_pResult)
        {
            m_pResult->Uninit();
            delete m_pResult;
            m_pResult = NULL;
        }
        break;

    case MODE_RANKING:  // ランキング
        if (m_pRanking != NULL)
        {
            m_pRanking->Uninit();
            delete m_pRanking;
            m_pRanking = NULL;
        }
        break;
    }

    m_mode = mode;

    // 次のモードを初期化
    switch (m_mode)
    {
    case MODE_TITLE:    // タイトル
        m_pTitle = CTitle::Create();
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial = CTutorial::Create();
        break;

    case MODE_GAME:     // ゲーム
        m_pGame = CGame::Create();
        break;

    case MODE_RESULT:   // リザルト
        m_pResult = CResult::Create(bIsClearedGame);
        break;

    case MODE_RANKING:  // ランキング
        m_pRanking = CRanking::Create();
        break;
    }
}