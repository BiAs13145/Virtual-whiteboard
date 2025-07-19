using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class Mouse : MonoBehaviour
{


    public Transform player;

    float xrotation = 0.0f;
    float yrotation = 0f;

    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;
    }

    // Update is called once per frame
    void Update()
    {
        float mousex = Input.GetAxis("Mouse X")*100*Time.deltaTime;

        float mousey = Input.GetAxis("Mouse Y") * 100 * Time.deltaTime;

        xrotation -= mousey;

        yrotation += mousex;

        xrotation = Mathf.Clamp(xrotation, -90f, 90f);

        transform.localRotation = Quaternion.Euler(xrotation, yrotation, 0f);

        

        player.Rotate(Vector3.up * mousex);
        



    }
}
