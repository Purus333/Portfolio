using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Ui_Manager : MonoBehaviour
{
    private static Ui_Manager m_instance;

    public static Ui_Manager instance
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Ui_Manager>();
            }

            return m_instance;
        }
    }

    public Text usernameTxt;
    public Text userhpTxt;
    public Text userlvTxt;
    public Text userexpTxt;
    public Text userdetailTxt;
    public Text usergoldTxt;
    public Text[] userskillcoolTxt; // 직접지정
    public Text map_name_fadeTxt;
    public Text inven_notice_Txt;
    public GameObject item_noitce_parnet;
    public Item_Notice[] item_noice_obj;
    public Text levelup_head_Txt;
    public Text levelup_middle_Txt;
    public Text die_notice_headTxt;
    public Text die_notice_middleTxt;
    public Text gold_get_noticeTxt;
    public Text[] quest_titleTxt;
    public Text[] quest_detailTxt;
    public Text save_doneTxt;

    public Image loading_image;
    public bool loading_check { get; private set; }
    private float times = 0;
    private bool map_fade_trigger;
    public bool showmap_trigger;

    private float inven_timer = 0;
    private float inven_wait_time = 2.0f;
    public int notice_num = 0;

    private float levelup_timer1 = 0;
    private float levelup_wait_time1 = 1.5f;
    private float levelup_timer2 = 0;
    private float levelup_wait_time2 = 2.0f;

    private float gold_timer = 0;
    private float gold_wait_time = 1.5f;

    private float save_timer = 0;
    private float save_wait_time = 2.0f;

    private void Awake()
    {
        usernameTxt = usernameTxt.GetComponent<Text>();
        userhpTxt = userhpTxt.GetComponent<Text>();
        userlvTxt = userlvTxt.GetComponent<Text>();
        userexpTxt = userexpTxt.GetComponent<Text>();
        userdetailTxt = userdetailTxt.GetComponent<Text>();
        usergoldTxt = usergoldTxt.GetComponent<Text>();
        map_name_fadeTxt = map_name_fadeTxt.GetComponent<Text>();
        inven_notice_Txt = inven_notice_Txt.GetComponent<Text>();
        item_noice_obj = item_noitce_parnet.GetComponentsInChildren<Item_Notice>();
        levelup_head_Txt = levelup_head_Txt.GetComponent<Text>();
        levelup_middle_Txt = levelup_middle_Txt.GetComponent<Text>();
        die_notice_headTxt = die_notice_headTxt.GetComponent<Text>();
        die_notice_middleTxt = die_notice_middleTxt.GetComponent<Text>();
        gold_get_noticeTxt = gold_get_noticeTxt.GetComponent<Text>();

        loading_check = false;
        loading_image = loading_image.GetComponent<Image>();
        loading_image.gameObject.SetActive(false);

        map_fade_trigger = false;
        showmap_trigger = false; // 뉴게임, 로드게임시 상단에 맵이름 보여주는 변수
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Player_Info.instance.player_scene_move_check == true) // 맵이동시 fade 효과
            UserMapMove();
        else if (Player_Info.instance.player_scene_move_check == false && map_fade_trigger == true ||
            showmap_trigger == true || GameManager.instance.load_done == true) // 씬이동후 fade 효과 종료뒤, 뉴게임 or 로드게임 or 맵이 바뀌었을때 상단에 맵이름 표기
            MapNameFadeShow(Player_Info.instance.player_map_index);

        if (inven_notice_Txt.gameObject.activeSelf == true)
            Inven_Timer();
        if (levelup_head_Txt.gameObject.activeSelf == true)
            LevelUp_Timer1();
        if (levelup_middle_Txt.gameObject.activeSelf == true)
            LevelUp_Timer2();
        if (gold_get_noticeTxt.gameObject.activeSelf == true)
            Gold_Timer();
        if (save_doneTxt.gameObject.activeSelf == true)
            Save_Done_Notice_Timer();
    }

    public void UserNameUi(string name)
    {
        usernameTxt.gameObject.SetActive(true);
        usernameTxt.text = name;
    }

    public void UserHpInfo(float max_hp, float cur_hp)
    {
        userhpTxt.gameObject.SetActive(true);
        userhpTxt.text = max_hp + "/" + cur_hp;
    }

    public void UserLvInfo(int lv)
    {
        userlvTxt.gameObject.SetActive(true);
        userlvTxt.text = "레벨:" + lv;
    }

    public void UserExpInfo(float cur_exp, float max_exp)
    {
        userexpTxt.gameObject.SetActive(true);
        userexpTxt.text = cur_exp + "/" + max_exp;
    }

    public void UserDetailInfo(int lv, int str, int dex, int con, float power, float defense, float cri)
    {
        userdetailTxt.gameObject.SetActive(true);
        userdetailTxt.text = "레벨 : " + lv + "\n\n힘 : " + str + "\n덱스 : " + dex + "\n콘 : " + con +
            "\n공격력 : " + power + "\n방어력 : " + defense + "\n치명타율 : " + cri;
    }

    public void UserGoldInfo(int money)
    {
        usergoldTxt.gameObject.SetActive(true);
        usergoldTxt.text = money + " Gold";
    }

    public void UserSkill_CoolTime(int num, int time)
    {
        if (time == 0)
            userskillcoolTxt[num].gameObject.SetActive(false);
        else
        {
            userskillcoolTxt[num].gameObject.SetActive(true);
            userskillcoolTxt[num].text = "" + time;
        }
    }

    public void Item_Get_Notice(Itemm _item)
    {
        if (notice_num == 4)
            notice_num = 0;

        item_noice_obj[notice_num].gameObject.SetActive(true);
        item_noice_obj[notice_num].transform.SetSiblingIndex(0); // 제일최근것을 0번째에 표시한다
        item_noice_obj[notice_num].text_on = true;
        item_noice_obj[notice_num].notice_timer = 0;
        item_noice_obj[notice_num].notice_txt.text = _item.item_name + "을(를) 획득하였습니다";

        notice_num++;
    }
    
    public void Gold_Get_Notice(int _value)
    {
        gold_get_noticeTxt.gameObject.SetActive(true);
        gold_get_noticeTxt.text = "+" + _value + " Gold";
        gold_timer = 0;
    }

    public void Gold_Timer()
    {
        if (gold_wait_time > gold_timer)
        {
            gold_timer += Time.deltaTime;
            return;
        }
        else
        {
            gold_get_noticeTxt.gameObject.SetActive(false);
            gold_timer = 0;
        }
    }

    public void Inven_Full_Notice()
    {
        inven_notice_Txt.gameObject.SetActive(true);
        inven_timer = 0;
    }

    public void Inven_Timer()
    {
        if (inven_wait_time > inven_timer)
        {
            inven_timer += Time.deltaTime;
            return;
        }
        else
        {
            inven_notice_Txt.gameObject.SetActive(false);
            inven_timer = 0;
        }
    }

    public void LevelUp_Notice_Head()
    {
        levelup_head_Txt.gameObject.SetActive(true);
        levelup_timer1 = 0;
    }

    public void LevelUp_Notice_Middle()
    {
        levelup_middle_Txt.gameObject.SetActive(true);
        levelup_timer2 = 0;
    }

    public void LevelUp_Timer1()
    {
        if (levelup_wait_time1 > levelup_timer1)
        {
            levelup_timer1 += Time.deltaTime;
            return;
        }
        else
        {
            levelup_head_Txt.gameObject.SetActive(false);
            levelup_timer1 = 0;
        }
    }

    public void LevelUp_Timer2()
    {
        if (levelup_wait_time2 > levelup_timer2)
        {
            levelup_timer2 += Time.deltaTime;
            return;
        }
        else
        {
            levelup_middle_Txt.gameObject.SetActive(false);
            levelup_timer2 = 0;
        }
    }

    public void Die_Notice_OnOff()
    {
        if (die_notice_headTxt.gameObject.activeSelf == false)
            die_notice_headTxt.gameObject.SetActive(true);
        else if (die_notice_headTxt.gameObject.activeSelf == true)
            die_notice_headTxt.gameObject.SetActive(false);

        if (die_notice_middleTxt.gameObject.activeSelf == false)
            die_notice_middleTxt.gameObject.SetActive(true);
        else if (die_notice_middleTxt.gameObject.activeSelf == true)
            die_notice_middleTxt.gameObject.SetActive(false);
    }

    public void Quest_Info(int quest_num, int count)
    {
        if (quest_num == 0)
        {
            quest_titleTxt[0].gameObject.SetActive(true);
            quest_detailTxt[0].gameObject.SetActive(true);
            quest_detailTxt[0].text = count + " / 3";
        }
        else if (quest_num == 1)
        {
            quest_titleTxt[1].gameObject.SetActive(true);
            quest_detailTxt[1].gameObject.SetActive(true);
            quest_detailTxt[1].text = count + " / 15";
        }
    }

    public void MapNameFadeShow(int check_map_index)
    {
        if (showmap_trigger == true)
        {
            map_fade_trigger = true;
            map_name_fadeTxt.gameObject.SetActive(true);
            Color color = map_name_fadeTxt.color;
            color.a = 1; // 실제입력값과 틀리다 (0.5f 는127.5이다 255 * 0.5)
            map_name_fadeTxt.color = color;

            showmap_trigger = false;
        }

        if (check_map_index == 1)
            map_name_fadeTxt.text = "조용한 숲";
        else if (check_map_index == 2)
            map_name_fadeTxt.text = "칼리스마을 외곽 숲";
        else if (check_map_index == 3)
            map_name_fadeTxt.text = "칼리스마을";
        else if (check_map_index == 4)
            map_name_fadeTxt.text = "진흙길 숲";
        else if (check_map_index == 5)
            map_name_fadeTxt.text = "깊은 바위 숲";

        StartCoroutine("FadeMapName");
    }

    public void UserMapMove() // 유저가 맵이동후 잠시 fade 효과의 ui
    {
        map_name_fadeTxt.gameObject.SetActive(false);
        loading_image.gameObject.SetActive(true);
        StartCoroutine("Load");
        if (loading_check == true)
        {
            loading_image.gameObject.SetActive(false);
            loading_check = false;
            Player_Info.instance.player_scene_move_check = false;
            times = 0; // 싱글톤이기 때문에 변수초기화 필수

            map_fade_trigger = true;
            map_name_fadeTxt.gameObject.SetActive(true);
            Color color = map_name_fadeTxt.color;
            color.a = 1; // 실제입력값과 틀리다
            map_name_fadeTxt.color = color;
        }
    }

    public void Save_Done_Notice()
    {
        save_doneTxt.gameObject.SetActive(true);
        save_timer = 0;
    }

    public void Save_Done_Notice_Timer()
    {
        if (save_wait_time > save_timer)
        {
            save_timer += Time.deltaTime;
            return;
        }
        else
        {
            save_doneTxt.gameObject.SetActive(false);
            save_timer = 0;
        }
    }

    IEnumerator Load() // 포탈이동후 사용
    {
        times += 10.0f * Time.deltaTime;

        if (times >= 20)
            loading_check = true;

        yield return null;
    }

    IEnumerator FadeMapName()
    {
        Color color = map_name_fadeTxt.color;

        for (int i = 20; i >= 0; i--)
        {
            color.a -= Time.deltaTime * 0.01f;

            map_name_fadeTxt.color = color;

            if (map_name_fadeTxt.color.a <= 0)
            {
                map_fade_trigger = false;
                map_name_fadeTxt.gameObject.SetActive(false);
            }
        }
        yield return null;
    }
}
