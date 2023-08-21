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

    private void Awake()
    {
        
    }

    void Start()
    {
        
    }

    void Update()
    {
        
    }

    public void Set_Damage(Transform pos, float dam)
    {
        for (int i = 0; i < show_dam.Count; i++)
        {
            if (show_dam[i].enabled == false)
            {
                // pos 추가
                show_dam[i].show_damage = dam;
            }
            else
            {
                Damage_Show temp_dam_show = null;
                // pos 추가
                temp_dam_show.show_damage = dam;
                show_dam.Add(temp_dam_show);
            }
        }
    }
}
