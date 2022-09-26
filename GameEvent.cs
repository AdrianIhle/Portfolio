using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class GameEvent 
{
    [SerializeField]
    public EventType eventType;
    [SerializeField]
    public EventDispatcher dispatcher;
    [SerializeField]
    public EventReciever[] receivers;
    [SerializeField]
    public string message;
    public GameEvent (EventReciever[] _receivers, EventDispatcher _eventDispatcher, EventType _eventType, string _message)
    {
        receivers = _receivers;
        dispatcher = _eventDispatcher;
        eventType = _eventType;
        message = _message;
    }


}

public enum EventType
{
    All,
    Generic,
    Gathering,
    Pickup,
    Inventory,
    Quest,
    Population,
    Construction,
    Dialogue
}
