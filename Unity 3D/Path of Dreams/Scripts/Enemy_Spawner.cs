using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy_Spawner : MonoBehaviour
{
    private static Enemy_Spawner m_instance;

    public static Enemy_Spawner instance // 접근용 프로퍼티
    {
        get
        {
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<Enemy_Spawner>();
            }
            return m_instance;
        }
    }

    public GameObject obj_pool;
    public Enemy enemyPrefab; // 소환할 적
    public Transform spawnPoint; // 적을 소환할 위치
    public Vector3 save_respawn_pt;

    private List<Enemy> enemies = new List<Enemy>();

    private bool first_spawn = false;
    private bool spawn_start = false;
    public int spawn_count = 0;
    private bool respawn_check = false;

    private float timer = 0;
    private int wait_time = 0;

    private void Awake()
    {
        if (m_instance != null)
        {
            Destroy(this.gameObject);
        }
    }

    void Start()
    {

    }

    void Update()
    {
        if (Player_Movement.instance.player_Input.move_animotion > 0 || spawn_start == true) // 로딩후 캐릭터가 움직일시 스폰시작
        {
            if (Player_Info.instance.player_scene_move_check == false && first_spawn == false)
                Init_Enemy();
            else if (Player_Info.instance.player_scene_move_check == false && first_spawn == true && respawn_check == false)
            {
                int dead_count = 0;

                for (int i = 0; i < enemies.Count; i++)
                {
                    if (enemies[i].dead == true && enemies[i].gameObject.activeSelf == false)
                        dead_count++;
                }

                if (dead_count == spawn_count)
                    respawn_check = true;
            }
            else if (Player_Info.instance.player_scene_move_check == false && first_spawn == true && respawn_check == true)
                ReSpawnEnemy();
        }
    }

    private void Init_Enemy()
    {
        if (enemyPrefab.gameObject.name == "Polygonal Metalon Green")
            spawn_count = 4;
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Red")
            spawn_count = 1;
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Purple")
            spawn_count = 1;

        for (int i = 0; i < spawn_count; i++)
        {
            Enemy temp_e;
            temp_e = CreateEnemy(Random.Range(-3.0f, 3.0f), Random.Range(-3.0f, 3.0f));
            enemies.Add(temp_e); // add list
            enemies[i].spawn_pos = save_respawn_pt;
        }

        first_spawn = true;
        spawn_start = true;
    }

    private void ReSpawnEnemy()
    {
        int re_health = 0;
        int re_damage = 0;
        float re_speed = 0;

        if (enemyPrefab.gameObject.name == "Polygonal Metalon Green")
        {
            spawn_count = 4;
            wait_time = 11;

            re_health = Random.Range(80, 100);
            re_damage = 5;
            re_speed = 1.8f;
        }
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Red")
        {
            spawn_count = 1;
            wait_time = 30;

            re_health = Random.Range(500, 550);
            re_damage = 20;
            re_speed = 2.3f;
        }
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Purple")
        {
            spawn_count = 1;
            wait_time = 90;

            re_health = Random.Range(1500, 1550);
            re_damage = 35;
            re_speed = 2.5f;
        }

        if (wait_time > timer)
        {
            timer += Time.deltaTime;
            return;
        }
        else
            timer = 0;

        // for 문으로 몹 전체 리스폰

        for (int i = 0; i < enemies.Count; i++)
        {
            enemies[i].gameObject.transform.position = enemies[i].spawn_pos;
            enemies[i].Setup(re_health, re_damage, re_speed);
        }

        respawn_check = false;
    }

    private Enemy CreateEnemy(float _x, float _z)
    {
        int health = 0;
        int damage = 0;
        float speed = 0;

        if (enemyPrefab.gameObject.name == "Polygonal Metalon Green")
        {
            // 랜덤값
            health = Random.Range(80, 100);
            damage = 5;
            speed = 1.8f;
        }
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Red")
        {
            health = Random.Range(500, 550);
            damage = 20;
            speed = 2.3f;
        }
        else if (enemyPrefab.gameObject.name == "Polygonal Metalon Purple")
        {
            health = Random.Range(1500, 1550);
            damage = 35;
            speed = 2.5f;
        }

        Vector3 save_pos = spawnPoint.position; // 원상복귀용
        Vector3 temp_pos = spawnPoint.position;
        temp_pos.x += _x;
        temp_pos.z += _z;
        spawnPoint.position = temp_pos;

        Enemy enemy = Instantiate(enemyPrefab, spawnPoint.position, spawnPoint.rotation);
        enemy.Setup(health, damage, speed);
        enemy.transform.parent = obj_pool.transform;
        //enemies.Add(enemy); // 리스트에 적 추가

        //enemy.onDeath += () => enemies.Remove(enemy); // 람다식
        //enemy.onDeath += () => Destroy(enemy.gameObject, 5.0f);

        save_respawn_pt = spawnPoint.position; // 리스폰을 위한 포지션
        spawnPoint.position = save_pos;

        return enemy;
    }
}
