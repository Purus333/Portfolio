using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Damage_Show_Pool : MonoBehaviour
{
    private static Damage_Show_Pool m_instance;

    public static Damage_Show_Pool instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Damage_Show_Pool>();
            }
            return m_instance;
        }
    }

    private List<GameObject> show_dam_obj_e = new List<GameObject>();
    private List<GameObject> show_dam_obj_p = new List<GameObject>();
    public GameObject e_hud_damage_txt;
    public GameObject p_hud_damage_txt;
    public GameObject pool_e;
    public GameObject pool_p;
    public bool create_trigger_e = false;
    public bool create_trigger_p = false;

    private void Awake()
    {
        
    }

    void Start()
    {

    }

    void Update()
    {
        
    }

    public void Set_Damage(Transform pos, float dam, int check_type)
    {
        if (check_type == 100)
        {
            if (show_dam_obj_p.Count == 0)
            {
                GameObject hudtxt = Instantiate(p_hud_damage_txt);
                hudtxt.transform.position = pos.position;
                hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                show_dam_obj_p.Add(hudtxt);
                hudtxt.transform.parent = pool_p.transform;
            }
            else if (show_dam_obj_p.Count > 0)
            {
                for (int i = 0; i < show_dam_obj_p.Count; i++)
                {
                    if (show_dam_obj_p[i].activeSelf == false)
                    {
                        show_dam_obj_p[i].transform.position = pos.position;
                        show_dam_obj_p[i].GetComponent<Damage_Show>().show_damage = dam;
                        show_dam_obj_p[i].GetComponent<Damage_Show>().On();
                        create_trigger_p = false;
                        break;
                    }
                    else
                        create_trigger_p = true;
                }

                if (create_trigger_p == true)
                {
                    GameObject hudtxt = Instantiate(p_hud_damage_txt);
                    hudtxt.transform.position = pos.position;
                    hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                    show_dam_obj_p.Add(hudtxt);
                    hudtxt.transform.parent = pool_p.transform;
                    create_trigger_p = false;
                }
            }
        }
        else if (check_type == 200)
        {
            if (show_dam_obj_e.Count == 0)
            {
                GameObject hudtxt = Instantiate(e_hud_damage_txt);
                hudtxt.transform.position = pos.position;
                hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                show_dam_obj_e.Add(hudtxt);
                hudtxt.transform.parent = pool_e.transform;
            }
            else if (show_dam_obj_e.Count > 0)
            {
                for (int i = 0; i < show_dam_obj_e.Count; i++)
                {
                    if (show_dam_obj_e[i].activeSelf == false)
                    {
                        show_dam_obj_e[i].transform.position = pos.position;
                        show_dam_obj_e[i].GetComponent<Damage_Show>().show_damage = dam;
                        show_dam_obj_e[i].GetComponent<Damage_Show>().On();
                        create_trigger_e = false;
                        break;
                    }
                    else
                        create_trigger_e = true;
                }

                if (create_trigger_e == true)
                {
                    GameObject hudtxt = Instantiate(e_hud_damage_txt);
                    hudtxt.transform.position = pos.position;
                    hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                    show_dam_obj_e.Add(hudtxt);
                    hudtxt.transform.parent = pool_e.transform;
                    create_trigger_e = false;
                }
            }
        }
    }
}
