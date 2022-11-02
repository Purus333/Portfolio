using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Npc_Interact : MonoBehaviour
{
    public Image npc_hud;
    public Image[] talk_panal;
    public TextManager_Npc[] TM_npc;
    public GameObject okcancel_panal_obj;
    public GameObject store_panal_obj;
    private bool store_open = false;

    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < TM_npc.Length; i++)
        {
            TM_npc[i].talk_go = false;
            TM_npc[i].dialog_end = true;
        }

        store_panal_obj = GameObject.FindGameObjectWithTag("Npc_Inven").transform.GetChild(0).gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        if (GameManager.instance.npc_text_start == true)
            Check_TalkEnd();
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.CompareTag("Player") && other.gameObject.GetComponentInParent<LivingEntity>().dead == false)
        {
            npc_hud.gameObject.SetActive(true);
            Player_Info.instance.player_talkable = true;

            if (GameManager.instance.npc_text_start == false && Player_Info.instance.player_talkable == true && Player_Info.instance.player_talkstart == true)
            {
                if (this.gameObject.name == "wood_npc01")
                    WoodNpc();
                else if (this.gameObject.name == "Village_npc01")
                    VillageNpc01();
                else if (this.gameObject.name == "Village_npc02")
                    VillageNpc02();
                else if (this.gameObject.name == "Village_npc03")
                    VillageNpc03();
                else if (this.gameObject.name == "Village_npc04(store)")
                    VillageNpc04();
            }
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Player") && other.gameObject.GetComponentInParent<LivingEntity>().dead == false)
        {
            npc_hud.gameObject.SetActive(false);
            Player_Info.instance.player_talkable = false;
        }
    }

    public void Check_TalkEnd()
    {
        for (int i = 0; i < talk_panal.Length; i++)
        {
            if (talk_panal[i].gameObject.activeSelf == true)
                for (int j = 0; j < TM_npc.Length; j++)
                {
                    if (TM_npc[j].dialog_end == true && TM_npc[j].talk_go == false && i == j)
                    {
                        talk_panal[j].gameObject.SetActive(false);
                        Player_Info.instance.player_talkstart = false;
                        GameManager.instance.npc_text_start = false;

                        if (this.gameObject.name == "wood_npc01")
                        {
                            if (Player_Info.instance.player_woodnpc01_stat == 1 && i == 0 && j == 0) // 대화마치고 보상 - woodnpc
                            {
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[29], GameManager.instance.item_list[29].item_id, 5);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[24], GameManager.instance.item_list[24].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[0], GameManager.instance.item_list[0].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[1], GameManager.instance.item_list[1].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[2], GameManager.instance.item_list[2].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[3], GameManager.instance.item_list[3].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[4], GameManager.instance.item_list[4].item_id);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[5], GameManager.instance.item_list[5].item_id);
                                Player_Info.instance.player_woodnpc01_stat = 2;
                            }
                        }
                        else if (this.gameObject.name == "Village_npc02")
                        {
                            if (Player_Info.instance.player_villagenpc02_stat == 0 && i == 0 && j == 0)
                                okcancel_panal_obj.gameObject.SetActive(true);
                            else if (Player_Info.instance.player_villagenpc02_stat == 2 && i == 2 && j == 2)
                            {
                                Player_Info.instance.Player_Get_Exp(100);
                                Player_Info.instance.Player_Get_Gold(500);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[25], GameManager.instance.item_list[25].item_id);

                                Ui_Manager.instance.quest_titleTxt[0].gameObject.SetActive(false);
                                Ui_Manager.instance.quest_detailTxt[0].gameObject.SetActive(false);
                                Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_questclear_sound);
                                Player_Info.instance.player_villagenpc02_stat = 3;
                            }
                        }
                        else if (this.gameObject.name == "Village_npc03")
                        {
                            if (Player_Info.instance.player_villagenpc03_stat == 0 && i == 0 && j == 0)
                                okcancel_panal_obj.gameObject.SetActive(true);
                            else if (Player_Info.instance.player_villagenpc03_stat == 2 && i == 2 && j == 2)
                            {
                                Player_Info.instance.Player_Get_Exp(80);
                                Player_Info.instance.Player_Get_Gold(350);
                                Inventory_Manager.instance.Get_Item(GameManager.instance.item_list[29], GameManager.instance.item_list[29].item_id, 10);

                                Ui_Manager.instance.quest_titleTxt[1].gameObject.SetActive(false);
                                Ui_Manager.instance.quest_detailTxt[1].gameObject.SetActive(false);
                                Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_questclear_sound);
                                Player_Info.instance.player_villagenpc03_stat = 3;
                                Inventory_Manager.instance.Quest_Item_Delete(1, 4006);
                            }
                        }
                        else if (this.gameObject.name == "Village_npc04(store)")
                        {
                            if (i == 0 && j == 0)
                            {
                                GameManager.instance.store_open = true;
                                store_panal_obj.gameObject.SetActive(true);
                            }
                        }
                        break;
                    }
                }
        }
    }

    public void Set_Talk(int num)
    {
        GameManager.instance.npc_text_start = true;
        talk_panal[num].gameObject.SetActive(true);
        TM_npc[num].talk_go = true;
        TM_npc[num].dialog_end = false;
        TM_npc[num].count = 0;
        for (int i = 0; i < TM_npc[num].texts.Length; i++)
            TM_npc[num].texts[i].gameObject.SetActive(false);
    }

    public void WoodNpc()
    {
        if (Player_Info.instance.player_woodnpc01_stat == 0)
        {
            Set_Talk(0);

            Player_Info.instance.player_woodnpc01_stat = 1;
        }
        else if (Player_Info.instance.player_woodnpc01_stat == 2)
            Set_Talk(1);
    }

    public void VillageNpc01()
    {
        Set_Talk(0);
    }

    public void VillageNpc02()
    {
        if (Player_Info.instance.player_villagenpc02_stat == 0)
            Set_Talk(0);
        else if (Player_Info.instance.player_villagenpc02_stat == 1)
            Set_Talk(1);
        else if (Player_Info.instance.player_villagenpc02_stat == 2 || Player_Info.instance.player_villagenpc02_stat == 3)
            Set_Talk(2);
    }

    public void VillageNpc03()
    {
        if (Player_Info.instance.player_villagenpc03_stat == 0)
            Set_Talk(0);
        else if (Player_Info.instance.player_villagenpc03_stat == 1)
            Set_Talk(1);
        else if (Player_Info.instance.player_villagenpc03_stat == 2 || Player_Info.instance.player_villagenpc03_stat == 3)
            Set_Talk(2);
    }

    public void VillageNpc04()
    {
        if (GameManager.instance.store_open == false)
            Set_Talk(0);
    }

    public void Select_Ok()
    {
        okcancel_panal_obj.gameObject.SetActive(false);

        if (this.gameObject.name == "Village_npc02" && Player_Info.instance.player_villagenpc02_stat == 0)
        {
            Player_Info.instance.player_villagenpc02_stat = 1;
            Ui_Manager.instance.Quest_Info(0, Player_Info.instance.player_quest_mobcount);
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_questok_sound);
        }
        else if (this.gameObject.name == "Village_npc03" && Player_Info.instance.player_villagenpc03_stat == 0)
        {
            Player_Info.instance.player_villagenpc03_stat = 1;
            Ui_Manager.instance.Quest_Info(1, Player_Info.instance.player_quest_itemcount);
            Player_Info.instance.player_info_audio.PlayOneShot(Player_Info.instance.player_questok_sound);
        }
    }

    public void Select_Cancel()
    {
        okcancel_panal_obj.gameObject.SetActive(false);
    }
}
