using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//this is a standard implementation of a generic max heap binary tree structure (highest value on top)
public interface IHeapItem<T> : IComparable<T>
{
    int HeapIndex
    {
        get;
        set;
    }
}

public class Heap<T> where T : IHeapItem<T>
{
    public T[] items;
    int currentItemCount;
    
    //creates the heap, setting the array size
    public Heap(int maxHeapSize)
    {
        items = new T[maxHeapSize];
    }

    //add an item to the heap, assigning it to the end of the array
    // triggers a sort to place the item at the right index
    //increments the item count/end of heap index
    public void AddItem(T item)
    {
        item.HeapIndex = currentItemCount;
        items[currentItemCount] = item;
        SortUp(item);
        currentItemCount++;
    }

    //removes the very first item in the heap, reassigns the root, sorts the root again to ensure the appropriate item is at the root, then returns the initially removed item.
    public T RemoveFirst()
    {
        T firstItem = items[0];
        currentItemCount--;
        items[0] = items[currentItemCount];
        items[0].HeapIndex = 0;
        SortDown(items[0]);
        return firstItem;
    }

    //finds the parent of the item by normal binary tree math) and compares the item and the parent, swapping them if the item is bigger than their parent
    void SortUp(T item)
    {
        int parentIndex = (item.HeapIndex - 1) / 2;

        while(true)
        {
            T parentItem = items[parentIndex];

            if(item.CompareTo(items[parentIndex]) > 0)
            {
                Swap(item, parentItem);
            }
            else
            { 
                break;
            }
        }
    }


    //finds the children of the item, figures out whether the candiate for the swap is left or right child 
    //left filled in first, right filled in after, right only assinged if it is of lower priority than left child
    //if the parent item is of lower priority than its comparison target, swap them. 
    void SortDown(T item)
    {
        while(true)
        {
            int left=  item.HeapIndex*2+1;
            int right = item.HeapIndex*2+2;
            int swapIndex = 0;

            if (left < currentItemCount)
            {
                swapIndex = left;
                if (right < currentItemCount)
                {
                    if (items[left].CompareTo(items[right]) < 0)
                    {
                        swapIndex = right;
                    }
                }

                if (item.CompareTo(items[swapIndex]) < 0)
                {
                    Swap(item, items[swapIndex]);
                }
                else
                {
                    return;
                }
            }
            else
                return;
        }
    }

    void Swap(T itemA, T itemB)
    {
        items[itemA.HeapIndex] = itemB;
        items[itemB.HeapIndex] = itemA;
        int itemAindex = itemA.HeapIndex;
        itemA.HeapIndex = itemB.HeapIndex;
        itemB.HeapIndex = itemAindex;
    }

    public int HeapSize()
    {
        return currentItemCount;
    }

    public T GetLeft(int index)
    {
        return items[index * 2];
    }

    public T GetRight(int index)
    {
        return items[index * 2 + 1];
    }

    public T GetParent(int index)
    {
        return items[index / 2];
    }

    bool HasLeft(int index)
{
    return index * 2 +1 <= HeapSize();
}

    bool HasRight(int index)
    {
        return index * 2+2 <= HeapSize();
    }

    T GetRoot()
    {
        return items[0];
    }

    public bool Contains(T item)
    {
        return Equals(items[item.HeapIndex], item);
    }

    public void UpdateItem(T item)
    {
        SortUp(item);
    }

    public void Clear()
    {
        while (currentItemCount > 0)
            RemoveFirst();
    }
}


