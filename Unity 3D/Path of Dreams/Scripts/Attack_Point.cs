using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Attack_Point : MonoBehaviour
{
    public Animator ani_player;
    public BoxCollider attack_range;
    public GameObject attack_range_obj;

    private void Awake()
    {
        attack_range_obj.gameObject.SetActive(false);
        attack_range.enabled = false;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Attack_On()
    {
        Player_Movement.instance.player_audio.PlayOneShot(Player_Movement.instance.player_attack_sound);
        attack_range_obj.gameObject.SetActive(true);
        attack_range.enabled = true;
    }

    public void Attack_Off()
    {
        attack_range_obj.gameObject.SetActive(false);
        attack_range.enabled = false;
    }

    public void Enemy_Attack_On()
    {
        attack_range_obj.gameObject.SetActive(true);
        attack_range.enabled = true;
    }

    public void Enemy_Attack_Off()
    {
        attack_range_obj.gameObject.SetActive(false);
        attack_range.enabled = false;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Enemy_HitPoint") && attack_range_obj.activeSelf == true &&
            this.gameObject.tag == "Player")
        {
            int num = Random.Range(0, 101);

            if (Player_Info.instance.Critical >= num)
                other.gameObject.GetComponentInParent<Enemy>().OnDamage(Player_Info.instance.Power * 2, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
            else
                other.gameObject.GetComponentInParent<Enemy>().OnDamage(Player_Info.instance.Power, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
        }
        else if (other.gameObject.CompareTag("Player_HitPoint") && attack_range_obj.activeSelf == true)
        {
            if (this.gameObject.name == "Polygonal Metalon Green" || this.gameObject.name == "Polygonal Metalon Green(Clone)")
            {
                int value = Random.Range(3, 6);

                other.gameObject.GetComponentInParent<Player_Info>().OnDamage(value, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
            }
            else if (this.gameObject.name == "Polygonal Metalon Red" || this.gameObject.name == "Polygonal Metalon Red(Clone)")
            {
                int value = Random.Range(15, 23);

                other.gameObject.GetComponentInParent<Player_Info>().OnDamage(value, new Vector3(0, 0, 0), new Vector3(0, 0, 0));
            }
        }
    }
}
