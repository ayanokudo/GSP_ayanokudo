//=============================================================================
//
// ボスの周りの敵の処理 [enemy_type3.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ENEMY_TYPE_3_H_
#define _ENEMY_TYPE_3_H_
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy_Type3 : public CEnemy
{
public:
    CEnemy_Type3();
    ~CEnemy_Type3();

    static CEnemy_Type3* Create(D3DXVECTOR3 pos, float fAngle);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, float fAngle);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void ShootBullet(void);

    // 角度のセッタ/ゲッタ
    void SetAngle(float fAngle) { m_fAngle = fAngle; }
    float GetAngle(void) { return m_fAngle; }

    void Rotation(D3DXVECTOR3 pos, float fLength, float fRot);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
    static char* m_pTextureName;          // テクスチャのファイル名

    float m_fAngle;                                 // 角度
};
#endif // !_ENEMY_TYPE_3_H_
