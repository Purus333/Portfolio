using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Drag_Slot : MonoBehaviour
{
    private static Drag_Slot m_instance;

    public static Drag_Slot instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Drag_Slot>();
            }
            return m_instance;
        }
    }
    [SerializeField]
    public Slot drag_slot;
    public Image item_image;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Set_DragImage(Image _item_image)
    {
        item_image.sprite = _item_image.sprite;
        Set_Color(1);
    }

    public void Set_Color(float _alpha_value)
    {
        Color color = item_image.color;
        color.a = _alpha_value;
        item_image.color = color;
    }

}

