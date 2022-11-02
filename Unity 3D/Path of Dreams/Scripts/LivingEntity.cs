using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

// 생명체로서 뼈대
// 체력, 데미지 받아들이기, 사망 기능, 사망 이벤트

public class LivingEntity : MonoBehaviour, IDamageable
{
    public float Starting_Health = 100f; // 시작 체력
    public float health; // 현재 체력
    public bool dead { get; protected set; } // 사망 상태
    public event Action onDeath; // 사망시 발동할 이벤트

    // 활성화될때 상태를 리셋
    protected virtual void OnEnable()
    {
        dead = false;

        health = Starting_Health;
    }

    // 데미지를 입는 기능
    public virtual void OnDamage(float damage, Vector3 hitPoint, Vector3 hitNormal)
    {
        health -= damage;

        if (health <= 0 && !dead)
        {
            Die();
        }
    }

    // 체력을 회복하는 기능
    public virtual void RestoreHealth(float newHealth)
    {
        if (dead)
        {
            return;
        }

        health += newHealth;
    }

    // 사망 처리
    public virtual void Die()
    {
        // onDeath 이벤트에 등록된 함수가 있다면 실행
        if (onDeath != null)
        {
            onDeath();
        }

        dead = true;
    }
}