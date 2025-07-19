using System.Collections;
using System.IO;
using UnityEngine;

public class Paintable : MonoBehaviour
{

    public GameObject Brush;
    public GameObject pointer;
    public GameObject board;
    public GameObject player;
    public GameObject vispoint;

    public float BrushSize = 1.0f;
    public RenderTexture RTexture;
    
    Ray hand;
    RaycastHit hit;

    bool pointon;

    

    // Use this for initialization
    void rotate()
    {
        

    }
    void Start()
    {
        transform.LookAt(board.transform);
    }

    // Update is called once per frame
    void Update()
    {
        rotate();
        
        hand.origin = player.transform.position;
        hand.direction = player.transform.forward;

        int poin = connection.poin;

        int draw = connection.draw;
        

        if ((Input.GetKey(KeyCode.K)))//點
        {
            pointer.SetActive(true);

            Physics.Raycast(hand, out  hit);



            pointer.transform.position = hit.point - transform.forward;
            
        }
       else pointer.SetActive(false);



        if (Input.GetKey("space"))//畫

            {
            //cast a ray to the plane
            Brush.SetActive(true);

            Physics.Raycast(hand, out hit);
            Brush.transform.position = hit.point;

            //instanciate a brush
            if (hit.collider!=Brush)
            {
                var go = Instantiate(Brush, hit.point, board.transform.rotation);
                go.transform.localScale = Vector3.one * BrushSize;
            }
                

            }
            if (Input.GetKey(KeyCode.L))
        {
            //if (hit.collider ==Brush)
           // {
                //GameObject eraser = hit.collider.gameObject;
                //Destroy(eraser);

            //}


        }
        
    }

    public void Save()
    {
        StartCoroutine(CoSave());
    }

    private IEnumerator CoSave()
    {
        //wait for rendering
        yield return new WaitForEndOfFrame();
        Debug.Log(Application.dataPath + "/savedImage.png");

        //set active texture
        RenderTexture.active = RTexture;

        //convert rendering texture to texture2D
        var texture2D = new Texture2D(RTexture.width, RTexture.height);
        texture2D.ReadPixels(new Rect(0, 0, RTexture.width, RTexture.height), 0, 0);
        texture2D.Apply();

        //write data to file
        var data = texture2D.EncodeToPNG();
        File.WriteAllBytes(Application.dataPath + "/savedImage.png", data);


    }
}