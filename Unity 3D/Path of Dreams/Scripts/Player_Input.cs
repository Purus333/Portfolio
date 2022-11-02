using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Player_Input : MonoBehaviour
{
    // 기종마다 컨트롤러가 틀려질수 있기에 input class를 생성해 관리
    static public bool use_drag = false;
    public bool walkrun;

    public Vector3 click_pos;
    public Vector3 skill_click_pos;
    public bool ismove = false;
    public bool moving_on = false; // 지속이동을 위한변수 & ui화면 클릭유지 상태에서 이동가능한곳으로 마우스를 움직일 경우 캐릭터 이동방지를 위함
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
        /////////// 이동
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

        //////////// 공격
        if (Input.GetMouseButtonDown(1) && !EventSystem.current.IsPointerOverGameObject())
            attak_on = true;
        if (Input.GetMouseButtonUp(1) && !EventSystem.current.IsPointerOverGameObject())
            attak_on = false;

        if (Input.GetMouseButtonDown(1) && !EventSystem.current.IsPointerOverGameObject() ||
            Input.GetMouseButton(1) && !EventSystem.current.IsPointerOverGameObject() && attak_on == true)
        {
            if (skill_on == true) // 선입력방지
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
            if (isattack == true || GameManager.instance.newgame_started == false) // 선입력방지
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
