using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HelpTutorial : MonoBehaviour
{
    public GameObject help_icon;
    private bool help_key_check;

    private void Awake()
    {

    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.H))
            Open_Help();
        if (Input.GetKeyDown(KeyCode.Escape))
            Close_Help();
    }

    void Open_Help()
    {
        if (help_key_check == false)
            help_key_check = true;
        else
            help_key_check = false;

        if (help_key_check == true)
        {
            help_icon.transform.parent.SetSiblingIndex(13);
            help_icon.gameObject.SetActive(true);
        }
        else
            help_icon.gameObject.SetActive(false);
    }

    public void Close_Help()
    {
        help_key_check = false;
        help_icon.gameObject.SetActive(false);
    }

    public void Click_Open_Help_Icon()
    {
        help_icon.transform.parent.SetSiblingIndex(13);
        help_key_check = true;
        help_icon.gameObject.SetActive(true);
    }

    public void Click_Close_Help_Icon()
    {
        help_key_check = false;
        help_icon.gameObject.SetActive(false);
    }
}
