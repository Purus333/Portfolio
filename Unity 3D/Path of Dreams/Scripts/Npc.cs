using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Npc : LivingEntity
{
    private LivingEntity targetEntity;
    private NavMeshAgent pathFinder;
    public Animator npc_Animator;

    public Vector3 npc_pos;
    public Vector3 des_pos;
    public Quaternion npc_ro;
    public int npc_num;
    private bool set_move = false;
    private bool move_done = false;
    private bool pause_move = false;
    private float idle_time = 0.0f;

    private void Awake()
    {
        pathFinder = GetComponent<NavMeshAgent>();
    }

    // Start is called before the first frame update
    void Start()
    {
        npc_pos = this.gameObject.transform.position;

        npc_ro = this.gameObject.transform.rotation;

        if (this.gameObject.name == "wood_npc01")
            npc_num = 1;
        else if (this.gameObject.name == "Village_npc01")
            npc_num = 11;
        else if (this.gameObject.name == "Village_npc02")
            npc_num = 12;
        else if (this.gameObject.name == "Village_npc03")
            npc_num = 13;
        else if (this.gameObject.name == "Village_npc04(store)")
            npc_num = 14;

        StartCoroutine(UpdatePath());
    }

    // Update is called once per frame
    void Update()
    {
        if (pause_move == true)
        {
            pathFinder.isStopped = true;

            if (npc_num == 1 || npc_num == 11)
                npc_Animator.SetBool("Npc_Walk", false);
            else if (npc_num == 12)
                npc_Animator.SetBool("Npc_Attack", false);

            if (Player_Info.instance.player_talkstart == false)
            {
                pause_move = false;
                this.gameObject.transform.LookAt(this.gameObject.transform);
                this.gameObject.transform.rotation = npc_ro;
            }
        }
        else
            Move();
    }

    private void Move()
    {
        if (set_move == true && move_done == false)
        {
            if (npc_num == 1 || npc_num == 11) // moving motion npc
            {
                pathFinder.isStopped = false;
                npc_Animator.SetBool("Npc_Walk", true);

                if (pathFinder.remainingDistance <= pathFinder.stoppingDistance)
                {
                    pathFinder.isStopped = true;
                    npc_Animator.SetBool("Npc_Walk", false);
                    Wait_Timer(5.0f);
                }
            }
            else if (npc_num == 12) // attack motion npc
            {
                if (npc_Animator.GetCurrentAnimatorStateInfo(0).IsName("Female Sword Attack 3") && npc_Animator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f)
                    npc_Animator.SetBool("Npc_Attack", false);
                else if (npc_Animator.GetBool("Npc_Attack") == false)
                    Wait_Timer(2.0f);
            }
        }
        else if (set_move == true && move_done == true)
        {
            if (npc_num == 1 || npc_num == 11)
            {
                pathFinder.SetDestination(npc_pos);
                pathFinder.isStopped = false;
                npc_Animator.SetBool("Npc_Walk", true);

                if (pathFinder.remainingDistance <= pathFinder.stoppingDistance)
                {
                    pathFinder.isStopped = true;
                    npc_Animator.SetBool("Npc_Walk", false);

                    this.gameObject.transform.LookAt(this.gameObject.transform);
                    this.gameObject.transform.rotation = npc_ro;

                    Wait_Timer(5.0f);
                }
            }
            else if (npc_num == 12)
                Wait_Timer(0.5f);
        }
    }

    private IEnumerator UpdatePath()
    {
        while(!dead)
        {
            if (set_move == false && move_done == false)
            {
                if (npc_num == 1)
                {
                    des_pos = npc_pos;
                    des_pos.x += 6;
                    pathFinder.SetDestination(des_pos);
                    set_move = true;
                }
                else if (npc_num == 11)
                {
                    int tmp_num = 0;

                    tmp_num = Random.Range(1, 3);

                    if (tmp_num == 1)
                    {
                        des_pos = npc_pos;
                        des_pos.z -= 8;
                        pathFinder.SetDestination(des_pos);
                    }
                    else if (tmp_num == 2)
                    {
                        des_pos = npc_pos;
                        des_pos.x += 9;
                        pathFinder.SetDestination(des_pos);
                    }

                    set_move = true;
                }
                else if (npc_num == 12)
                {
                    pathFinder.isStopped = false;
                    npc_Animator.SetBool("Npc_Attack", true);
                    set_move = true;
                }
            }

            yield return new WaitForSeconds(0.30f);
        }
    }

    public void Wait_Timer(float time_num)
    {
        if (time_num > idle_time)
        {
            idle_time += Time.deltaTime;
            return;
        }
        else
        {
            if (set_move == true && move_done == false)
                move_done = true;
            else if (set_move == true && move_done == true)
            {
                set_move = false;
                move_done = false;
            }

            idle_time = 0;
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.CompareTag("Player") && other.gameObject.GetComponentInParent<LivingEntity>().dead == false)
        {
            if (Player_Info.instance.player_talkstart == true)
            {
                this.gameObject.transform.LookAt(other.transform);
                pause_move = true;
            }
        }
    }
}
