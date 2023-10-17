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

    private List<GameObject> show_dam_obj = new List<GameObject>();
    public GameObject e_hud_damage_txt;
    public GameObject p_hud_damage_txt;
    public GameObject pool_e;
    public GameObject pool_p;
    public bool create_trigger = false;

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
            if (show_dam_obj.Count == 0)
            {
                GameObject hudtxt = Instantiate(p_hud_damage_txt);
                hudtxt.transform.position = pos.position;
                hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                show_dam_obj.Add(hudtxt);
                //pool_p.transform.parent = hudtxt.transform;
            }
            else if (show_dam_obj.Count > 0)
            {
                for (int i = 0; i < show_dam_obj.Count; i++)
                {
                    if (show_dam_obj[i].activeSelf == false)
                    {
                        show_dam_obj[i].transform.position = pos.position;
                        show_dam_obj[i].GetComponent<Damage_Show>().show_damage = dam;
                        show_dam_obj[i].GetComponent<Damage_Show>().On();
                        create_trigger = false;
                        break;
                    }
                    else
                        create_trigger = true;
                }

                if (create_trigger == true)
                {
                    GameObject hudtxt = Instantiate(p_hud_damage_txt);
                    hudtxt.transform.position = pos.position;
                    hudtxt.GetComponent<Damage_Show>().show_damage = dam;
                    show_dam_obj.Add(hudtxt);
                    create_trigger = false;
                }
            }
        }
        else if (check_type == 200)
        {
          
            //for (int i = 0; i < show_dam.Count; i++)
            //{
            //    if (show_dam[i].enabled == false)
             //   {
             //       show_dam[i].transform.position = pos.position;
             //       show_dam[i].show_damage = dam;
             //   }
             //   else
             //   {
             //       Damage_Show temp_dam_show = null;
             //       Instantiate(e_hud_damage_txt);
            //
            //        temp_dam_show.transform.position = pos.position;
             //       temp_dam_show.show_damage = dam;
             //       show_dam.Add(temp_dam_show);
             //   }
             //
            
        }
    }
}
