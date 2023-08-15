using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Ingame_Menu : MonoBehaviour
{
    public GameObject menu_background_obj;
    public SaveAndLoad in_SaveandLoad;

    // Start is called before the first frame update
    void Start()
    {
        in_SaveandLoad = GameManager.instance.g_SaveandLoad.GetComponent<SaveAndLoad>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void Open_Menu()
    {
        menu_background_obj.gameObject.SetActive(true);
    }

    public void Continue_Game()
    {
        menu_background_obj.gameObject.SetActive(false);
    }

    public void Save_Game()
    {
        in_SaveandLoad.Save();

        menu_background_obj.gameObject.SetActive(false);
        Ui_Manager.instance.Save_Done_Notice();
    }

    public void Goback_MainMenu()
    {
        SceneManager.LoadSceneAsync("MenuScene", LoadSceneMode.Single);
    }
}
