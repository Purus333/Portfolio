using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Item_Delete_Check : MonoBehaviour
{
    private static Item_Delete_Check m_instance;
    
    public static Item_Delete_Check instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Item_Delete_Check>();
            }
            return m_instance;
        }
    }

    public GameObject delete_ui_obj;
    public bool delete_item_check;

    private void Awake()
    {
        delete_item_check = false;
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (delete_item_check == true)
        {
            Drag_Slot.instance.drag_slot.Slot_Clear();
            Drag_Slot.instance.drag_slot = null;
            delete_item_check = false;
        }

        if (delete_ui_obj.activeSelf == true)
        {
            if (Input.GetKeyDown(KeyCode.Return))
                Delete_Ok();
            else if (Input.GetKeyDown(KeyCode.Escape))
                Delete_Cancel();
        }
    }

    public void Delete_Ok()
    {
        delete_item_check = true;
        delete_ui_obj.gameObject.SetActive(false);
    }

    public void Delete_Cancel()
    {
        delete_item_check = false;
        delete_ui_obj.gameObject.SetActive(false);
    }
}
