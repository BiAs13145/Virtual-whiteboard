using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;
using System.Threading;
using System.IO.Ports;
using System.IO;



public class connection : MonoBehaviour
{

    private SerialPort sp;
    private int readTimeout = 1;
    public string port;
    private Thread readthread;
    public string readmessage;
    public string msg;
    public string[] ugmsg;
    public string[] avp;
    public static float x,y,px,py,tx,ty,unx,uny;
    public static int poin, draw;


    // Start is called before the first frame update

    void Start()
    {
        sp = new SerialPort("COM3", 9600);
        
        if (!sp.IsOpen)
        {
            sp.Open();
            sp.ReadTimeout = readTimeout;
        }
    }

    // Update is called once per frame
    void Update()
    {
        msg = sp.ReadLine();


        sp.BaseStream.Flush();
        string[] ugmsg = msg.Split(',');
        
        avp[0] = ugmsg[0];
        avp[1] = ugmsg[1];
        avp[2] = ugmsg[2];
        avp[3] = ugmsg[3];
        //avp[4] = ugmsg[4];
        //avp[5] = ugmsg[5];
       

        unx =float.Parse(avp[0]);
        uny = float.Parse(avp[1]);

        /*if (x == 1 || x == 0) x = tx;
        if (y == 1 || y == 0) y = ty;*/
        draw = int.Parse(avp[2]);
        poin = int.Parse(avp[3]);

        if (unx != 1 && unx != 0) tx = unx;
        if (uny != 1 && uny != 0) ty = uny;


        /*if ((unx/x)>0.5 && (unx / x)<2) */x = tx;
        /*if ((uny / y) > 0.5 && (uny / y) < 2) */y = ty;
        Debug.Log(x);
        Debug.Log(y);

        
    }


}
