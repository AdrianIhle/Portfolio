using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventReciever : MonoBehaviour
{
    [SerializeField]
    public EventType listeningFor;
    [HideInInspector]
    public List<EventDispatcher> listeningTo = new List<EventDispatcher>();
    public bool useEventHub = true;
    public bool recieverActive = true;

    [Header("Debug")]
    [SerializeField]
    bool printReceptions;
    public void Start()
    {
        if (recieverActive && useEventHub)
            InitializeReciever();
    }

    public void InitializeReciever()
    {
        if (useEventHub)
        {
            EventHub.instance.AddReceiver(this);
        }
    }

    public void AddToSubcribedList(EventDispatcher d)
    {
        listeningTo.Add(d);
    }

    public void RemoveFromSubcribedList(EventDispatcher d)
    {
        int di= listeningTo.FindIndex(x => x.gameObject.GetInstanceID() == d.GetInstanceID());
        listeningTo.RemoveAt(di);
        listeningTo.TrimExcess();
    }

    public bool IsAlreadySubscribed(EventDispatcher d)
    {
        if (listeningTo.Count > 0)
            return listeningTo.Exists(x => x.gameObject.GetInstanceID() == d.GetInstanceID());
        else
            return false;
    }

    public bool Notify(string message)
    {
        if(message == null)
        {
            message = " ";
        }
        if (recieverActive)
        {
            if(printReceptions)
                print(this.gameObject.name + " recieved " + message);
            try
            {
                gameObject.SendMessage("EventRecieved", message);
            }
            catch (Exception e)
            {
                print($"{gameObject.name} caught an exception: " + e.Message);
            }

            return true;
        }
        else
            return false;
    }
}
