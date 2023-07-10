using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Portion : MonoBehaviour
{
    private static Portion m_instance;

    public static Portion instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Portion>();
            }
            return m_instance;
        }
    }

    [SerializeField]
    private GameObject slot_parnet;
    public Slot p_slot;
    public Image cooltime_image;
    public bool cooltime_check = false;
    public ToolTip Tp;

    private void Awake()
    {
        p_slot = slot_parnet.GetComponentInChildren<Slot>();
        cooltime_image = cooltime_image.GetComponent<Image>();
        cooltime_image.gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (cooltime_check == true)
            StartCoroutine("CoolTime");

        Use_Portion();
    }

    public void Use_Portion()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            if (p_slot.item_valid_check == true)
            {
                bool value = false;

                value = Inventory_Manager.instance.Use_Item(p_slot.item, p_slot.item_id);

                if (value == true)
                    p_slot.Slot_Count(-1);

                
                if (Tp.tooltip_obj.activeSelf == true) // 슬롯의 아이템 변동 시 툴팁에 변동 내용을 반영하기위함
                {
                    Tp.Hide_ToolTip();
                    if (p_slot.item_valid_check == true)
                        Tp.Show_ToolTip(p_slot.item, transform.position);
                }
            }
        }
    }

    IEnumerator CoolTime()
    {
        cooltime_image.gameObject.SetActive(true);

        Color color = cooltime_image.color;

        for (int i = 10; i >= 0; i--)
        {
            color.a -= Time.deltaTime * 0.01f;

            cooltime_image.color = color;

            if (cooltime_image.color.a <= 0)
            {
                cooltime_check = false;
                color.a = 0.5f;
                cooltime_image.color = color;
                cooltime_image.gameObject.SetActive(false);
                StopCoroutine("CoolTime");
            }
        }

        yield return null;
    }

    public void Load_Quick_Item(string _item_name, int _item_count)
    {
        for (int i = 0; i < GameManager.instance.item_list.Count; i++)
        {
            if (GameManager.instance.item_list[i].item_name == _item_name)
                p_slot.Add_Item(GameManager.instance.item_list[i], GameManager.instance.item_list[i].item_id, _item_count);
        }
    }
}
