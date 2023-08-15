using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class SaveAndLoad : MonoBehaviour
{
    public Save_Data save_data;
    public Save_Data load_data;
    private string save_directory;
    private string save_file_name = "/save.txt";

    void Start()
    {
        save_directory = Application.dataPath + "/Save/";

        if (Directory.Exists(save_directory) == false)
            Directory.CreateDirectory(save_directory);
    }

    void Update()
    {
        
    }

    public void Save()
    {
        save_data = new Save_Data();

        save_data._Player_Name = Player_Info.instance.Player_Name;
        save_data._Player_Level = Player_Info.instance.Player_Level;
        save_data._Player_Gold = Player_Info.instance.Player_Gold;
        save_data._Player_MaxExp = Player_Info.instance.Player_MaxExp;
        save_data._Player_Exp = Player_Info.instance.Player_Exp;
        save_data._Player_Max_Health = Player_Info.instance.Player_Max_Health;
        save_data._Player_Health = Player_Info.instance.health;
        save_data._Player_Str = Player_Info.instance.Player_Str;
        save_data._Player_Dex = Player_Info.instance.Player_Dex;
        save_data._Player_Con = Player_Info.instance.Player_Con;
        save_data._Power = Player_Info.instance.Power;
        save_data._Defense = Player_Info.instance.Defense;
        save_data._Critical = Player_Info.instance.Critical;
        save_data._player_woodnpc01_stat = Player_Info.instance.player_woodnpc01_stat;
        save_data._player_villagenpc02_stat = Player_Info.instance.player_villagenpc02_stat;
        save_data._player_villagenpc03_stat = Player_Info.instance.player_villagenpc03_stat;
        save_data._player_quest_mobcount = Player_Info.instance.player_quest_mobcount;
        save_data._player_quest_itemcount = Player_Info.instance.player_quest_itemcount;
        save_data._player_map_index = Player_Info.instance.player_map_index;

        Slot[] inven_slots = Inventory_Manager.instance.Get_Inven_Slots();
        for (int i = 0; i < inven_slots.Length; i++)
        {
            if (inven_slots[i].item_valid_check == true)
            {
                save_data.invenitem_array_num.Add(i);
                save_data.invenitem_name.Add(inven_slots[i].item.item_name);
                save_data.invenitem_count.Add(inven_slots[i].item_count);
            }
        }

        Slot[] cm_slots = Character_Manager.instance.Get_Cm_Slots();
        for (int i = 0; i < cm_slots.Length; i++)
        {
            if (cm_slots[i].item_valid_check == true)
            {
                save_data.charitem_array_num.Add(i);
                save_data.charitem_name.Add(cm_slots[i].item.item_name);
                save_data.charitem_count.Add(cm_slots[i].item_count);
            }
        }

        if (Portion.instance.p_slot.item_valid_check == true)
        {
            save_data.quickitem_name = Portion.instance.p_slot.item.item_name;
            save_data.quickitem_count = Portion.instance.p_slot.item_count;
        }

        string json = JsonUtility.ToJson(save_data);

        File.WriteAllText(save_directory + save_file_name, json);
    }

    public void Load()
    {
        load_data = new Save_Data();

        if (File.Exists(save_directory + save_file_name))
        {
            string load_json = File.ReadAllText(save_directory + save_file_name);
            load_data = JsonUtility.FromJson<Save_Data>(load_json);

            Player_Info.instance.Player_Name = load_data._Player_Name;
            Player_Info.instance.Player_Level = load_data._Player_Level;
            Player_Info.instance.Player_Gold = load_data._Player_Gold;
            Player_Info.instance.Player_MaxExp = load_data._Player_MaxExp;
            Player_Info.instance.Player_Exp = load_data._Player_Exp;
            Player_Info.instance.Player_Max_Health = load_data._Player_Max_Health;
            Player_Info.instance.health = load_data._Player_Health;
            Player_Info.instance.Player_Str = load_data._Player_Str;
            Player_Info.instance.Player_Dex = load_data._Player_Dex;
            Player_Info.instance.Player_Con = load_data._Player_Con;
            Player_Info.instance.Power = load_data._Power;
            Player_Info.instance.Defense = load_data._Defense;
            Player_Info.instance.Critical = load_data._Critical;
            Player_Info.instance.player_woodnpc01_stat = load_data._player_woodnpc01_stat;
            Player_Info.instance.player_villagenpc02_stat = load_data._player_villagenpc02_stat;
            Player_Info.instance.player_villagenpc03_stat = load_data._player_villagenpc03_stat;
            Player_Info.instance.player_quest_mobcount = load_data._player_quest_mobcount;
            Player_Info.instance.player_quest_itemcount = load_data._player_quest_itemcount;
            Player_Info.instance.player_map_index = load_data._player_map_index;

            for (int i = 0; i < load_data.invenitem_name.Count; i++)
                Inventory_Manager.instance.Load_Inven(load_data.invenitem_array_num[i], load_data.invenitem_name[i], load_data.invenitem_count[i]);

            for (int i = 0; i < load_data.charitem_name.Count; i++)
                Character_Manager.instance.Load_Cm(load_data.charitem_array_num[i], load_data.charitem_name[i], load_data.charitem_count[i]);

            if (load_data.quickitem_count > 0)
                Portion.instance.Load_Quick_Item(load_data.quickitem_name, load_data.quickitem_count);
        }
    }
}
