using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class Item_Store_Check : MonoBehaviour
{
    private static Item_Store_Check m_instance;

    public static Item_Store_Check instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Item_Store_Check>();
            }
            return m_instance;
        }
    }

    public InputField buy_input_count_text;
    public InputField sell_input_count_text;
    public GameObject buy_panal_obj;
    public GameObject sell_panal_obj;
    public GameObject buy_count_panal_obj;
    public GameObject sell_count_panal_obj;

    public Itemm tmp_item;
    public int tmp_item_id;
    public int tmp_item_count;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Buy_Ok()
    {
        if (tmp_item.item_type == Itemm.Item_Type.Equip)
        {
            if (Player_Info.instance.Player_Gold >= tmp_item.item_price) // gold 체크
            {
                bool check = false;

                check = Inventory_Manager.instance.Get_Item(tmp_item, tmp_item_id); // 인벤 공간 체크

                if (check == true)
                {
                    Player_Info.instance.Player_Gold -= tmp_item.item_price;
                    Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
                }
            }
        }
        else if (tmp_item.item_type == Itemm.Item_Type.Use || tmp_item.item_type == Itemm.Item_Type.Etc)
            buy_count_panal_obj.gameObject.SetActive(true);

        buy_panal_obj.SetActive(false);
    }

    public void Buy_Cancel()
    {
        buy_panal_obj.SetActive(false);
    }

    public void Sell_Ok()
    {
        if (tmp_item.item_type == Itemm.Item_Type.Equip)
        {
            bool check = false;

            check = Inventory_Manager.instance.Delete_Item(tmp_item, tmp_item_id);

            if (check == true)
            {
                Player_Info.instance.Player_Gold += tmp_item.item_price / 2;
                Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
            }
        }
        else if (tmp_item.item_type == Itemm.Item_Type.Use || tmp_item.item_type == Itemm.Item_Type.Etc)
            sell_count_panal_obj.gameObject.SetActive(true);

        sell_panal_obj.SetActive(false);
    }

    public void Sell_Cancel()
    {
        sell_panal_obj.SetActive(false);
    }

    public void Buy_Count_Ok()
    {
        int num = 0;

        int.TryParse(buy_input_count_text.text, out num);

        if (buy_input_count_text.text.Length > 0 && num > 0)
        {
            if (Player_Info.instance.Player_Gold >= tmp_item.item_price * num)
            {
                bool check = false;

                check = Inventory_Manager.instance.Get_Item(tmp_item, tmp_item_id, num);

                if (check == true)
                {
                    Player_Info.instance.Player_Gold -= tmp_item.item_price * num;
                    Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
                }
            }
        }

        if (buy_input_count_text.text.Length > 0)
            buy_count_panal_obj.gameObject.SetActive(false);

        buy_input_count_text.text = "";
    }

    public void Buy_Count_Cancel()
    {
        buy_count_panal_obj.gameObject.SetActive(false);
        buy_input_count_text.text = "";
    }

    public void Sell_Count_Ok()
    {
        int num = 0;

        int.TryParse(sell_input_count_text.text, out num);

        if (sell_input_count_text.text.Length > 0 && num > 0)
        {
            if (tmp_item_count >= num)
            {
                bool check = false;

                check = Inventory_Manager.instance.Delete_Item(tmp_item, tmp_item_id, num);

                if (check == true)
                {
                    Player_Info.instance.Player_Gold += (tmp_item.item_price * num) / 2;
                    Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
                }
            }
        }

        if (sell_input_count_text.text.Length > 0)
            sell_count_panal_obj.gameObject.SetActive(false);

        sell_input_count_text.text = "";
    }

    public void Sell_Count_Cancel()
    {
        sell_count_panal_obj.gameObject.SetActive(false);
        sell_input_count_text.text = "";
    }
}
