using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class NewStart_Loading_Screen : MonoBehaviour
{
    private static NewStart_Loading_Screen m_instance; // 싱글톤이 할당될 static 변수
    // 싱글톤 접근용 프로퍼티
    public static NewStart_Loading_Screen instance
    {
        get
        {
            // 만약 싱글톤 변수에 아직 오브젝트가 할당되지 않았다면
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<NewStart_Loading_Screen>();
            }

            // 싱글톤 오브젝트를 반환
            return m_instance;
        }
    }

    public Image loading_image;
    public Image fade_image;
    public Image text_image;
    public Image Input_image;
    public Image loadgame_loading_image;
    public Button next_button;
    public bool loading_check { get; private set; }
    private bool fade_start = false;
    private float times = 0;
    public bool game_start_check { get; private set; }
    private bool name_input;
    public bool name_input_check;

    public GameObject cam1, cam2;
    public GameObject minimap_cam;
    public GameObject player;
    public GameObject ui_canvas;

    private float timer;
    private int wait_timer;
    private float load_timer;
    private int wait_load_time;

    private bool donot_active; // 새게임, 로드시작후 프롤로그맵에 진입시 재시작금지를 위한 변수

    private void Awake()
    {
        if (instance != this)
        {
            // 중복일 경우 자신을 파괴
            Destroy(gameObject);
        }

        loading_check = false;
        name_input = false;
        name_input_check = false;
        timer = 0;
        wait_timer = 2;
        load_timer = 0;
        wait_load_time = 4;

        fade_image = fade_image.GetComponent<Image>();
        loading_image = loading_image.GetComponent<Image>();
        text_image = text_image.GetComponent<Image>();
        Input_image = Input_image.GetComponent<Image>();

        text_image.gameObject.SetActive(false);
        Input_image.gameObject.SetActive(false);
        next_button = next_button.GetComponent<Button>();
        next_button.gameObject.SetActive(false);

        //cam1 = GameObject.FindWithTag("FCam"); // public으로 관리하기
        //cam2 = GameObject.FindWithTag("FCam2");
        cam1.gameObject.SetActive(false);
        cam2.gameObject.SetActive(false);
        minimap_cam.gameObject.SetActive(false);
        player.gameObject.SetActive(true);

        GameManager.instance.main_menu_select = true; // new게임 선택함

        donot_active = false;
    }

    void Start()
    {
        if (GameManager.instance.scene_load == false)
            StartCoroutine("Start_Scene_Setting");

        if (GameManager.instance.newgame_started == true)
        {
            donot_active = true;
            fade_image.gameObject.SetActive(false);
            loading_image.gameObject.SetActive(false);
            player.gameObject.SetActive(false);
        }

        if (GameManager.instance.load_data == true && GameManager.instance.newgame_started == false)
            loadgame_loading_image.gameObject.SetActive(true);
    }

    void Update()
    {
        if (donot_active == false) // wood01 맵에서 돌아올때 다시안켜지게 하기위함
        {
            if (loadgame_loading_image.gameObject.activeSelf == true)
                Load_Game_Loading();

            if (fade_start == false && game_start_check == false)
                Loading();
            else if (fade_start == true && game_start_check == false)
                Start_Fade();
            else if (game_start_check == true && name_input == false)
                Input_Name();
        }
    }

    private void Start_Fade()
    {
        StartCoroutine("Fade");
        if (loading_check == true)
        {
            fade_image.gameObject.SetActive(false);
            text_image.gameObject.SetActive(true);
            next_button.gameObject.SetActive(true);

            if (GameManager.instance.load_data == true) // load 게임
                TextManager.instance.dialog_end = true;
        }
        if (loading_check == true && TextManager.instance.dialog_end == true)
        {
            text_image.gameObject.SetActive(false);
            next_button.gameObject.SetActive(false);
            TextManager.instance.Skip_txt.gameObject.SetActive(false);

            game_start_check = true;

            cam1.gameObject.SetActive(true);
            minimap_cam.gameObject.SetActive(true);
        }
    }


    private void Loading()
    {
        cam2.gameObject.SetActive(true);

        times += 10.0f * Time.deltaTime;

        if (times >= 30 || times >= 5 && GameManager.instance.load_data == true)
        {
            loading_image.gameObject.SetActive(false);
            fade_start = true;
        }
    }

    private void Input_Name()
    {
        timer += Time.deltaTime;

        if (timer >= wait_timer)
        {
            Input_image.gameObject.SetActive(true);
            name_input = true;
            timer = 0;

            Player_Info.instance.player_newgame_ani_trigger = true;
        }
    }

    IEnumerator Fade()
    {
        Color color = fade_image.color;

        for (int i = 20; i >= 0; i--)
        {
            color.a -= Time.deltaTime * 0.01f;

            fade_image.color = color;

            if (fade_image.color.a <= 0 || GameManager.instance.load_data == true)
                loading_check = true;
        }

        yield return null;
    }

    IEnumerator Start_Scene_Setting()
    {
        yield return new WaitForSeconds(0.75f);

        Scene scene = SceneManager.GetSceneByBuildIndex(1);

        SceneManager.SetActiveScene(scene);
        SceneManager.UnloadSceneAsync(0);

        GameManager.instance.scene_load = true;
    }

    public void Load_Game_Loading()
    {
        if (wait_load_time > load_timer)
        {
            load_timer += Time.deltaTime;
            return;
        }
        else
        {
            loadgame_loading_image.gameObject.SetActive(false);
            load_timer = 0;
        }
    }
}
