using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    private static GameManager m_instance;
    
    public static GameManager instance // 접근용 프로퍼티
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<GameManager>();
            }
            return m_instance;
        }
    }

    public bool newgame_started;
    public bool load_data;
    public bool load_done;
    public bool main_menu_select;
    public bool scene_move_check;
    public GameObject player;
    public GameObject minimap_cam;
    public SaveAndLoad g_SaveandLoad;
    public bool npc_text_start;
    public bool store_open;
    public bool scene_load;
    public bool scene_move;
    private int temp_map_index;

    [SerializeField]
    public List<Itemm> item_list = new List<Itemm>();

    private void Awake()
    {
        if (instance != this)
        {
            // 중복일 경우 자신을 파괴
            Destroy(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        newgame_started = false;
        load_data = false;
        load_done = false;
        main_menu_select = false;
        scene_move_check = false;
        npc_text_start = false;
        store_open = false;
        scene_load = false;
        scene_move = false;
        temp_map_index = 0;

        Item_Init(); // 아이템 초기화
    }

    // Update is called once per frame
    void Update()
    {
        if (main_menu_select == true)
        {
            if (newgame_started == false)
                NewGame();
            else if (scene_move_check == true)
                SceneMove();
        }
    }

    private void NewGame()
    {
        if (NewStart_Loading_Screen.instance.game_start_check == true && NewStart_Loading_Screen.instance.name_input_check == true)
        {
            newgame_started = true;
            player = GameObject.FindWithTag("Player");
            minimap_cam = GameObject.FindWithTag("Minimap");
            Ui_Manager.instance.showmap_trigger = true;

            if (load_data == true)
            {
                if (Player_Info.instance.player_map_index == 1)
                {
                    player.transform.position = new Vector3(45, 2.66f, 56.4f);
                    minimap_cam.transform.position = new Vector3(45, 17.36f, 56.4f);

                    load_done = true;
                }
                else if (Player_Info.instance.player_map_index == 2 || Player_Info.instance.player_map_index == 3 || Player_Info.instance.player_map_index == 4)
                {
                    temp_map_index = Player_Info.instance.player_map_index;

                    player.transform.position = new Vector3(36.8f, 8, 37.5f);
                    minimap_cam.transform.position = new Vector3(36.8f, 17.36f, 37.5f);
                }
            }
        }
    }

    private void SceneMove()
    {
        if (Player_Info.instance.player_map_index == 1)
        {
            StartCoroutine("Scene_Setting_FirstMap");

            player.transform.position = new Vector3(39, 8, 38);
            minimap_cam.transform.position = new Vector3(39, 17.36f, 38);
        }
        else if (Player_Info.instance.player_map_index == 2)
        {
            StartCoroutine("Scene_Setting_WoodMap");

            player.transform.position = new Vector3(58, 0, 20);
            minimap_cam.transform.position = new Vector3(58, 17.36f, 20);
        }
        
        if (load_data == true && load_done == false) // load 시 한번만 실행
        {
            Player_Info.instance.player_map_index = temp_map_index;

            if (Player_Info.instance.player_map_index == 2)
            {
                player.transform.position = new Vector3(39.78f, 0.1f, 54.38f);
                minimap_cam.transform.position = new Vector3(39.78f, 17.36f, 54.38f);
            }
            else if (Player_Info.instance.player_map_index == 3 || Player_Info.instance.player_map_index == 4)
            {
                player.transform.position = new Vector3(-25.5f, 0.1f, 49.58f);
                minimap_cam.transform.position = new Vector3(-25.5f, 17.36f, 49.58f);
                Player_Info.instance.player_map_index = 3;
            }

            load_done = true;
        }

        scene_move_check = false;
    }

    public void Item_Init()
    {
        // item 번호, 이름, 설명, 타입, 장비타입(장비아닐시 0), 힘, 덱스, 콘, 공격력, 방어력, 크리티컬확율, 가격
        item_list.Add(new Itemm(1001, "가죽 상의", "평범한 가죽으로 만든 상의", Itemm.Item_Type.Equip , Itemm.Equip_Type.Torso, 2, 2, 2, 0, 5, 0, 300));
        item_list.Add(new Itemm(1002, "가죽 망토", "평범한 가죽으로 만든 망토", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1003, "가죽 투구", "평범한 가죽으로 만든 투구", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1004, "가죽 장갑", "평범한 가죽으로 만든 장갑", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1005, "가죽 신발", "평범한 가죽으로 만든 신발", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1006, "가죽 하의", "평범한 가죽으로 만든 하의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 2, 2, 2, 0, 5, 0, 300));
        item_list.Add(new Itemm(1007, "플레이트 상의", "이름모를 장인이 철을 다듬어 만든 상의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 5, 5, 5, 0, 12, 0, 700));
        item_list.Add(new Itemm(1008, "플레이트 망토", "이름모를 장인이 철을 다듬어 만든 망토", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1009, "플레이트 투구", "이름모를 장인이 철을 다듬어 만든 투구", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1010, "플레이트 장갑", "이름모를 장인이 철을 다듬어 만든 장갑", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1011, "플레이트 신발", "이름모를 장인이 철을 다듬어 만든 신발", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1012, "플레이트 하의", "이름모를 장인이 철을 다듬어 만든 하의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 5, 5, 5, 0, 12, 0, 700));
        item_list.Add(new Itemm(1013, "몽환의 상의", "꿈속에서 찾아낸 보물의 상의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 7, 7, 7, 0, 20, 0, 1500));
        item_list.Add(new Itemm(1014, "몽환의 망토", "꿈속에서 찾아낸 보물의 망토", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1015, "몽환의 투구", "꿈속에서 찾아낸 보물의 투구", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1016, "몽환의 장갑", "꿈속에서 찾아낸 보물의 장갑", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1017, "몽환의 신발", "꿈속에서 찾아낸 보물의 신발", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1018, "몽환의 하의", "꿈속에서 찾아낸 보물의 하의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 7, 7, 7, 0, 20, 0, 1500));
        item_list.Add(new Itemm(1019, "전설의 플레이트 상의", "전설의 용사만이 착용할수있는 상의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 10, 10, 10, 0, 35, 0, 5000));
        item_list.Add(new Itemm(1020, "전설의 플레이트 망토", "전설의 용사만이 착용할수있는 망토", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1021, "전설의 플레이트 투구", "전설의 용사만이 착용할수있는 투구", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1022, "전설의 플레이트 장갑", "전설의 용사만이 착용할수있는 장갑", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1023, "전설의 플레이트 신발", "전설의 용사만이 착용할수있는 신발", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1024, "전설의 플레이트 하의", "전설의 용사만이 착용할수있는 하의", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 10, 10, 10, 0, 35, 0, 5000));
        item_list.Add(new Itemm(2001, "브론즈 롱소드", "청동으로 만든 롱소드", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 10, 0, 2, 350));
        item_list.Add(new Itemm(2002, "실버 롱소드", "은으로 만든 롱소드", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 25, 0, 4, 800));
        item_list.Add(new Itemm(2003, "강인한 롱소드", "소문으로만 듣던 강인한 검", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 38, 0, 5, 2000));
        item_list.Add(new Itemm(2004, "각성의 롱소드", "무기의 장인들이 모여서 만든 검", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 60, 0, 7, 7000));
        item_list.Add(new Itemm(2005, "환각의 롱소드", "꿈에서나 볼 수 있는 전설의 검", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 99, 0, 10, 11000));
        item_list.Add(new Itemm(3001, "체력포션", "소량의 체력을 회복시켜준다", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(3002, "고급 체력포션", "중간의 체력을 회복시켜준다", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(3003, "최고급 체력포션", "대량의 체력을 회복시켜준다", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 350));
        item_list.Add(new Itemm(4001, "평범한 가죽", "어디서나 볼 수 있는 평범한 가죽", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(4002, "붉은 꽃", "숲속에서 발견 할 수 있는 붉은빛 꽃", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4003, "푸른 꽃", "숲속에서 발견 할 수 있는 푸른빛 꽃", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 250));
        item_list.Add(new Itemm(4004, "구리 원석", "평범한 구리 원석", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(4005, "철 원석", "평범한 철 원석", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4006, "평범한 고기덩이", "평범해 보이는 고기덩이", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 50));
        item_list.Add(new Itemm(4007, "희귀한 고기덩이", "희귀해 보이는 고기덩이", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 150));
        item_list.Add(new Itemm(4008, "붉은 천", "붉은빛이 도는 천", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4009, "푸른 천", "푸른빛이 도는 천", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 250));
    }

    IEnumerator Scene_Setting_FirstMap()
    {
        yield return new WaitForSeconds(0.75f);

        Scene scene = SceneManager.GetSceneByBuildIndex(1);

        SceneManager.SetActiveScene(scene);
        SceneManager.UnloadSceneAsync(2);

        scene_move = false;
    }

    IEnumerator Scene_Setting_WoodMap()
    {
        yield return new WaitForSeconds(0.75f);

        Scene scene = SceneManager.GetSceneByBuildIndex(2);

        SceneManager.SetActiveScene(scene);
        SceneManager.UnloadSceneAsync(1);

        scene_move = false;
    }
}
