using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotation : MonoBehaviour
{
    // Start is called before the first frame update
    private float x, y, tx, ty;
     Vector3 currentEulerAngles;
    void Start()
    {
        tx = connection.x;
        ty = connection.y;


    }
    // Update is called once per frame
    void Update()
    {
        x = connection.x;//-tx;
        y = connection.y;//-ty;

        

        tx = connection.x;
        ty = connection.y;




        if (Input.GetKey(KeyCode.S)) x += 5;
        if (Input.GetKey(KeyCode.W))  x -= 5;
        if (Input.GetKey(KeyCode.D)) y += 5;
        if (Input.GetKey(KeyCode.A)) y -= 5;

        //if (Input.GetKey(KeyCode.S)) gameObject.transform.Rotate(50*Time.deltaTime, 0, 0,Space.Self);

        //if (Input.GetKey(KeyCode.W)) gameObject.transform.Rotate(-50*Time.deltaTime, 0, 0,Space.Self);

        //if (Input.GetKey(KeyCode.D)) gameObject.transform.Rotate(0, 50 * Time.deltaTime,0, Space.World);

        //if (Input.GetKey(KeyCode.A)) gameObject.transform.Rotate(0, -50 * Time.deltaTime, 0, Space.World);

        transform.eulerAngles = currentEulerAngles;

       

        //transform.localEulerAngles = transform.localEulerAngles+new Vector3( -x, y, 0);

        transform.Rotate(x, y, 0);


    }
}
