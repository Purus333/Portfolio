using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.AI;

public class Enemy : LivingEntity
{
    private LivingEntity targetEntity; // ������ ���
    private NavMeshAgent pathFinder; // ��ΰ�� AI ������Ʈ

    public ParticleSystem hitEffect; // �ǰݽ� ����� ��ƼŬ ȿ��
    public Animator enemyAnimator;

    public AudioSource enemyAudio;
    public AudioClip enemy_hit_sound;
    public AudioClip enemy_die_sound;

    public Slider healthSlider;
    public GameObject hud_damage_txt;
    public Transform hud_pos;
    public Vector3 spawn_pos;

    public float damage = 0; // ���ݷ�
    public float attack_cooltime = 2.0f;
    private float last_attack_time = 0;
    public bool attack_check = false; // ������ ���� on, off
    public bool isattack = false; // ���ݹ������� üũ�Ҽ�����
    public bool istarget = false;

    private float dam_timer = 0;
    private float dam_cool_time = 0.3f;
    public bool ondamage_able = true;

    private void Awake()
    {
        pathFinder = GetComponent<NavMeshAgent>();
    }

    void Start()
    {
        StartCoroutine(UpdatePath());
    }

    void Update()
    {
        if (!dead) // ���� ���������
        {
            if (targetEntity != null && !targetEntity.dead) // �÷��̾ ���������
            {
                Move();
                Attack();
            }
            else // �ݴ��ǰ��
            {
                pathFinder.isStopped = true;
                enemyAnimator.SetBool("Enemy_GetTarget", false);
                istarget = false;
                isattack = false;
            }
        }

        if (ondamage_able == false)
            OnDamage_Timer();
    }

    private void Move()
    {
        if (istarget == true && targetEntity != null && !targetEntity.dead && isattack == false)
        {
            pathFinder.isStopped = false;
            enemyAnimator.SetBool("Enemy_GetTarget", true);
            pathFinder.SetDestination(targetEntity.transform.position);
        }
        else if (istarget == true && targetEntity != null && !targetEntity.dead && isattack == true)
        {
            pathFinder.isStopped = true;
            enemyAnimator.SetBool("Enemy_GetTarget", false);
        }
    }

    private IEnumerator UpdatePath()
    {
        while (!dead)
        {
            if (istarget == false && isattack == false)
            {
                pathFinder.isStopped = true;

                Collider[] colliders = Physics.OverlapSphere(transform.position, 5f);

                for (int i = 0; i < colliders.Length; i++)
                {
                    LivingEntity livingEntity = colliders[i].GetComponent<LivingEntity>();

                    if (livingEntity != null && !livingEntity.dead && livingEntity.gameObject.tag == "Player")
                    {
                        targetEntity = livingEntity;
                        istarget = true;
                        break;
                    }
                }
            }
            yield return new WaitForSeconds(0.25f); // �ݺ�
        }
    }

    public void Setup(float newHealth, float newDamage, float newSpeed)
    {
        this.dead = false;
        this.gameObject.SetActive(true);

        pathFinder.enabled = true;
        pathFinder.isStopped = false;

        isattack = false;
        istarget = false;
        attack_check = false;
        ondamage_able = true;
        dam_timer = 0;
        dam_cool_time = 0.3f;
        attack_cooltime = 2.0f;
        last_attack_time = 0;

        StartCoroutine(UpdatePath());

        Collider enemyCollider = GetComponent<Collider>();
        enemyCollider.enabled = true;

        SphereCollider enemy_hitbox = transform.GetChild(18).gameObject.GetComponent<SphereCollider>();
        enemy_hitbox.enabled = true;

        Starting_Health = newHealth;
        health = newHealth;
        damage = newDamage;
        pathFinder.speed = newSpeed;

        healthSlider.gameObject.SetActive(true);
        healthSlider.maxValue = Starting_Health;
        healthSlider.value = health;

        this.gameObject.GetComponent<Rigidbody>().isKinematic = false;
    }

    public override void OnDamage(float damage, Vector3 hitPoint, Vector3 hitNormal)
    {
        if (ondamage_able == false) // �ߺ������� ����
            return;

        if (!dead)
        {
            hitEffect.Play();
            enemyAudio.PlayOneShot(enemy_hit_sound);
        }

        // LivingEntity�� OnDamage()�� �����Ͽ� ������ ����
        base.OnDamage(damage, hitPoint, hitNormal);
        healthSlider.value = health;

        Damage_Show_Pool.instance.Set_Damage(hud_pos, damage, 200);

        ondamage_able = false;
    }

    public void OnDamage_Timer()
    {
        if (dam_cool_time > dam_timer)
        {
            dam_timer += Time.deltaTime;
            return;
        }
        else
        {
            ondamage_able = true;
            dam_timer = 0;
        }
    }

    private void Hide()
    {
        this.gameObject.SetActive(false);
    }

    public override void Die()
    {
        base.Die();

        this.gameObject.GetComponent<Rigidbody>().isKinematic = true;

        Collider enemyCollider = GetComponent<Collider>();
        enemyCollider.enabled = false;

        SphereCollider enemy_hitbox = transform.GetChild(18).gameObject.GetComponent<SphereCollider>();
        enemy_hitbox.enabled = false;

        pathFinder.isStopped = true;

        enemyAnimator.SetTrigger("Enemy_Die");
        enemyAudio.PlayOneShot(enemy_die_sound);
        healthSlider.gameObject.SetActive(false);

        Invoke("Hide", 4f);

        if (this.gameObject.name == "Polygonal Metalon Green(Clone)")
        {
            //�����۰� �� ����ġ�� �÷��̾�� ����
            int num = Random.Range(1, 3);
            if (num == 1)
                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[37], GameManager.instance.item_list[37].item_id);

            Player_Info.instance.Player_Get_Exp(5);

            int gold_value = Random.Range(20, 30);
            Player_Info.instance.Player_Get_Gold(gold_value);
        }
        else if (this.gameObject.name == "Polygonal Metalon Red(Clone)")
        {
            int num = Random.Range(1, 4);
            if (num == 1)
                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[33], GameManager.instance.item_list[33].item_id);

            int num2 = Random.Range(1, 3);
            if (num2 == 1)
                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[38], GameManager.instance.item_list[38].item_id);

            Player_Info.instance.Player_Get_Exp(50);

            int gold_value = Random.Range(250, 301);
            Player_Info.instance.Player_Get_Gold(gold_value);

            if (Player_Info.instance.player_villagenpc02_stat == 1 || Player_Info.instance.player_villagenpc02_stat == 2) // ����Ʈ �������϶�
            {
                Ui_Manager.instance.Quest_Info(0, ++Player_Info.instance.player_quest_mobcount);
                if (Player_Info.instance.player_quest_mobcount >= 3)
                    Player_Info.instance.player_villagenpc02_stat = 2;
            }
        }
    }

    private void Attack()
    {
        if (isattack == true && attack_check == true && enemyAnimator.GetBool("Enemy_Attack") == false)
        {
            enemyAnimator.SetBool("Enemy_Attack", true);
        }
        else if (enemyAnimator.GetCurrentAnimatorStateInfo(0).IsName("Enemy_Stab Attack") &&
            enemyAnimator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f && enemyAnimator.GetBool("Enemy_Attack") == true)
        {
            enemyAnimator.SetBool("Enemy_Attack", false);
            attack_check = false;
        }
    }

    private void Skill_Attack()
    {

    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.CompareTag("Player_HitPoint") && other.gameObject.GetComponentInParent<LivingEntity>().dead == false)
        {
            isattack = true; // ���ݰ����� �����ΰ��� �����ϱ����� ����

            if (this.gameObject.name == "Polygonal Metalon Green(Clone)")
                attack_cooltime = 2.0f;
            else if (this.gameObject.name == "Polygonal Metalon Red(Clone)")
                attack_cooltime = 0.35f;
            else if (this.gameObject.name == "Polygonal Metalon Purple(Clone)")
                attack_cooltime = 0.3f;

            if (!dead && Time.time >= last_attack_time + attack_cooltime)
            {
                // �������κ��� LivingEntity Ÿ���� �������� �õ�
                LivingEntity attack_Target = other.GetComponentInParent<LivingEntity>(); //Player_HitPoint�� �θ�

                // ������ LivingEntity�� �ڽ��� ���� ����̶�� ���� ����
                if (attack_Target != null && attack_Target == targetEntity)
                {
                    // �ֱ� ���� �ð��� ����
                    last_attack_time = Time.time;

                    this.transform.LookAt(targetEntity.transform);

                    attack_check = true; // ������ ���� �¿���
                }
            }
        }
        else if (isattack == true && other.gameObject.CompareTag("Enemy_HitPoint") && attack_check != true)
            isattack = false;
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Player_HitPoint") && other.gameObject.GetComponentInParent<LivingEntity>().dead == false)
        {
            istarget = false;
            isattack = false;
        }
    }
}
