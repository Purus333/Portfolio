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

    private List<Damage_Show> show_dam = new List<Damage_Show>();
    public GameObject e_hud_damage_txt;
    public GameObject p_hud_damage_txt;

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
            for (int i = 0; i < show_dam.Count; i++)
            {
                if (show_dam[i].enabled == false)
                {
                    show_dam[i].transform.position = pos.position;
                    show_dam[i].show_damage = dam;
                }
                else
                {
                    Damage_Show temp_dam_show = null;
                    Instantiate(p_hud_damage_txt);

                    temp_dam_show.transform.position = pos.position;
                    temp_dam_show.show_damage = dam;
                    show_dam.Add(temp_dam_show);
                }
            }
        }
        else if (check_type == 200)
        {
            for (int i = 0; i < show_dam.Count; i++)
            {
                if (show_dam[i].enabled == false)
                {
                    show_dam[i].transform.position = pos.position;
                    show_dam[i].show_damage = dam;
                }
                else
                {
                    Damage_Show temp_dam_show = null;
                    Instantiate(e_hud_damage_txt);

                    temp_dam_show.transform.position = pos.position;
                    temp_dam_show.show_damage = dam;
                    show_dam.Add(temp_dam_show);
                }
            }
        }
    }
}
