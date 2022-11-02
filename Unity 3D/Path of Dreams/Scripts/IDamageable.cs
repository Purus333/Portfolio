using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// �׸Ŵ���, ���������̺�, ��������Ƽ �����ӿ� �°� ���� �G �߰��ϱ�*****
// public event Action onDeath; ���캸�� << c++�� �ݹ鰰�� ���

// �������� ���� �� �ִ� Ÿ�Ե��� ���������� ������ �ϴ� �������̽�
public interface IDamageable
{
    // �������� ���� �� �ִ� Ÿ�Ե��� IDamageable�� ����ϰ� OnDamage �޼��带 �ݵ�� �����ؾ� �Ѵ�
    // OnDamage �޼���� �Է����� ������ ũ��(damage), ���� ����(hitPoint), ���� ǥ���� ����(hitNormal)�� �޴´�
    void OnDamage(float damage, Vector3 hitPoint, Vector3 hitNormal);
}