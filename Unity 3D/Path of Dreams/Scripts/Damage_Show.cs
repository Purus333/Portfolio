using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Damage_Show : MonoBehaviour
{
    private float move_speed;
    public TextMeshPro dam_txt;
    public float show_damage;

    private void Awake()
    {
        move_speed = 2.0f;
    }

    // Start is called before the first frame update
    void Start()
    {
        dam_txt = GetComponent<TextMeshPro>();
        dam_txt.text = "" + show_damage;
        Invoke("Off", 2.0f);
    }

    // Update is called once per frame
    void Update()
    {
        this.transform.Translate(new Vector3(0, move_speed * Time.deltaTime, 0)); // 텍스트 위치
    }

    public void DestroyObject()
    {
        Destroy(this.gameObject);
    }

    public void On()
    {
        dam_txt.text = "" + show_damage;
        this.gameObject.SetActive(true);
        Invoke("Off", 2.0f);
    }

    public void Off()
    {
        this.gameObject.SetActive(false);
    }
}
