using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Input_Manager : MonoBehaviour
{
    public InputField input_text;
    public Canvas ui_cvs;
    public SaveAndLoad iput_SaveandLoad;

    private void Awake()
    {
        ui_cvs = ui_cvs.GetComponent<Canvas>();
    }

    // Start is called before the first frame update
    void Start()
    {
        iput_SaveandLoad = GameManager.instance.g_SaveandLoad.GetComponent<SaveAndLoad>();
    }

    // Update is called once per frame
    void Update()
    {
        if (NewStart_Loading_Screen.instance.name_input_check == false && GameManager.instance.load_data == true)
            Load_Set();

        if (NewStart_Loading_Screen.instance.name_input_check == false)
            Name_Set_Check();
    }

    public void Set_NewGame()
    {
        if (input_text.text.Length <= 0) // �̸� ���Է½�
            return;

        Player_Info.instance.Player_Name = input_text.text; // �̸�����
        NewStart_Loading_Screen.instance.Input_image.gameObject.SetActive(false); // ����â �ݱ�
        NewStart_Loading_Screen.instance.name_input_check = true; // �̸��Է� üũ

        ui_cvs.gameObject.SetActive(true); // ���� �������̽� on

        Ui_Manager.instance.UserNameUi(Player_Info.instance.Player_Name);
        Ui_Manager.instance.UserHpInfo(Player_Info.instance.Player_Max_Health, Player_Info.instance.health);
        Ui_Manager.instance.UserLvInfo(Player_Info.instance.Player_Level);
        Ui_Manager.instance.UserExpInfo(Player_Info.instance.Player_Exp, Player_Info.instance.Player_MaxExp);
        Ui_Manager.instance.UserDetailInfo(Player_Info.instance.Player_Level, Player_Info.instance.Player_Str, Player_Info.instance.Player_Dex, 
            Player_Info.instance.Player_Con, Player_Info.instance.Power, Player_Info.instance.Defense, Player_Info.instance.Critical);
        Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
    }

    public void Name_Set_Check()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            if (input_text.text.Length == 0) // �̸� ���Է½�
                return;

            Player_Info.instance.Player_Name = input_text.text;
            NewStart_Loading_Screen.instance.Input_image.gameObject.SetActive(false);
            NewStart_Loading_Screen.instance.name_input_check = true;

            ui_cvs.gameObject.SetActive(true); // ���� �������̽� on

            Ui_Manager.instance.UserNameUi(Player_Info.instance.Player_Name);
            Ui_Manager.instance.UserHpInfo(Player_Info.instance.Player_Max_Health, Player_Info.instance.health);
            Ui_Manager.instance.UserLvInfo(Player_Info.instance.Player_Level);
            Ui_Manager.instance.UserExpInfo(Player_Info.instance.Player_Exp, Player_Info.instance.Player_MaxExp);
            Ui_Manager.instance.UserDetailInfo(Player_Info.instance.Player_Level, Player_Info.instance.Player_Str, Player_Info.instance.Player_Dex,
                Player_Info.instance.Player_Con, Player_Info.instance.Power, Player_Info.instance.Defense, Player_Info.instance.Critical);
            Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
        }
    }

    public void Load_Set()
    {
        NewStart_Loading_Screen.instance.Input_image.gameObject.SetActive(false);
        NewStart_Loading_Screen.instance.name_input_check = true;

        ui_cvs.gameObject.SetActive(true); // ���� �������̽� on

        iput_SaveandLoad.Load();

        Player_Info.instance.healthSlider.maxValue = Player_Info.instance.Player_Max_Health;
        Player_Info.instance.healthSlider.value = Player_Info.instance.health;
        Player_Info.instance.expSlider.maxValue = Player_Info.instance.Player_MaxExp;
        Player_Info.instance.expSlider.value = Player_Info.instance.Player_Exp;

        Ui_Manager.instance.UserNameUi(Player_Info.instance.Player_Name);
        Ui_Manager.instance.UserHpInfo(Player_Info.instance.Player_Max_Health, Player_Info.instance.health);
        Ui_Manager.instance.UserLvInfo(Player_Info.instance.Player_Level);
        Ui_Manager.instance.UserExpInfo(Player_Info.instance.Player_Exp, Player_Info.instance.Player_MaxExp);
        Ui_Manager.instance.UserDetailInfo(Player_Info.instance.Player_Level, Player_Info.instance.Player_Str, Player_Info.instance.Player_Dex,
            Player_Info.instance.Player_Con, Player_Info.instance.Power, Player_Info.instance.Defense, Player_Info.instance.Critical);
        Ui_Manager.instance.UserGoldInfo(Player_Info.instance.Player_Gold);
    }
}
