using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

[System.Serializable]
public class Save_Data
{
    public string _Player_Name;
    public int _Player_Level;
    public int _Player_Gold;
    public float _Player_MaxExp;
    public float _Player_Exp;
    public float _Player_Max_Health;
    public float _Player_Health;
    public int _Player_Str;
    public int _Player_Dex;
    public int _Player_Con;
    public float _Power;
    public float _Defense;
    public float _Critical;

    public int _player_woodnpc01_stat;
    public int _player_villagenpc02_stat;
    public int _player_villagenpc03_stat;
    public int _player_quest_mobcount;
    public int _player_quest_itemcount;
    public int _player_map_index;

    public List<int> invenitem_array_num = new List<int>();
    public List<string> invenitem_name = new List<string>();
    public List<int> invenitem_count = new List<int>();

    public List<int> charitem_array_num = new List<int>();
    public List<string> charitem_name = new List<string>();
    public List<int> charitem_count = new List<int>();

    public string quickitem_name;
    public int quickitem_count;

    void Start()
    {

    }

    void Update()
    {
        
    }
}
