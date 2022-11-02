using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Player_Input : MonoBehaviour
{
    // �������� ��Ʈ�ѷ��� Ʋ������ �ֱ⿡ input class�� ������ ����
    static public bool use_drag = false;
    public bool walkrun;

    public Vector3 click_pos;
    public Vector3 skill_click_pos;
    public bool ismove = false;
    public bool moving_on = false; // �����̵��� ���Ѻ��� & uiȭ�� Ŭ������ ���¿��� �̵������Ѱ����� ���콺�� ������ ��� ĳ���� �̵������� ����
    public float move_animotion = 0;
    public bool isattack = false;
    public bool attak_on = false;

    public bool skill_on = false;
    public int skill_num = 0;

    private void Awake()
    {

    }

    void Start()
    {

    }

    void Update()
    {
        Input_Check();
    }

    public void Input_Check()
    {   
        /////////// �̵�
        if (Input.GetMouseButtonDown(0) && !EventSystem.current.IsPointerOverGameObject())
            moving_on = true;
        if (Input.GetMouseButtonUp(0) && !EventSystem.current.IsPointerOverGameObject())
            moving_on = false;

        //use_skill = Input.GetAxis(Skill_1);
        if ((Input.GetMouseButtonDown(0) && !EventSystem.current.IsPointerOverGameObject()) ||
            (Input.GetMouseButton(0) && !EventSystem.current.IsPointerOverGameObject()) && moving_on == true)
        {
            if (use_drag == true || isattack == true || skill_on == true)
                return;

            click_pos = Vector3.one;

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if (Physics.Raycast(ray, out hit))
            {
                click_pos = hit.point;
                ismove = true;
            }
        }

        //////////// ����
        if (Input.GetMouseButtonDown(1) && !EventSystem.current.IsPointerOverGameObject())
            attak_on = true;
        if (Input.GetMouseButtonUp(1) && !EventSystem.current.IsPointerOverGameObject())
            attak_on = false;

        if (Input.GetMouseButtonDown(1) && !EventSystem.current.IsPointerOverGameObject() ||
            Input.GetMouseButton(1) && !EventSystem.current.IsPointerOverGameObject() && attak_on == true)
        {
            if (skill_on == true) // ���Է¹���
                return;

            click_pos = Vector3.one;

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if (Physics.Raycast(ray, out hit))
            {
                click_pos = hit.point;
                isattack = true;
            }
        }

        if (Input.GetKey("q") && Player_Info.instance.skill1_check == false)
            skill_num = 1;
        else if (Input.GetKey("w") && Player_Info.instance.skill2_check == false)
            skill_num = 2;
        else if (Input.GetKey("e") && Player_Info.instance.skill3_check == false)
            skill_num = 3;
        if (skill_num != 0)
        {
            if (isattack == true || GameManager.instance.newgame_started == false) // ���Է¹���
            {
                skill_num = 0;
                return;
            }

            skill_click_pos = Vector3.one;

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if (Physics.Raycast(ray, out hit))
            {
                skill_click_pos = hit.point;
                skill_on = true;
            }
        }

        if (Input.GetKeyDown("f") && Player_Info.instance.player_talkable == true && 
            GameManager.instance.npc_text_start == false && GameManager.instance.store_open == false)
            Player_Info.instance.player_talkstart = true;

        if (Input.GetKeyUp("z"))
            walkrun = true;
    }
}
