using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

// ����ü�μ� ����
// ü��, ������ �޾Ƶ��̱�, ��� ���, ��� �̺�Ʈ

public class LivingEntity : MonoBehaviour, IDamageable
{
    public float Starting_Health = 100f; // ���� ü��
    public float health; // ���� ü��
    public bool dead { get; protected set; } // ��� ����
    public event Action onDeath; // ����� �ߵ��� �̺�Ʈ

    // Ȱ��ȭ�ɶ� ���¸� ����
    protected virtual void OnEnable()
    {
        dead = false;

        health = Starting_Health;
    }

    // �������� �Դ� ���
    public virtual void OnDamage(float damage, Vector3 hitPoint, Vector3 hitNormal)
    {
        health -= damage;

        if (health <= 0 && !dead)
        {
            Die();
        }
    }

    // ü���� ȸ���ϴ� ���
    public virtual void RestoreHealth(float newHealth)
    {
        if (dead)
        {
            return;
        }

        health += newHealth;
    }

    // ��� ó��
    public virtual void Die()
    {
        // onDeath �̺�Ʈ�� ��ϵ� �Լ��� �ִٸ� ����
        if (onDeath != null)
        {
            onDeath();
        }

        dead = true;
    }
}