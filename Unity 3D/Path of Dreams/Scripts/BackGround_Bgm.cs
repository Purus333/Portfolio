using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackGround_Bgm : MonoBehaviour
{
    public AudioSource background_audio;
    public AudioClip bgm1;
    public AudioClip bgm2;
    public AudioClip bgm3;

    private int check_map_index;

    private void Awake()
    {
        check_map_index = 0;
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (check_map_index != Player_Info.instance.player_map_index)
            Check_Bgm();
    }

    public void Check_Bgm()
    {
        if (Player_Info.instance.player_map_index == 1)
        {
            background_audio.Stop();
            background_audio.clip = bgm1;
            background_audio.Play();
            check_map_index = 1;
        }
        else if (Player_Info.instance.player_map_index == 2)
        {
            background_audio.Stop();
            background_audio.clip = bgm1;
            background_audio.Play();
            check_map_index = 2;
        }
        else if (Player_Info.instance.player_map_index == 3)
        {
            background_audio.Stop();
            background_audio.clip = bgm2;
            background_audio.Play();
            check_map_index = 3;
        }
        else if (Player_Info.instance.player_map_index == 4 || Player_Info.instance.player_map_index == 5)
        {
            background_audio.Stop();
            background_audio.clip = bgm3;
            background_audio.Play();
            check_map_index = Player_Info.instance.player_map_index;
        }
    }
}
