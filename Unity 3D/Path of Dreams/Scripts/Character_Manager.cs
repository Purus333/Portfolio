using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Character_Manager : MonoBehaviour
{
    private static Character_Manager m_instance;

    public static Character_Manager instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Character_Manager>();
            }
            return m_instance;
        }
    }

    public GameObject character_st;
    private bool character_st_key_check;

    [SerializeField]
    private GameObject slot_parnet;
    [SerializeField]
    private Slot[] slots;

    public Slot tmp_cm_slot = null;

    private void Awake()
    {
        character_st.gameObject.SetActive(true);
        character_st_key_check = false;
        slots = slot_parnet.GetComponentsInChildren<Slot>();
        character_st.gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.C))
            Open_Charater_State();
        if (Input.GetKeyDown(KeyCode.Escape))
            Close_Charater_State();

        Check_Player_Weapon();
    }

    void Open_Charater_State()
    {
        if (character_st_key_check == false)
            character_st_key_check = true;
        else
            character_st_key_check = false;

        if (character_st_key_check == true)
        {
            character_st.transform.parent.SetSiblingIndex(13);
            character_st.gameObject.SetActive(true);
            character_st.transform.GetChild(0).gameObject.SetActive(true);
        }
        else
        {
            character_st.gameObject.SetActive(false);
            character_st.transform.GetChild(0).gameObject.SetActive(false);
        }
    }

    void Close_Charater_State()
    {
        character_st_key_check = false;
        character_st.gameObject.SetActive(false);
        character_st.transform.GetChild(0).gameObject.SetActive(false);
    }

    public void Click_Open_CM()
    {
        character_st.transform.parent.SetSiblingIndex(13);
        character_st_key_check = true;
        character_st.gameObject.SetActive(true);
        character_st.transform.GetChild(0).gameObject.SetActive(true);
    }

    public void Click_Close_CM()
    {
        character_st_key_check = false;
        character_st.gameObject.SetActive(false);
        character_st.transform.GetChild(0).gameObject.SetActive(false);
    }

    public bool Equip_Item(Itemm _item, int _item_id, int cnt = 1)
    {
        bool check = false;

        for (int i = 0; i < slots.Length; i++)
        {
            check = Check_Item_Type(_item, _item_id, slots[i]);
            if (check == true && slots[i].item_valid_check == false)
            {
                tmp_cm_slot = new Slot();
                tmp_cm_slot.item_valid_check = false;

                slots[i].Add_Item(_item, _item_id, cnt);
                return check;
            }
            else if (check == true && slots[i].item_valid_check == true)
            {
                tmp_cm_slot = new Slot();
                tmp_cm_slot.item = slots[i].item;
                tmp_cm_slot.item_id = slots[i].item_id;
                tmp_cm_slot.item_image = slots[i].item_image;
                tmp_cm_slot.item_count = slots[i].item_count;
                tmp_cm_slot.item_count_text = slots[i].item_count_text;
                tmp_cm_slot.item_valid_check = slots[i].item_valid_check;
                tmp_cm_slot.slot_type_check = slots[i].slot_type_check;
                tmp_cm_slot.click_time = slots[i].click_time;

                slots[i].Add_Item(_item, _item_id, cnt);
                return check;
            }
        }
        return false;
    }

    public bool Check_Item_Type(Itemm _item, int _item_id, Slot slot)
    {
        if (_item.equip_type == Itemm.Equip_Type.Helmet && slot.gameObject.tag == "Helmet")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Torso && slot.gameObject.tag == "Torso")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Pants && slot.gameObject.tag == "Pants")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Shoes && slot.gameObject.tag == "Shoes")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Weapon && slot.gameObject.tag == "Weapon")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Glove && slot.gameObject.tag == "Glove")
            return true;
        else if (_item.equip_type == Itemm.Equip_Type.Cloak && slot.gameObject.tag == "Cloak")
            return true;
        else
            return false;
    }

    public void Check_Player_Weapon()
    {
        for (int i = 0; i < slots.Length; i++)
        {
            if (slots[i].gameObject.tag == "Weapon" && slots[i].item_valid_check == true)
            {
                Player_Info.instance.Player_Weapon_Obj.gameObject.SetActive(true);
                break;
            }
            else if (slots[i].gameObject.tag == "Weapon" && slots[i].item_valid_check == false)
            {
                Player_Info.instance.Player_Weapon_Obj.gameObject.SetActive(false);
                break;
            }
        }
    }

    public Slot[] Get_Cm_Slots()
    {
        return slots;
    }

    public void Load_Cm(int _arraynum, string _item_name, int _item_count)
    {
        for (int i = 0; i < GameManager.instance.item_list.Count; i++)
        {
            if (GameManager.instance.item_list[i].item_name == _item_name)
                slots[_arraynum].Add_Item(GameManager.instance.item_list[i], GameManager.instance.item_list[i].item_id, _item_count);
        }
    }
}
