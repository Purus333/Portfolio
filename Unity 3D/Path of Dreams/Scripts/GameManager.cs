using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    private static GameManager m_instance;
    
    public static GameManager instance // ���ٿ� ������Ƽ
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
            // �ߺ��� ��� �ڽ��� �ı�
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

        Item_Init(); // ������ �ʱ�ȭ
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
        
        if (load_data == true && load_done == false) // load �� �ѹ��� ����
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
        // item ��ȣ, �̸�, ����, Ÿ��, ���Ÿ��(���ƴҽ� 0), ��, ����, ��, ���ݷ�, ����, ũ��Ƽ��Ȯ��, ����
        item_list.Add(new Itemm(1001, "���� ����", "����� �������� ���� ����", Itemm.Item_Type.Equip , Itemm.Equip_Type.Torso, 2, 2, 2, 0, 5, 0, 300));
        item_list.Add(new Itemm(1002, "���� ����", "����� �������� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1003, "���� ����", "����� �������� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1004, "���� �尩", "����� �������� ���� �尩", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1005, "���� �Ź�", "����� �������� ���� �Ź�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 2, 2, 2, 0, 4, 0, 300));
        item_list.Add(new Itemm(1006, "���� ����", "����� �������� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 2, 2, 2, 0, 5, 0, 300));
        item_list.Add(new Itemm(1007, "�÷���Ʈ ����", "�̸��� ������ ö�� �ٵ�� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 5, 5, 5, 0, 12, 0, 700));
        item_list.Add(new Itemm(1008, "�÷���Ʈ ����", "�̸��� ������ ö�� �ٵ�� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1009, "�÷���Ʈ ����", "�̸��� ������ ö�� �ٵ�� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1010, "�÷���Ʈ �尩", "�̸��� ������ ö�� �ٵ�� ���� �尩", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1011, "�÷���Ʈ �Ź�", "�̸��� ������ ö�� �ٵ�� ���� �Ź�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 5, 5, 5, 0, 10, 0, 700));
        item_list.Add(new Itemm(1012, "�÷���Ʈ ����", "�̸��� ������ ö�� �ٵ�� ���� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 5, 5, 5, 0, 12, 0, 700));
        item_list.Add(new Itemm(1013, "��ȯ�� ����", "�޼ӿ��� ã�Ƴ� ������ ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 7, 7, 7, 0, 20, 0, 1500));
        item_list.Add(new Itemm(1014, "��ȯ�� ����", "�޼ӿ��� ã�Ƴ� ������ ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1015, "��ȯ�� ����", "�޼ӿ��� ã�Ƴ� ������ ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1016, "��ȯ�� �尩", "�޼ӿ��� ã�Ƴ� ������ �尩", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1017, "��ȯ�� �Ź�", "�޼ӿ��� ã�Ƴ� ������ �Ź�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 7, 7, 7, 0, 17, 0, 1500));
        item_list.Add(new Itemm(1018, "��ȯ�� ����", "�޼ӿ��� ã�Ƴ� ������ ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 7, 7, 7, 0, 20, 0, 1500));
        item_list.Add(new Itemm(1019, "������ �÷���Ʈ ����", "������ ��縸�� �����Ҽ��ִ� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Torso, 10, 10, 10, 0, 35, 0, 5000));
        item_list.Add(new Itemm(1020, "������ �÷���Ʈ ����", "������ ��縸�� �����Ҽ��ִ� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Cloak, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1021, "������ �÷���Ʈ ����", "������ ��縸�� �����Ҽ��ִ� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Helmet, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1022, "������ �÷���Ʈ �尩", "������ ��縸�� �����Ҽ��ִ� �尩", Itemm.Item_Type.Equip, Itemm.Equip_Type.Glove, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1023, "������ �÷���Ʈ �Ź�", "������ ��縸�� �����Ҽ��ִ� �Ź�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Shoes, 10, 10, 10, 0, 30, 0, 5000));
        item_list.Add(new Itemm(1024, "������ �÷���Ʈ ����", "������ ��縸�� �����Ҽ��ִ� ����", Itemm.Item_Type.Equip, Itemm.Equip_Type.Pants, 10, 10, 10, 0, 35, 0, 5000));
        item_list.Add(new Itemm(2001, "����� �ռҵ�", "û������ ���� �ռҵ�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 10, 0, 2, 350));
        item_list.Add(new Itemm(2002, "�ǹ� �ռҵ�", "������ ���� �ռҵ�", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 25, 0, 4, 800));
        item_list.Add(new Itemm(2003, "������ �ռҵ�", "�ҹ����θ� ��� ������ ��", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 38, 0, 5, 2000));
        item_list.Add(new Itemm(2004, "������ �ռҵ�", "������ ���ε��� �𿩼� ���� ��", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 60, 0, 7, 7000));
        item_list.Add(new Itemm(2005, "ȯ���� �ռҵ�", "�޿����� �� �� �ִ� ������ ��", Itemm.Item_Type.Equip, Itemm.Equip_Type.Weapon, 0, 0, 0, 99, 0, 10, 11000));
        item_list.Add(new Itemm(3001, "ü������", "�ҷ��� ü���� ȸ�������ش�", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(3002, "��� ü������", "�߰��� ü���� ȸ�������ش�", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(3003, "�ְ�� ü������", "�뷮�� ü���� ȸ�������ش�", Itemm.Item_Type.Use, 0, 0, 0, 0, 0, 0, 0, 350));
        item_list.Add(new Itemm(4001, "����� ����", "��𼭳� �� �� �ִ� ����� ����", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(4002, "���� ��", "���ӿ��� �߰� �� �� �ִ� ������ ��", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4003, "Ǫ�� ��", "���ӿ��� �߰� �� �� �ִ� Ǫ���� ��", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 250));
        item_list.Add(new Itemm(4004, "���� ����", "����� ���� ����", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 100));
        item_list.Add(new Itemm(4005, "ö ����", "����� ö ����", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4006, "����� ��ⵢ��", "����� ���̴� ��ⵢ��", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 50));
        item_list.Add(new Itemm(4007, "����� ��ⵢ��", "����� ���̴� ��ⵢ��", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 150));
        item_list.Add(new Itemm(4008, "���� õ", "�������� ���� õ", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 200));
        item_list.Add(new Itemm(4009, "Ǫ�� õ", "Ǫ������ ���� õ", Itemm.Item_Type.Etc, 0, 0, 0, 0, 0, 0, 0, 250));
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
