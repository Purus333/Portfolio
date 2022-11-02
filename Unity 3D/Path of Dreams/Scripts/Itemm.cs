using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[System.Serializable]
public class Itemm : MonoBehaviour
{
    public enum Item_Type
    {
        Use,
        Equip,
        Etc
    }

    public enum Equip_Type
    {
        Helmet = 100,
        Torso,
        Pants,
        Shoes,
        Weapon,
        Glove,
        Cloak,
    }

    public Sprite item_image;
    public Item_Type item_type;
    public Equip_Type equip_type;

    public int item_id;
    public string item_name;
    public string item_desc;
    public int item_count;

    public int item_str;
    public int item_dex;
    public int item_con;
    public int item_power;
    public int item_defense;
    public int item_critical;
    public int item_price;

    public Itemm(int _item_id, string _item_name, string _item_desc, Item_Type _item_type, Equip_Type _equip_type, 
        int _item_str, int _item_dex, int _item_con, int _item_power, int _item_defense, int _item_critical, int _item_price, int _item_count = 1)
    {
        item_id = _item_id;
        item_name = _item_name;
        item_desc = _item_desc;
        item_type = _item_type;
        equip_type = _equip_type;
        item_str = _item_str;
        item_dex = _item_dex;
        item_con = _item_con;
        item_power = _item_power;
        item_defense = _item_defense;
        item_critical = _item_critical;
        item_price = _item_price;
        item_count = _item_count;
        item_image = Resources.Load("Item_Icon/" + _item_id.ToString(), typeof(Sprite)) as Sprite;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
