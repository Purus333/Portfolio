using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Drag_Canvas : MonoBehaviour, IPointerDownHandler, IDragHandler
{
    private Vector2 pointer_pos;
    [SerializeField]
    private RectTransform ui_rt;

    private void Awake()
    {

    }

    void Start()
    {

    }

    void Update()
    {

    }

    public void OnPointerDown(PointerEventData p_data)
    {
        pointer_pos = p_data.position;
    }

    public void OnDrag(PointerEventData data)
    {
        Vector2 offset = data.position - pointer_pos;
        pointer_pos = data.position;

        ui_rt.anchoredPosition += offset;
    }
}
