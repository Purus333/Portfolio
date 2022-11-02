using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Npc_Inventory : MonoBehaviour
{
    public GameObject inven;

    [SerializeField]
    private GameObject slot_parnet;
    [SerializeField]
    private Slot[] slots;

    private void Awake()
    {
        inven.gameObject.SetActive(true);
        slots = slot_parnet.GetComponentsInChildren<Slot>();
        inven.gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start()
    {
        Init_Store_Inven();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            Close_Inventory();
    }

    public void Close_Inventory()
    {
        GameManager.instance.store_open = false;
        inven.gameObject.SetActive(false);
    }

    public void Init_Store_Inven()
    {
        if (this.gameObject.name == "Npc_Inventory(store)")
        {
            slots[0].Add_Item(GameManager.instance.item_list[24], GameManager.instance.item_list[24].item_id);
            slots[1].Add_Item(GameManager.instance.item_list[25], GameManager.instance.item_list[25].item_id);
            slots[2].Add_Item(GameManager.instance.item_list[26], GameManager.instance.item_list[26].item_id);
            slots[3].Add_Item(GameManager.instance.item_list[27], GameManager.instance.item_list[27].item_id);
            slots[4].Add_Item(GameManager.instance.item_list[28], GameManager.instance.item_list[28].item_id);
        }
    }

    public void Change_Store_Weapon()
    {
        if (this.gameObject.name == "Npc_Inventory(store)")
        {
            for (int i = 0; i < slots.Length; i++)
                if (slots[i].item_valid_check == true)
                    slots[i].Slot_Clear();

            slots[0].Add_Item(GameManager.instance.item_list[24], GameManager.instance.item_list[24].item_id);
            slots[1].Add_Item(GameManager.instance.item_list[25], GameManager.instance.item_list[25].item_id);
            slots[2].Add_Item(GameManager.instance.item_list[26], GameManager.instance.item_list[26].item_id);
            slots[3].Add_Item(GameManager.instance.item_list[27], GameManager.instance.item_list[27].item_id);
            slots[4].Add_Item(GameManager.instance.item_list[28], GameManager.instance.item_list[28].item_id);
        }
    }

    public void Change_Store_Equip()
    {
        if (this.gameObject.name == "Npc_Inventory(store)")
        {
            for (int i = 0; i < slots.Length; i++)
                if (slots[i].item_valid_check == true)
                    slots[i].Slot_Clear();

            int num = 0;

            while (num != 24)
            {
                slots[num].Add_Item(GameManager.instance.item_list[num], GameManager.instance.item_list[num].item_id);

                num++;
            }
        }
    }

    public void Change_Store_Consume()
    {
        if (this.gameObject.name == "Npc_Inventory(store)")
        {
            for (int i = 0; i < slots.Length; i++)
                if (slots[i].item_valid_check == true)
                    slots[i].Slot_Clear();

            slots[0].Add_Item(GameManager.instance.item_list[29], GameManager.instance.item_list[29].item_id);
            slots[1].Add_Item(GameManager.instance.item_list[30], GameManager.instance.item_list[30].item_id);
            slots[2].Add_Item(GameManager.instance.item_list[31], GameManager.instance.item_list[31].item_id);
        }
    }
}
