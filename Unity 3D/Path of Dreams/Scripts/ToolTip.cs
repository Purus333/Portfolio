using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ToolTip : MonoBehaviour
{
    public GameObject tooltip_obj;

    [SerializeField]
    private Text txt_ItemName;
    [SerializeField]
    private Text txt_ItemDesc;
    [SerializeField]
    private Text txt_ItemStateDesc;
    [SerializeField]
    private Text txt_ItemPrice;

    private void Awake()
    {
        
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (!EventSystem.current.IsPointerOverGameObject()) // ui ��Ȱ���� ���� ��Ȱ�� �Ѵ�
            Hide_ToolTip();
    }

    public void Show_ToolTip(Itemm _item, Vector3 _pos)
    {
        tooltip_obj.gameObject.SetActive(true);

        tooltip_obj.transform.position = _pos;

        txt_ItemName.text = _item.item_name;
        txt_ItemDesc.text = _item.item_desc;

        if (_item.item_type == Itemm.Item_Type.Equip)
            txt_ItemStateDesc.text = "�� " + _item.item_str + " ���� " + _item.item_dex + " �� " + _item.item_con + " ���ݷ� " + _item.item_power + " ���� " + _item.item_defense + " ġ��ŸȮ�� " + _item.item_critical;
        else
            txt_ItemStateDesc.text = "";

        txt_ItemPrice.text = _item.item_price + " Gold";
    }

    public void Hide_ToolTip()
    {
        tooltip_obj.gameObject.SetActive(false);
    }
}
