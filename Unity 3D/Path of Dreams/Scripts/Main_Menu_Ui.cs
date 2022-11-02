using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.IO;

public class Main_Menu_Ui : MonoBehaviour
{
    public GameObject gm;
    private bool click_check;

    private string save_directory;
    private string save_file_name = "/save.txt";

    private void Awake()
    {
        click_check = false;
    }

    // Start is called before the first frame update
    void Start()
    {
        save_directory = Application.dataPath + "/Save/";

        if (Directory.Exists(save_directory) == false)
            Directory.CreateDirectory(save_directory);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void NewStart()
    {
        if (click_check == false)
        {
            SceneManager.LoadSceneAsync("First_Scene", LoadSceneMode.Additive);
            Scene scene = SceneManager.GetSceneByBuildIndex(1);
            SceneManager.MoveGameObjectToScene(gm, scene);

            click_check = true;
        }
    }

    public void LoadStart()
    {
        if (File.Exists(save_directory + save_file_name))
        {
            if (click_check == false)
            {
                GameManager.instance.load_data = true;

                SceneManager.LoadSceneAsync("First_Scene", LoadSceneMode.Additive);
                Scene scene = SceneManager.GetSceneByBuildIndex(1);
                SceneManager.MoveGameObjectToScene(gm, scene);

                click_check = true;
            }
        }
    }

    public void GameExit()
    {
        if (click_check == false)
        {
            UnityEditor.EditorApplication.isPlaying = false;

            Application.Quit();

            click_check = true;
        }
    }
}
