using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Inventory_Manager : MonoBehaviour
{
    private static Inventory_Manager m_instance;

    public static Inventory_Manager instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Inventory_Manager>();
            }
            return m_instance;
        }
    }

    public GameObject inven;
    private bool inven_key_check;

    [SerializeField]
    private GameObject slot_parnet;
    [SerializeField]
    private Slot[] slots;

    public bool cooltime_trigger;

    private void Awake()
    {
        inven.gameObject.SetActive(true); // 한번활성화 해야지 슬롯이 적용된다 (list공간)
        inven_key_check = false;
        slots = slot_parnet.GetComponentsInChildren<Slot>();
        inven.gameObject.SetActive(false);
        cooltime_trigger = false;
    }

    void Start()
    {

    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.I))
            Open_Inventory();
        if (Input.GetKeyDown(KeyCode.Escape))
            Clsoe_Inventory();
        if (Portion.instance.cooltime_check == true)
            Use_Item_CoolTime_On();
        if (Portion.instance.cooltime_check == false && cooltime_trigger == true)
            Use_Item_CoolTime_Off();

        Quest_Item_Check();
    }

    void Open_Inventory()
    {
        if (inven_key_check == false)
            inven_key_check = true;
        else
            inven_key_check = false;

        if (inven_key_check == true)
        {
            inven.transform.parent.SetSiblingIndex(13); // ui 우선순위 변경
            inven.gameObject.SetActive(true);
            inven.transform.GetChild(0).gameObject.SetActive(true);
        }
        else
        {
            inven.gameObject.SetActive(false);
            inven.transform.GetChild(0).gameObject.SetActive(false);
        }
    }

    void Clsoe_Inventory()
    {
        inven_key_check = false;
        inven.gameObject.SetActive(false);
        inven.transform.GetChild(0).gameObject.SetActive(false);
    }

    public void Click_Open_Inventory()
    {
        inven.transform.parent.SetSiblingIndex(13);
        inven_key_check = true;
        inven.gameObject.SetActive(true);
        inven.transform.GetChild(0).gameObject.SetActive(true);
    }

    public void Click_Close_Inventory() // 마우스버튼용
    {
        inven_key_check = false;
        inven.gameObject.SetActive(false);
        inven.transform.GetChild(0).gameObject.SetActive(false);
    }

    public bool Get_Item(Itemm _item, int _item_id, int cnt = 1)
    {
        if (Itemm.Item_Type.Equip != _item.item_type) // 장비템이 아닐시에
        {
            if (Portion.instance.p_slot.item_valid_check == true && Portion.instance.p_slot.item.item_name == _item.item_name)
            { // 물약슬롯에 있는 물약과 같은 물약을 획득시
                Portion.instance.p_slot.Slot_Count(cnt);
                Ui_Manager.instance.Item_Get_Notice(_item);
                return true;
            }

            for (int i = 0; i < slots.Length; i++)
            {
                if (slots[i].item_valid_check == true && slots[i].item.item_name == _item.item_name)
                {
                    slots[i].Slot_Count(cnt); // 아이템갯수 카운트
                    Ui_Manager.instance.Item_Get_Notice(_item);
                    return true;
                }
            }
        }

        for (int i = 0; i < slots.Length; i++)
        {
            if (slots[i].item_valid_check == false)
            {
                slots[i].Add_Item(_item, _item_id, cnt);
                Ui_Manager.instance.Item_Get_Notice(_item);
                return true;
            }
        }

        Ui_Manager.instance.Inven_Full_Notice();

        return false;
    }

    public bool Get_Item_CharToInven(Itemm _item, int _item_id, int cnt = 1) // 장비창 > 인벤창 전용함수
    { // 이 함수를 써야 중복으로 획득ui가 뜨지않는다
        if (Itemm.Item_Type.Equip != _item.item_type)
        {
            if (Portion.instance.p_slot.item_valid_check == true && Portion.instance.p_slot.item.item_name == _item.item_name)
            {
                Portion.instance.p_slot.Slot_Count(cnt);
                return true;
            }

            for (int i = 0; i < slots.Length; i++)
            {
                if (slots[i].item_valid_check == true && slots[i].item.item_name == _item.item_name)
                {
                    slots[i].Slot_Count(cnt);
                    return true;
                }
            }
        }

        for (int i = 0; i < slots.Length; i++)
        {
            if (slots[i].item_valid_check == false)
            {
                slots[i].Add_Item(_item, _item_id, cnt);
                return true;
            }
        }

        Ui_Manager.instance.Inven_Full_Notice();

        return false;
    }

    public bool Use_Item(Itemm _item, int _item_id)
    {
        if (Player_Info.instance.Player_Max_Health > Player_Info.instance.health && Portion.instance.cooltime_check == false)
        {
            if (_item.item_id == 3001)
                Player_Info.instance.RestoreHealth(50);
            else if (_item.item_id == 3002)
                Player_Info.instance.RestoreHealth(100);
            else if (_item.item_id == 3003)
                Player_Info.instance.RestoreHealth(200);

            Portion.instance.cooltime_check = true;
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_potion_sound);

            return true;
        }
        return false;
    }

    public void Use_Item_CoolTime_On()
    {
        for (int i = 0; i < slots.Length; i++)
            if (slots[i].item_id != 0 && slots[i].item.item_type == Itemm.Item_Type.Use && slots[i].item_count > 0)
                slots[i].item_image.color = (new Color(1, 0, 0, 1));

        cooltime_trigger = true;
    }

    public void Use_Item_CoolTime_Off()
    {
        for (int i = 0; i < slots.Length; i++)
            if (slots[i].item_id != 0 && slots[i].item.item_type == Itemm.Item_Type.Use && slots[i].item_count > 0)
                slots[i].item_image.color = (new Color(1, 1, 1, 1));

        cooltime_trigger = false;
    }

    public void Quest_Item_Check()
    {
        int tmp_count = 0;

        if (Player_Info.instance.player_villagenpc03_stat == 1 || Player_Info.instance.player_villagenpc03_stat == 2)
        {
            for (int i = 0; i < slots.Length; i++)
                if (slots[i].item_valid_check == true && slots[i].item.item_id == 4006)
                    tmp_count += slots[i].item_count;

            Player_Info.instance.player_quest_itemcount = tmp_count;
            Ui_Manager.instance.Quest_Info(1, Player_Info.instance.player_quest_itemcount);

            if (Player_Info.instance.player_quest_itemcount >= 15)
                Player_Info.instance.player_villagenpc03_stat = 2;
        }
    }

    public void Quest_Item_Delete(int _quest_num, int _item_id)
    {
        for (int i = 0; i < slots.Length; i++)
            if (slots[i].item_valid_check == true && slots[i].item.item_id == _item_id)
                if (_quest_num == 1)
                    slots[i].Slot_Count(-15);
    }

    public bool Delete_Item(Itemm _item, int _item_id, int cnt = 1)
    {
        for (int i = 0; i < slots.Length; i++)
        {
            if (slots[i].item_valid_check == true && slots[i].item.item_name == _item.item_name)
            {
                slots[i].Slot_Count(-cnt);
                return true;
            }
        }
        return false;
    }

    public Slot[] Get_Inven_Slots()
    {
        return slots;
    }

    public void Load_Inven(int _arraynum, string _item_name, int _item_count)
    {
        for (int i = 0; i < GameManager.instance.item_list.Count; i++)
        {
            if (GameManager.instance.item_list[i].item_name == _item_name)
                slots[_arraynum].Add_Item(GameManager.instance.item_list[i], GameManager.instance.item_list[i].item_id, _item_count);
        }
    }
}
