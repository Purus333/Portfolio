using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Ingame_Menu : MonoBehaviour
{
    public GameObject menu_background_obj;

    // Start is called before the first frame update
    void Start()
    {
        
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
        SaveAndLoad.instance.Save();

        menu_background_obj.gameObject.SetActive(false);
        Ui_Manager.instance.Save_Done_Notice();
    }

    public void Goback_MainMenu()
    {
        SceneManager.LoadSceneAsync("MenuScene", LoadSceneMode.Single);
    }
}
