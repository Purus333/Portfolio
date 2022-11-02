using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Movement : MonoBehaviour
{
    private static Player_Movement m_instance;

    public static Player_Movement instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Player_Movement>();
            }
            return m_instance;
        }
    }

    public float moveSpeed = 0.0f;
    private int walk_status = 0;
    public int active_skill_num = 0;
    public bool buff_onoff = false;
    private float buff_timer = 0;
    private float buff_remain_time = 8;

    public Player_Input player_Input;
    private Rigidbody player_Rigidbody;
    public Animator player_Animator;
    public AudioSource player_audio;
    public AudioClip player_attack_sound;

    public GameObject minimap_cam;

    // Start is called before the first frame update
    void Start()
    {
        player_Input = GetComponent<Player_Input>();
        player_Rigidbody = GetComponent<Rigidbody>();
        player_Animator = GetComponent<Animator>();
        // 사용할 컴포넌트들의 참조 불러오기

        ChangeSpeed();
    }

    // 물리 갱신 주기에 맞춰 실행된다
    private void FixedUpdate()
    {
        if (Player_Info.instance.dead == false)
        {
            if (GameManager.instance.newgame_started == true && Player_Info.instance.player_scene_move_check == false) // 노말한경우
            {
                Move();
                player_Animator.SetFloat("Move", player_Input.move_animotion);
                Attack();
                Skill();
                Buff();
            }
            else if (GameManager.instance.newgame_started == true && Player_Info.instance.player_scene_move_check == true) // 맵이동했을경우
            {
                player_Input.ismove = false;
                player_Input.move_animotion = 0;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        // 플레이 시작시 캐릭터이름 입력창이 뜨면 그때 캐릭터가 일어나게한다 (일회성)
        if (Player_Info.instance.player_newgame_ani_trigger == true && player_Animator.GetBool("NewGameStart") == false)
            player_Animator.SetTrigger("NewGameStart");

        SetWalkRun();

        if (Player_Info.instance.dead == true)
        {
            player_Input.ismove = false;
            player_Input.move_animotion = 0;
            player_Input.isattack = false;
            player_Input.skill_on = false;
        }
    }

    private void Move()
    {
        if (player_Input.ismove == true)
        {
            if (Vector3.Distance(player_Input.click_pos, transform.position) <= 0.1f || player_Input.attak_on == true || player_Input.skill_on == true)
            {
                player_Input.ismove = false;
                player_Input.move_animotion = 0;
                return;
            }

            Vector3 moveDistance = player_Input.click_pos - transform.position;
            var dir = player_Input.click_pos - transform.position; //방향

            Quaternion rotation = Quaternion.LookRotation(dir);

            player_Rigidbody.MovePosition(transform.position += moveDistance.normalized * Time.deltaTime * moveSpeed);
            player_Rigidbody.rotation = Quaternion.Slerp(player_Rigidbody.rotation, rotation, 6.0f * Time.deltaTime); // 구면선형보간 사용

            if (walk_status == 0)
                player_Input.move_animotion = 0.5f;
            else if (walk_status == 1)
                player_Input.move_animotion = 1.0f;

            Vector3 tmp_pos = minimap_cam.transform.position;

            tmp_pos.x = transform.position.x;
            tmp_pos.z = transform.position.z;

            minimap_cam.transform.position = tmp_pos;
        }
    }

    private void Attack()
    {
        if (player_Input.isattack == true)
        {
            if (player_Animator.GetBool("Attack_Check") == false)
                player_Animator.SetBool("Attack_Check", true);
            else if (player_Animator.GetBool("Attack_Check") == true)
            { 
                var dir = player_Input.click_pos - transform.position; //방향
                Quaternion rotation = Quaternion.LookRotation(dir);
                player_Rigidbody.rotation = Quaternion.Slerp(player_Rigidbody.rotation, rotation, 7.0f * Time.deltaTime); // 구면선형보간 사용

                if (player_Animator.GetCurrentAnimatorStateInfo(0).IsName("Female Sword Attack 1") &&
                    player_Animator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f)
                {
                    player_Animator.SetBool("Attack_Check", false);
                    player_Input.isattack = false;
                }
            }
        }
    }

    private void Skill()
    {
        if (player_Input.skill_on == true)
        {
            if (player_Animator.GetBool("Skill_Check") == false)
            {
                player_Animator.SetBool("Skill_Check", true);
                active_skill_num = player_Input.skill_num;
            }
            else if (player_Animator.GetBool("Skill_Check") == true)
            {
                var dir = player_Input.skill_click_pos - transform.position;
                Quaternion rotation = Quaternion.LookRotation(dir);
                player_Rigidbody.rotation = Quaternion.Slerp(player_Rigidbody.rotation, rotation, 7.0f * Time.deltaTime);

                if (player_Animator.GetCurrentAnimatorStateInfo(0).IsName("Female Skill Attack") &&
                    player_Animator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f)
                {
                    player_Animator.SetBool("Skill_Check", false);
                    player_Input.skill_on = false;
                    player_Input.skill_num = 0;
                    active_skill_num = 0;
                }
            }
        }
    }

    private void Buff()
    {
        if (buff_onoff == true)
        {
            if (walk_status == 0)
                moveSpeed = 4.0f;
            else if (walk_status == 1)
                moveSpeed = 6.0f;

            if (buff_remain_time > buff_timer)
            {
                buff_timer += Time.deltaTime;
                return;
            }
            else
            {
                buff_timer = 0;
                buff_onoff = false;
                ChangeSpeed();
            }
        }
    }

    private void SetWalkRun()
    {
        if (player_Input.walkrun == true)
        {
            if (walk_status == 0)
                walk_status = 1;
            else if (walk_status == 1)
                walk_status = 0;

            ChangeSpeed();
            player_Input.walkrun = false;
        }
    }

    private void ChangeSpeed()
    {
        if (walk_status == 0)
            moveSpeed = 2.0f;
        else if (walk_status == 1)
            moveSpeed = 4.0f;
    }
}
