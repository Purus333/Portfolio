using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Item_Notice : MonoBehaviour
{
    public Text notice_txt;
    public bool text_on;
    public float notice_timer;
    public float notice_wait_time;

    private void Awake()
    {
        // ui 캔버스에서 할당하기 위해 시작시 디폴트는 활성화 상태임
        text_on = false;
        notice_timer = 0;
        notice_wait_time = 3.0f;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (this.gameObject.activeSelf == true && text_on == true)
            Timer();
    }

    public void Timer()
    {
        if (notice_wait_time > notice_timer)
        {
            notice_timer += Time.deltaTime;
            return;
        }
        else
        {
            this.gameObject.SetActive(false);
            text_on = false;
            notice_timer = 0;
        }
    }
}
