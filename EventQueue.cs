using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class EventQueue
{
    [SerializeField]
    Queue<GameEvent> queue = new Queue<GameEvent>();
    [SerializeField]
    public int Count { get => queue.Count; }

    public void Enqueue(GameEvent _event)
    {
        queue.Enqueue(_event);
    }
    public GameEvent Dequeue()
    {
        return queue.Dequeue();
    }

}
