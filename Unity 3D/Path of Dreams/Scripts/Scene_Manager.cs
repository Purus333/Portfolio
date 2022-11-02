using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Scene_Manager : MonoBehaviour
{
    public GameObject[] obj;
    public int obj_num = 0;
    public int cur_s_num;
    public int next_s_num;

    private bool obj_set_check;

    private void Awake()
    {
        obj_set_check = false;
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (GameManager.instance.newgame_started == true && obj_set_check == false)
        {
            Set_Move_Object_Default();
            obj_set_check = true;
        }
    }

    public void Move_Scene()
    {
        SceneManager.LoadSceneAsync(next_s_num, LoadSceneMode.Additive);

        Scene scene = SceneManager.GetSceneByBuildIndex(next_s_num);

        for (int i = 0; i < obj_num; i++)
            SceneManager.MoveGameObjectToScene(obj[i], scene);

        GameManager.instance.scene_move_check = true;
        Player_Info.instance.player_map_index = next_s_num;
        Player_Info.instance.player_scene_move_check = true;

        GameManager.instance.scene_move = true;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (GameManager.instance.scene_move == false)
            Move_Scene();
    }

    void Set_Move_Object_Default()
    {
        if (GameObject.FindWithTag("Player").activeSelf == true)
            obj[0] = GameObject.FindWithTag("Player");
        if (GameObject.FindWithTag("GameManager").activeSelf == true)
            obj[1] = GameObject.FindWithTag("GameManager");
        if (GameObject.FindWithTag("Ui_Canvas").activeSelf == true)
            obj[2] = GameObject.FindWithTag("Ui_Canvas");
        if (GameObject.FindWithTag("FCam").activeSelf == true)
            obj[3] = GameObject.FindWithTag("FCam");
        if (GameObject.FindWithTag("FCam2").activeSelf == true)
            obj[4] = GameObject.FindWithTag("FCam2");
        if (GameObject.FindWithTag("Minimap").activeSelf == true)
            obj[5] = GameObject.FindWithTag("Minimap");
    }
}
