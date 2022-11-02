using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Skill_AttackPoint : MonoBehaviour
{
    public Animator ani_player;
    public GameObject[] attack_range_obj;
    public BoxCollider[] attack_range;

    private void Awake()
    {
        for (int i = 0; i < attack_range_obj.Length; i++)
        {
            attack_range_obj[i].gameObject.SetActive(false);
            attack_range[i].enabled = false;
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Skill_Attack_On()
    {
        if (Player_Movement.instance.player_Animator.GetBool("Skill_Check") == false)
            return;
        else
        {
            // 각각 다른스킬 3개를 3번 연달아 입력했을때 3개의스킬 전부 체크되는걸 방지하기위해
            // 애니메이션에 함수를 추가하여 콜라이더가 on인 상태일때만 체크하도록 함
            if (Player_Movement.instance.active_skill_num == 1 && Player_Info.instance.skill1_check == false)
                Player_Info.instance.skill1_check = true;
            else if (Player_Movement.instance.active_skill_num == 2 && Player_Info.instance.skill2_check == false)
                Player_Info.instance.skill2_check = true;
            else if (Player_Movement.instance.active_skill_num == 3 && Player_Info.instance.skill3_check == false)
            {
                Player_Info.instance.skill3_check = true;
                Player_Movement.instance.buff_onoff = true;
            }
        }

        if (Player_Movement.instance.active_skill_num == 1)
        {
            Player_Info.instance.skill1_effect.Play();
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_skill1_sound);
        }
        else if (Player_Movement.instance.active_skill_num == 2)
        {
            Player_Info.instance.skill2_effect.Play();
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_skill2_sound);
        }
        else if (Player_Movement.instance.active_skill_num == 3)
        {
            Player_Info.instance.skill3_effect.Play();
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_skill3_sound);
        }

        attack_range_obj[Player_Movement.instance.active_skill_num - 1].gameObject.SetActive(true);
        attack_range[Player_Movement.instance.active_skill_num - 1].enabled = true;
    }

    public void Skill_Attack_Off()
    {
        if (Player_Movement.instance.player_Animator.GetBool("Skill_Check") == false)
            return;

        if (attack_range_obj[Player_Movement.instance.active_skill_num - 1].gameObject.activeSelf == true)
            attack_range_obj[Player_Movement.instance.active_skill_num - 1].gameObject.SetActive(false);

        if (attack_range[Player_Movement.instance.active_skill_num - 1].enabled == true)
            attack_range[Player_Movement.instance.active_skill_num - 1].enabled = false;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Enemy_HitPoint") && attack_range_obj[0].activeSelf == true &&
            this.gameObject.tag == "Player")
        {
            int skill_dam = 50;
            int num = Random.Range(0, 101);

            if (Player_Info.instance.Critical >= num)
                other.gameObject.GetComponentInParent<Enemy>().OnDamage((skill_dam + Player_Info.instance.Power) * 2, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
            else
                other.gameObject.GetComponentInParent<Enemy>().OnDamage((skill_dam + Player_Info.instance.Power), new Vector3(0, 0, 0), new Vector3(0, 0, 0));
        }
        else if (other.gameObject.CompareTag("Enemy_HitPoint") && attack_range_obj[1].activeSelf == true &&
            this.gameObject.tag == "Player")
        {
            int skill_dam = 100;
            int num = Random.Range(0, 101);

            if (Player_Info.instance.Critical >= num)
                other.gameObject.GetComponentInParent<Enemy>().OnDamage((skill_dam + Player_Info.instance.Power) * 2, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
            else
                other.gameObject.GetComponentInParent<Enemy>().OnDamage((skill_dam + Player_Info.instance.Power), new Vector3(0, 0, 0), new Vector3(0, 0, 0));
        }
    }
}
