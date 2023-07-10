using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Slot : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerClickHandler, IBeginDragHandler, IDragHandler, IDropHandler, IEndDragHandler
{
    public Itemm item;
    public int item_id;
    public Image item_image;
    public int item_count; // 슬롯의 아이템 카운트
    public Text item_count_text;
    public bool item_valid_check;
    public GameObject slot_type_check;
    public float click_time;
    public ToolTip Tp;
    public Item_Store_Check Item_sc;

    private void Awake()
    {
        item_valid_check = false;
        item_id = 0;
        click_time = 0.0f;
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    public void Add_Item(Itemm _item, int _item_id, int _item_count = 1)
    {
        item = _item;
        item_id = _item_id;
        item_count = _item_count;
        item_image.sprite = item.item_image;
        item_valid_check = true;

        if (item.item_type != Itemm.Item_Type.Equip)
        {
            item_count_text.gameObject.SetActive(true);
            item_count_text.text = item_count.ToString();
        }
        else
        {
            item_count_text.text = "";
            item_count_text.gameObject.SetActive(false);
        }

        Set_Color(1);
    }

    public void Set_Color(float alpha)
    {
        Color color = item_image.color;
        color.a = alpha;
        item_image.color = color;
    }

    public void Slot_Count(int cnt)
    {
        item_count += cnt;
        item_count_text.text = item_count.ToString();

        if (item_count <= 0)
            Slot_Clear();
    }

    public void Slot_Clear()
    {
        item = null;
        item_valid_check = false;
        item_count = 0;
        item_id = 0;
        item_image.sprite = null;
        Set_Color(0); // 투명화

        item_count_text.text = "";
        item_count_text.gameObject.SetActive(false);
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if (item_valid_check == true)
            Tp.Show_ToolTip(item, transform.position);
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        Tp.Hide_ToolTip();
    }

    public void OnPointerClick(PointerEventData eventdata)
    {
        bool check = false;

        if (Item_Delete_Check.instance.delete_ui_obj.activeSelf == true) // ui창 띄운상태에서는 리턴(버그방지)
            return;

        if ((eventdata.button == PointerEventData.InputButton.Right && item_id != 0 && item_count != 0) || 
            (eventdata.button == PointerEventData.InputButton.Left && item_id != 0 && item_count != 0 && Time.time - click_time < 0.25f)) // 마우스 이벤트
        {

            if (Time.time - click_time < 0.25f)
            {
                click_time = -1;
            }

            if (GameManager.instance.store_open == true && this.gameObject.tag == "Npc_Inven") // 상점구매
            {
                if (Item_sc.buy_panal_obj.gameObject.activeSelf == false)
                {
                    Item_sc.tmp_item = this.item;
                    Item_sc.tmp_item_id = this.item_id;
                    Item_sc.buy_panal_obj.gameObject.SetActive(true);
                }
            }
            else if (GameManager.instance.store_open == true && this.gameObject.tag == "Slot") // 상점판매
            {
                if (Item_sc.sell_panal_obj.gameObject.activeSelf == false)
                {
                    Item_sc.tmp_item = this.item;
                    Item_sc.tmp_item_id = this.item_id;
                    Item_sc.tmp_item_count = this.item_count;
                    Item_sc.sell_panal_obj.gameObject.SetActive(true);
                }
            }

            else if (GameManager.instance.store_open == false)
            {
                if (item.item_type == Itemm.Item_Type.Equip)
                {
                    if (slot_type_check.gameObject.tag == "Slot") // 인벤창 -> 캐릭창
                    {
                        check = Character_Manager.instance.Equip_Item(item, item_id);
                        if (check == true && Character_Manager.instance.tmp_cm_slot.item_valid_check == false)
                        {
                            Player_Info.instance.Player_Equip_On(item.item_str, item.item_dex, item.item_con, item.item_power, item.item_defense, item.item_critical); //캐릭터 스텟적용

                            Slot_Count(-1);
                        }
                        else if (check == true && Character_Manager.instance.tmp_cm_slot.item_valid_check == true)
                        {
                            Player_Info.instance.Player_Equip_Off(Character_Manager.instance.tmp_cm_slot.item.item_str, Character_Manager.instance.tmp_cm_slot.item.item_dex, Character_Manager.instance.tmp_cm_slot.item.item_con,
                                Character_Manager.instance.tmp_cm_slot.item.item_power, Character_Manager.instance.tmp_cm_slot.item.item_defense, Character_Manager.instance.tmp_cm_slot.item.item_critical); //캐릭터 스텟적용
                            Player_Info.instance.Player_Equip_On(item.item_str, item.item_dex, item.item_con, item.item_power, item.item_defense, item.item_critical);

                            Add_Item(Character_Manager.instance.tmp_cm_slot.item, Character_Manager.instance.tmp_cm_slot.item_id, Character_Manager.instance.tmp_cm_slot.item_count);
                        }
                    }
                    else // 캐릭창 -> 인벤창
                    {
                        check = Inventory_Manager.instance.Get_Item_CharToInven(item, item_id);

                        if (check == true)
                        {
                            Player_Info.instance.Player_Equip_Off(item.item_str, item.item_dex, item.item_con, item.item_power, item.item_defense, item.item_critical); //캐릭터 스텟적용

                            Slot_Count(-1);
                        }
                    }

                    // 스텟ui 업데이트
                    Ui_Manager.instance.UserDetailInfo(Player_Info.instance.Player_Level, Player_Info.instance.Player_Str, Player_Info.instance.Player_Dex,
                        Player_Info.instance.Player_Con, Player_Info.instance.Power, Player_Info.instance.Defense, Player_Info.instance.Critical);
                }
                else if (item.item_type == Itemm.Item_Type.Use)
                {
                    bool value = false;

                    value = Inventory_Manager.instance.Use_Item(item, item_id);

                    if (value == true)
                        Slot_Count(-1);
                }
            }

            
        }
        else if (eventdata.button == PointerEventData.InputButton.Left && item_id != 0 && item_count != 0)
        {
            // 더블클릭 활성화를 위해 변수 업데이트
            click_time = Time.time;
        }

        if (Tp.tooltip_obj.activeSelf == true) // 슬롯의 아이템 변동 시 툴팁에 변동 내용을 반영
        {
            Tp.Hide_ToolTip();
            if (item_valid_check == true)
                Tp.Show_ToolTip(item, transform.position);
        }
    }

    public void OnBeginDrag(PointerEventData eventData)
    {
        // Item_Delete_Check.instance.delete_ui_obj.activeSelf == false << (버그방지)
        if (item_valid_check == true && Item_Delete_Check.instance.delete_ui_obj.activeSelf == false && GameManager.instance.store_open == false)
        {
            Drag_Slot.instance.drag_slot = this; // 얕은복사 (주소 참조)
            Drag_Slot.instance.Set_DragImage(item_image);
            Drag_Slot.instance.item_image.transform.position = eventData.position;

            Player_Input.use_drag = true;
        }
    }

    public void OnDrag(PointerEventData eventData)
    {
        // Item_Delete_Check.instance.delete_ui_obj.activeSelf == false << (버그방지)
        if (item_valid_check == true && Item_Delete_Check.instance.delete_ui_obj.activeSelf == false)
            Drag_Slot.instance.item_image.transform.position = eventData.position;
    }

    public void OnEndDrag(PointerEventData eventData)
    {
        if (EventSystem.current.IsPointerOverGameObject() == false && Drag_Slot.instance.drag_slot != null)
        {
            Drag_Slot.instance.Set_Color(0);
            Item_Delete_Check.instance.delete_ui_obj.gameObject.SetActive(true);
        }
        // Item_Delete_Check.instance.delete_ui_obj.activeSelf == false << (버그방지)
        else if (Item_Delete_Check.instance.delete_ui_obj.activeSelf == false)
        {
            Drag_Slot.instance.Set_Color(0);
            Drag_Slot.instance.drag_slot = null; // 초기화
        }

        Player_Input.use_drag = false;
    }

    public void OnDrop(PointerEventData eventData) // OnDrop이 순서상 OnEndDrag보다 먼저이다
    {
        // Item_Delete_Check.instance.delete_ui_obj.activeSelf == false << (버그방지)
        if (Drag_Slot.instance.drag_slot != null && Drag_Slot.instance.drag_slot.item_image.color != new Color(1, 0, 0, 1) &&
            item_image.color != new Color(1, 0, 0, 1) && Portion.instance.cooltime_check == false && Item_Delete_Check.instance.delete_ui_obj.activeSelf == false)
        {
            Slot_ChangeAndSet();

            if (Tp.tooltip_obj.activeSelf == true) // tool tip 업데이트
            {
                Tp.Hide_ToolTip();
                if (item_valid_check == true)
                    Tp.Show_ToolTip(item, transform.position);
            }
        }
    }

    public void Slot_ChangeAndSet()
    {
        Itemm tmp_item = item;
        int tmp_item_id = item_id;
        int tmp_item_count = item_count;

        if (slot_type_check.gameObject.tag == Drag_Slot.instance.drag_slot.slot_type_check.gameObject.tag)
        {
            if (this == Drag_Slot.instance.drag_slot) // 슬롯을 제자리에서 드래그앤드롭했을때 사라지는것 방지
                return;

            if (item_valid_check == true && item.item_id == Drag_Slot.instance.drag_slot.item.item_id && item.item_type != Itemm.Item_Type.Equip)
            {
                Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count + item_count);
                Drag_Slot.instance.drag_slot.Slot_Clear();
                return;
            }
            else if (this.gameObject.tag == "Slot")
                Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
            else
                return;

            if (tmp_item_count >= 1)
                Drag_Slot.instance.drag_slot.Add_Item(tmp_item, tmp_item_id, tmp_item_count);
            else
                Drag_Slot.instance.drag_slot.Slot_Clear();
        }

        else if (slot_type_check.gameObject.tag != Drag_Slot.instance.drag_slot.slot_type_check.gameObject.tag)
        {
            if (item_valid_check == true && item.equip_type == Drag_Slot.instance.drag_slot.item.equip_type && item.item_type == Itemm.Item_Type.Equip)
            { // 장비
                if (this.gameObject.tag == "Helmet" || this.gameObject.tag == "Torso" || this.gameObject.tag == "Pants" || this.gameObject.tag == "Shoes" || this.gameObject.tag == "Weapon" ||
                    this.gameObject.tag == "Glove" || this.gameObject.tag == "Cloak")
                {   //캐릭터 스텟적용
                    Player_Info.instance.Player_Equip_Off(tmp_item.item_str, tmp_item.item_dex, tmp_item.item_con, tmp_item.item_power, tmp_item.item_defense, tmp_item.item_critical);
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                }
                else
                {
                    Player_Info.instance.Player_Equip_Off(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Player_Info.instance.Player_Equip_On(tmp_item.item_str, tmp_item.item_dex, tmp_item.item_con, tmp_item.item_power, tmp_item.item_defense, tmp_item.item_critical);
                }

                Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                Drag_Slot.instance.drag_slot.Add_Item(tmp_item, tmp_item_id, tmp_item_count);
            }
            else if (item_valid_check == true && item.item_type == Drag_Slot.instance.drag_slot.item.item_type && item.item_type == Itemm.Item_Type.Use)
            { // 물약
                if (item.item_id == Drag_Slot.instance.drag_slot.item.item_id)
                { // 같은물약 합치기
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count + item_count);
                    Drag_Slot.instance.drag_slot.Slot_Clear();
                }
                else if (item.item_id != Drag_Slot.instance.drag_slot.item.item_id)
                { // 다른물약 교체
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                    Drag_Slot.instance.drag_slot.Add_Item(tmp_item, tmp_item_id, tmp_item_count);
                }
            }
            else if (item_valid_check == false)
            {
                if (this.gameObject.tag == "Helmet" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Helmet)
                {   //캐릭터 스텟적용
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Torso" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Torso)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Pants" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Pants)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Shoes" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Shoes)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Weapon" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Weapon)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Glove" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Glove)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Cloak" && Drag_Slot.instance.drag_slot.item.equip_type == Itemm.Equip_Type.Cloak)
                {
                    Player_Info.instance.Player_Equip_On(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Slot")
                {
                    Player_Info.instance.Player_Equip_Off(Drag_Slot.instance.drag_slot.item.item_str, Drag_Slot.instance.drag_slot.item.item_dex, Drag_Slot.instance.drag_slot.item.item_con, Drag_Slot.instance.drag_slot.item.item_power, Drag_Slot.instance.drag_slot.item.item_defense, Drag_Slot.instance.drag_slot.item.item_critical);
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                }
                else if (this.gameObject.tag == "Portion" && Drag_Slot.instance.drag_slot.item.item_type == Itemm.Item_Type.Use)
                    Add_Item(Drag_Slot.instance.drag_slot.item, Drag_Slot.instance.drag_slot.item_id, Drag_Slot.instance.drag_slot.item_count);
                else
                    return;

                Drag_Slot.instance.drag_slot.Slot_Clear();
            }

            // 스텟ui 업데이트
            Ui_Manager.instance.UserDetailInfo(Player_Info.instance.Player_Level, Player_Info.instance.Player_Str, Player_Info.instance.Player_Dex, 
                Player_Info.instance.Player_Con, Player_Info.instance.Power, Player_Info.instance.Defense, Player_Info.instance.Critical);
        }
    }
}
