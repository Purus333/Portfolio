using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TextManager : MonoBehaviour
{
    private static TextManager m_instance; // �̱����� �Ҵ�� static ����
    // �̱��� ���ٿ� ������Ƽ
    public static TextManager instance
    {
        get
        {
            // ���� �̱��� ������ ���� ������Ʈ�� �Ҵ���� �ʾҴٸ�
            if (m_instance == null)
            {
                m_instance = FindObjectOfType<TextManager>();
            }

            // �̱��� ������Ʈ�� ��ȯ
            return m_instance;
        }
    }

    public Text[] texts;

    public int text_count;
    private int count;
    public bool talk_go;
    private bool talk_end;
    private Text tmp_txt;
    public Text Skip_txt;
    private string tmp_talk;
    public bool dialog_end;

    private void Awake()
    {
        count = 0;
        talk_go = true;
        talk_end = false;
        dialog_end = false;

        Skip_txt.gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (NewStart_Loading_Screen.instance.loading_check == true && talk_go == true &&
            NewStart_Loading_Screen.instance.name_input_check == false)
        {
            Skip_txt.gameObject.SetActive(true);
            Set_Text();
        }

        Skip_Text();
    }

    void Skip_Text()
    {
        if (Input.GetKeyDown(KeyCode.G))
        {
            talk_go = false; // talk_go, dialog_end �ΰ��� ������ �ٽ� ����Ҷ� �ʱⰪ���� ��������
            dialog_end = true;
        }
    }

    void Set_Text()
    {
        talk_go = false;

        if (count < text_count)
        {
            tmp_txt = texts[count];
            tmp_talk = tmp_txt.text;
            tmp_txt.text = "";
        }
            StartCoroutine("Get_Text");
    }

    public void Next_Button()
    {
        if (count >= text_count)
        {
            texts[count - 1].gameObject.SetActive(false);
            dialog_end = true;
        }
        else if (talk_end == true)
        {
            if (texts[count].gameObject.activeSelf == true && count != text_count - 1)
                texts[count].gameObject.SetActive(false);

            count++;
            talk_go = true;
            talk_end = false;
        }
    }

    IEnumerator Get_Text()
    {
        if (count >= text_count)
        {
            talk_go = false;
            dialog_end = true;
        }
        else
        {
            foreach (char c in tmp_talk)
            {
                tmp_txt.text += c;
                texts[count].gameObject.SetActive(true);
                yield return new WaitForSeconds(0.07f);
            }
            talk_end = true;
        }

        yield return null;
    }
}
