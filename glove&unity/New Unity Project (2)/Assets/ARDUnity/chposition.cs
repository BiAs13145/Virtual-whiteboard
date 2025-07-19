using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class chposition : MonoBehaviour
{
    float px, py;
    
    

    // Start is called before the first frame update
    void Start()
    {
        px = connection.px;
        py = connection.py;
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = new Vector3(px, py, 0);
    }
}
