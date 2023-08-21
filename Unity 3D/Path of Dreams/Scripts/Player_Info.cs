using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Player_Info : LivingEntity
{
    private static Player_Info m_instance; 

    public static Player_Info instance
    {
        get
        {

            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Player_Info>();
            }

            return m_instance;
        }
    }

    public Animator playerAnimator;
    public AudioSource player_info_audio;
    public AudioClip player_hit_sound;
    public AudioClip player_potion_sound;
    public AudioClip player_levelup_sound;
    public AudioClip player_die_sound;
    public AudioClip player_revival_sound;
    public AudioClip player_skill1_sound;
    public AudioClip player_skill2_sound;
    public AudioClip player_skill3_sound;
    public AudioClip player_questok_sound;
    public AudioClip player_questclear_sound;
    public ParticleSystem revival_effect;
    public ParticleSystem skill1_effect;
    public ParticleSystem skill2_effect;
    public ParticleSystem skill3_effect;

    public bool skill1_check;
    public bool skill2_check;
    public bool skill3_check;

    public GameObject hud_damage_txt;
    public Transform hud_pos;

    public Slider healthSlider; // ü�� UI �����̴�
    public Slider expSlider; // ����ġ UI �����̴�

    public string Player_Name;
    public int Player_Level;
    public int Player_Gold;
    public float Player_MaxExp;
    public float Player_Exp;

    public float Player_Max_Health;

    public int Player_Str;
    public int Player_Dex;
    public int Player_Con;

    public float Power;
    public float Defense;
    public float Critical;

    private float revival_cooltime;
    private float revival_timer;

    public GameObject Player_Weapon_Obj;

    public bool player_talkable;
    public bool player_talkstart;
    public int player_woodnpc01_stat;
    public int player_villagenpc02_stat;
    public int player_villagenpc03_stat;
    public int player_quest_mobcount;
    public int player_quest_itemcount;

    public int player_map_index;
    public bool player_scene_move_check;
    public bool player_newgame_ani_trigger; // �����ӽ� �ִϸ��̼� ���� Ʈ����

    public GameObject minimap_cam;

    private void Awake()
    {
        revival_cooltime = 3.5f;
        revival_timer = 0;

        Player_Weapon_Obj.gameObject.SetActive(false);

        player_talkable = false;
        player_talkstart = false;

        player_newgame_ani_trigger = false;

        skill1_check = false;
        skill2_check = false;
        skill3_check = false;
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (dead)
            Revival();

        Player_LevelUp();

        if (player_villagenpc02_stat == 1 || player_villagenpc02_stat == 2)
            Ui_Manager.instance.Quest_Info(0, player_quest_mobcount);
    }

    protected override void OnEnable()
    {
        // new���ӽ�
        // LivingEntity�� OnEnable() ���� (���� �ʱ�ȭ)
        base.OnEnable(); // �θ� �ѹ� ������

        health = 120; // ���� �����ϰ� ����� ü��
        Player_Max_Health = health;
        Player_Level = 1;
        Player_Exp = 0;
        Player_MaxExp = Player_Level * 25;
        Player_Gold = 0;
        Player_Str = 5;
        Player_Dex = 5;
        Player_Con = 5;
        Power = 5 + (Player_Str / 5);
        Defense = 5 + (Player_Con / 5);
        Critical = 1 + (Player_Dex / 10);

        // ������ ��,����,���� 1~5 ������ ���ڷ� �������� ����
        // ������ �Ŀ��� ���潺�� 1~3, ũ��Ƽ�������� 0~1�� �������� ����
        // �Ŀ��� ��5�� 1, ���潺�� ��5�� 1, ũ��Ƽ�������� ����10�� 1 (����������)

        healthSlider.gameObject.SetActive(true);
        healthSlider.maxValue = Player_Max_Health; // ü�½����̴� ����
        healthSlider.value = health;

        expSlider.gameObject.SetActive(true);
        expSlider.maxValue = Player_MaxExp;
        expSlider.value = Player_Exp;

        player_map_index = 1; // ĳ���Ͱ� �ִ� ���ε��� ��ȣ
        player_scene_move_check = false; // ���� �̵��ߴ��� üũ�ϴ� ����

        player_woodnpc01_stat = 0; // ���ǽÿ� ��ȣ�ۿ� ���º���
        player_villagenpc02_stat = 0;
        player_villagenpc03_stat = 0;
        player_quest_mobcount = 0;
        player_quest_itemcount = 0;
    }

    public override void OnDamage(float damage, Vector3 hitPoint, Vector3 hitNormal)
    {
        if (!dead)
        {
            player_info_audio.PlayOneShot(player_hit_sound);
        }

        // LivingEntity�� OnDamage()�� �����Ͽ� ������ ����
        base.OnDamage(damage, hitPoint, hitNormal);
        healthSlider.value = health;
        Ui_Manager.instance.UserHpInfo(Player_Max_Health, health);

        GameObject hudtxt = Instantiate(hud_damage_txt);
        hudtxt.transform.position = hud_pos.position;
        hudtxt.GetComponent<Damage_Show>().show_damage = damage;
    }

    public override void Die()
    {
        base.Die();

        player_info_audio.PlayOneShot(player_die_sound);
        playerAnimator.SetTrigger("Player_Die");
        Ui_Manager.instance.Die_Notice_OnOff();
    }

    public void Revival()
    {
        if (revival_cooltime >= revival_timer)
        {
            revival_timer += Time.deltaTime;
            playerAnimator.ResetTrigger("Player_Revival");
        }
        else
        {
            if (player_map_index == 1)
                this.transform.position = new Vector3(63, 1.398f, 60.39f);
            else if (player_map_index == 2)
                this.transform.position = new Vector3(36.1f, 0, 55.85f);
            else if (player_map_index == 3)
                this.transform.position = new Vector3(-25.7f, 0, 47.31f);
            else if (player_map_index == 4)
            {
                this.transform.position = new Vector3(-25.7f, 0, 47.31f);
                player_map_index = 3;
            }

            dead = false;
            playerAnimator.SetBool("Attack_Check", false);
            playerAnimator.SetBool("Skill_Check", false);
            playerAnimator.ResetTrigger("Player_Die");
            playerAnimator.SetTrigger("Player_Revival");

            health = Player_Max_Health;
            healthSlider.value = health;
            Ui_Manager.instance.UserHpInfo(Player_Max_Health, health);

            player_info_audio.PlayOneShot(player_revival_sound);
            revival_effect.Play();
            Ui_Manager.instance.Die_Notice_OnOff();
            revival_timer = 0;

            Vector3 tmp_pos = minimap_cam.transform.position;

            tmp_pos.x = this.transform.position.x;
            tmp_pos.z = this.transform.position.z;

            minimap_cam.transform.position = tmp_pos;
        }
    }

    public override void RestoreHealth(float newHealth)
    {
        float value = 0;

        value = health + newHealth;

        if (value > Player_Max_Health)
        {
            newHealth -= (value - Player_Max_Health);
        }

        // LivingEntity�� RestoreHealth() ���� (ü�� ����)
        base.RestoreHealth(newHealth);
        healthSlider.value = health; // �����̴��� ����
        Ui_Manager.instance.UserHpInfo(Player_Max_Health, health);
    }

    public void Player_Equip_On(int _Player_Str, int _Player_Dex, int _Player_Con, float _Power, float _Defense, float _Critical)
    {
        Power -= (Player_Str / 5); // ������ �������� ���� ������ �ʱ�ȭ
        Defense -= (Player_Con / 5);
        Critical -= (Player_Dex / 10);

        Player_Str += _Player_Str;
        Player_Dex += _Player_Dex;
        Player_Con += _Player_Con;

        Power += _Power + (Player_Str / 5);
        Defense += _Defense + (Player_Con / 5);
        Critical += _Critical + (Player_Dex / 10);
    }

    public void Player_Equip_Off(int _Player_Str, int _Player_Dex, int _Player_Con, float _Power, float _Defense, float _Critical)
    {
        Power -= (Player_Str / 5); // ������ �������� ���� ������ �ʱ�ȭ
        Defense -= (Player_Con / 5);
        Critical -= (Player_Dex / 10);

        Player_Str -= _Player_Str;
        Player_Dex -= _Player_Dex;
        Player_Con -= _Player_Con;

        Power -= _Power - (Player_Str / 5);
        Defense -= _Defense - (Player_Con / 5);
        Critical -= _Critical - (Player_Dex / 10);
    }

    public void Player_Get_Exp(float _value)
    {
        Player_Exp += _value;

        expSlider.value = Player_Exp;
        Ui_Manager.instance.UserExpInfo(Player_Exp, Player_MaxExp);
    }

    public void Player_Get_Gold(int _value)
    {
        Player_Gold += _value;

        Ui_Manager.instance.UserGoldInfo(Player_Gold);
        Ui_Manager.instance.Gold_Get_Notice(_value);
    }

    public void Player_LevelUp()
    {
        if (Player_MaxExp <= Player_Exp)
        {
            Player_Max_Health += 30;
            health = Player_Max_Health;
            Player_Level += 1;
            Player_Exp = 0;
            Player_MaxExp = Player_Level * 25;

            for (int i = 0; i < 3; i++)
            {
                int tmp_stat = Random.Range(1, 6);

                if (i == 0)
                    Player_Str += tmp_stat;
                else if (i == 1)
                    Player_Dex += tmp_stat;
                else if (i == 2)
                    Player_Con += tmp_stat;
            }

            int tmp_stat2 = Random.Range(1, 4);
            Power += tmp_stat2;
            tmp_stat2 = Random.Range(1, 4);
            Defense += tmp_stat2;
            tmp_stat2 = Random.Range(0, 2);
            Critical += tmp_stat2;

            Ui_Manager.instance.UserLvInfo(Player_Level);
            Ui_Manager.instance.UserDetailInfo(Player_Level, Player_Str, Player_Dex, Player_Con, Power, Defense, Critical);

            healthSlider.maxValue = Player_Max_Health;
            healthSlider.value = health;
            Ui_Manager.instance.UserHpInfo(Player_Max_Health, health);

            expSlider.maxValue = Player_MaxExp;
            expSlider.value = Player_Exp;
            Ui_Manager.instance.UserExpInfo(Player_Exp, Player_MaxExp);

            player_info_audio.PlayOneShot(player_levelup_sound);
            Ui_Manager.instance.LevelUp_Notice_Head();
            Ui_Manager.instance.LevelUp_Notice_Middle();
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        //// player_map_index �� ������ 1,2 �� ���̵��� �ٲ��
        if (other.gameObject.CompareTag("Calis_Village") && player_map_index != 3)
        {
            player_map_index = 3;
            Ui_Manager.instance.showmap_trigger = true;
        }
        else if (other.gameObject.CompareTag("Calis_Village_Wood") && player_map_index != 2)
        {
            player_map_index = 2;
            Ui_Manager.instance.showmap_trigger = true;
        }
        else if (other.gameObject.CompareTag("Muddy_Wood") && player_map_index != 4)
        {
            player_map_index = 4;
            Ui_Manager.instance.showmap_trigger = true;
        }
    }
}
