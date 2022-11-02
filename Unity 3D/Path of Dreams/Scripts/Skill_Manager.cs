using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Skill_Manager : MonoBehaviour
{

    private bool[] skill_coolcheck = new bool[3];
    private float[] skill_timer = new float[3];
    private float[] skill_cool_time = new float[3];

    private void Awake()
    {
        for (int i = 0; i < skill_coolcheck.Length; i++)
            skill_coolcheck[i] = false;

        for (int i = 0; i < skill_timer.Length; i++)
            skill_timer[i] = 0;

        skill_cool_time[0] = 5;
        skill_cool_time[1] = 15;
        skill_cool_time[2] = 30;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Check_Skill();
        Timer();
    }

    public void Check_Skill()
    {
        if (Player_Info.instance.skill1_check == true && skill_coolcheck[0] == false)
            skill_coolcheck[0] = true;
        else if (Player_Info.instance.skill2_check == true && skill_coolcheck[1] == false)
            skill_coolcheck[1] = true;
        else if (Player_Info.instance.skill3_check == true && skill_coolcheck[2] == false)
            skill_coolcheck[2] = true;
    }

    public void Timer()
    {
        for (int i = 0; i < 3; i++)
        {
            if (skill_coolcheck[i] == true)
            {
                if (skill_cool_time[i] > skill_timer[i])
                {
                    Ui_Manager.instance.UserSkill_CoolTime(i, (int)skill_cool_time[i] - (int)skill_timer[i]);
                    skill_timer[i] += Time.deltaTime;
                }
                else
                {
                    Ui_Manager.instance.UserSkill_CoolTime(i, 0);
                    skill_coolcheck[i] = false;
                    skill_timer[i] = 0;

                    if (i == 0)
                        Player_Info.instance.skill1_check = false;
                    else if (i == 1)
                        Player_Info.instance.skill2_check = false;
                    else if (i == 2)
                        Player_Info.instance.skill3_check = false;
                }
            }
        }
    }
}
