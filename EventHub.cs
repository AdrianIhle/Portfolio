using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class EventHub : MonoBehaviour
{
    public static EventHub instance;
    int eventsQueued;
    public int maxEventsPerFrame;
    public int maxSubscriptionsPerFrame;

    [SerializeField]
    [Header("debug")]
    int eventCount;


    [SerializeField]
    List<EventDispatcher> dispatchers = new List<EventDispatcher>();
    [SerializeField]
    List<EventReciever> recievers = new List<EventReciever>();
    [SerializeField]
    Queue<EventReciever> subscriptionRequests = new Queue<EventReciever>();


    //[SerializeField]
    EventQueue constructionEvents = new EventQueue();
    //[SerializeField]
    EventQueue gatheringEvents = new EventQueue();
    //[SerializeField]
    EventQueue pickupEvents = new EventQueue();
    //[SerializeField]
    EventQueue inventoryEvents = new EventQueue();
    // [SerializeField]
    EventQueue populationEvents = new EventQueue();
   // [SerializeField]
    EventQueue questEvents = new EventQueue();
  //  [SerializeField]
    EventQueue genericEvents = new EventQueue();

    public void Awake()
    {
        instance = this;
    }

    public void Update()
    {
        eventsQueued = CountEvents();
        eventCount = eventsQueued;

        if(eventsQueued > 0)
        {
            ProcessGameEvents();
        }

        if(subscriptionRequests.Count > 0)
        {
            //print("subscription requests " + subscriptionRequests.Count);
            ProcessRequests();
        }
    }

    #region Events
    int CountEvents()
    {
        return constructionEvents.Count + gatheringEvents.Count + pickupEvents.Count+ inventoryEvents.Count + genericEvents.Count + populationEvents.Count + questEvents.Count;
    }

    void StartProcessingEvent(GameEvent gameEvent)
    {
        ThreadStart threadStart = delegate
        {
            ProcessEvent(gameEvent);
        };

        threadStart.Invoke();
    }

    void ProcessEvent(GameEvent gameEvent)
    {

        foreach(EventReciever r in gameEvent.receivers)
        {
            //print("advising " + r.gameObject.name + " of event " + gameEvent.message + " from " + gameEvent.dispatcher);
            r.Notify(gameEvent.message);
        }
    }

    void ProcessGameEvents()
    {
        int index = 0;

        for (index = 0; index < maxEventsPerFrame;)
        {
            int startIndex = index;
            if (constructionEvents.Count > 0)
            {
                StartProcessingEvent(constructionEvents.Dequeue());
                index++;
            }
            if (gatheringEvents.Count > 0)
            {
                StartProcessingEvent(gatheringEvents.Dequeue());
                index++;
            }
            if (pickupEvents.Count > 0)
            {
                StartProcessingEvent(pickupEvents.Dequeue());
                index++;
            }
            if (inventoryEvents.Count > 0)
            {
                StartProcessingEvent(inventoryEvents.Dequeue());
                index++;
            }
            if (populationEvents.Count > 0)
            {
                StartProcessingEvent(populationEvents.Dequeue());
                index++;
            }
            if (questEvents.Count > 0)
            {
                StartProcessingEvent(questEvents.Dequeue());
                index++;
            }
            if (genericEvents.Count > 0)
            {
                StartProcessingEvent(genericEvents.Dequeue());
                index++;
            }


            if (startIndex == index)
                break;
        }

    }

    public void QueueEvent(GameEvent gameEvent)
    {
        switch (gameEvent.eventType)
        {
            case EventType.Construction:
                constructionEvents.Enqueue(gameEvent);
                //print("construction events: " + constructionEvents.Count);
                break;
            case EventType.Gathering:
                gatheringEvents.Enqueue(gameEvent);
                //print("gathering events: " + gatheringEvents.Count);
                break;
            case EventType.Pickup:
                pickupEvents.Enqueue(gameEvent);
                //print("gathering events: " + gatheringEvents.Count);
                break;
            case EventType.Inventory:
                inventoryEvents.Enqueue(gameEvent);
                //print("inventory events: " + inventoryEvents.Count);
                break;
            case EventType.Generic:
                genericEvents.Enqueue(gameEvent);
                //print("generic events: " + genericEvents.Count);
                break;
            case EventType.Population:
                populationEvents.Enqueue(gameEvent);
                //print("population events: " + populationEvents.Count);
                break;
            case EventType.Quest:
                questEvents.Enqueue(gameEvent);
                //print("quest events: " + questEvents.Count);
                break;
            default:
                break;
        }
    }


    #endregion

    #region Dispatchers

    void ProcessRequests()
    {

        if (subscriptionRequests.Count > maxSubscriptionsPerFrame)
        {
            for (int i = 0; i < maxSubscriptionsPerFrame; i++)
            {
                //print("attempting to process requests");
                ProcessSubscriptionRequest(subscriptionRequests.Dequeue());
            }
        }
        else
        {
            for (int i = subscriptionRequests.Count; i > 0 ; i--)
            {
                ProcessSubscriptionRequest(subscriptionRequests.Dequeue());
            }
        }
    }

    public void AddSubscriptionRequest(EventReciever reciever)
    {
        subscriptionRequests.Enqueue(reciever);
    }

    void ProcessSubscriptionRequest(EventReciever reciever)
    {
        //print(" attempting to process sub request from " + reciever.gameObject.name);

        ThreadStart threadStart = delegate
        {
            SubscribeToEvents(reciever, reciever.listeningFor);
        };

        threadStart.Invoke();
    }

    void SubscribeToEvents(EventReciever reciever, EventType listenFor)
    {
        if (reciever.listeningFor.Equals(EventType.All))
        {
            foreach (EventDispatcher d in dispatchers)
            {
                d.SubscribeToDispatcher(reciever);
                reciever.AddToSubcribedList(d);
            }
        }
        else
        {
            List<EventDispatcher> ds = dispatchers.FindAll(d => d.eventType.Equals(listenFor));
            foreach (EventDispatcher d in ds)
            {
                d.SubscribeToDispatcher(reciever);
                reciever.AddToSubcribedList(d);
            }
        }
    }

    public void AddDispatcher(EventDispatcher dispatcher)
    {
        dispatchers.Add(dispatcher);

        EventReciever[] rs = recievers.FindAll(x => x.listeningFor.Equals(dispatcher.eventType)).ToArray();

        foreach(EventReciever r in rs)
        {
            dispatcher.SubscribeToDispatcher(r);
        }
    }

    public void RemoveDispatcher(EventDispatcher dispatcher)
    {
        dispatchers.Remove(dispatcher);
        dispatchers.TrimExcess();
    }

    #endregion

    #region Receivers
    public void AddReceiver(EventReciever reciever)
    {
        recievers.Add(reciever);
        AddSubscriptionRequest(reciever);
    }
    public void RemoveReceiver(EventReciever reciever)
    {
        recievers.Remove(reciever);
        recievers.TrimExcess();
    }
    #endregion
}
