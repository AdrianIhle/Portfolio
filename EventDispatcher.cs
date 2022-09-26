using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventDispatcher : MonoBehaviour
{
    [SerializeField]
    EventHub eventHub;
    [SerializeField]
    bool triggerEvent;
    [SerializeField]
    public EventType eventType;
    [SerializeField]
    string eventMessage;

    [SerializeField]
    List<EventReciever> registeredReceivers = new List<EventReciever>();

    // Start is called before the first frame update
    void Start()
    {
        eventHub = EventHub.instance;
        eventHub.AddDispatcher(this);
    }

    // Update is called once per frame

    void Update()
    {
        if(triggerEvent)
        {
            print("event triggered on " + gameObject.name);
            triggerEvent = false;
            TriggerEvent(eventMessage);
        }
    }

    public void ChangeEventType(EventType _eventType)
    {
        eventType = _eventType;
    }

    public void ChangeEventMessage(string _eventMessage)
    {
        eventMessage = _eventMessage;
    }

    public void TriggerEvent(string message)
    {
        //print("dispatcher on " + gameObject.name + " messaging " + message);
        eventHub.QueueEvent(new GameEvent(registeredReceivers.ToArray(), this, eventType, message));
    }

    public void TriggerEvent()
    {
        eventHub.QueueEvent(new GameEvent(registeredReceivers.ToArray(),this, eventType, eventMessage));
    }

    public void SubscribeToDispatcher(EventReciever eventReciever)
    {
        if(!registeredReceivers.Exists(x => x.GetInstanceID().Equals(eventReciever.GetInstanceID())))
            registeredReceivers.Add(eventReciever);
    }
    public void UnsubscribeFromDispatcher(EventReciever eventReciever)
    {
        registeredReceivers.Remove(eventReciever);
    }

    private void OnDestroy()
    {
        try
        {
            eventHub.RemoveDispatcher(this);
        }
        catch(Exception e)
        {
            print(e.Message);
        }
    }
}
