using UnityEngine;

public class IntNode : TwoDGridNode<IntNode>
{
    [SerializeField] private int value;

    //these values should be easily alterable and affect no other values unless a method is called so can remain public
   [Header("Value Limits")]
   [SerializeField] public int minValue = -256;
   [SerializeField] public int maxValue = 256;



    public IntNode (int _gridX, int _gridY, Vector3 _worldPosition, float _diameter, int _value, int _minValue, int _maxValue) : base (_gridX, _gridY, _worldPosition, _diameter)
   {
       this.value = _value;
       this.minValue = _minValue;
       this.maxValue = _maxValue;
        base.containedObject = this;
        base.CalculateBounds();
        base.debugColor = new Color(GetNormalizedValue(), GetNormalizedValue(), GetNormalizedValue(), GetNormalizedValue());
   }

    public override string ToString()
    {
        return ($"index: {gridPosition}, position: {worldPosition}, value: {value}");
    }

    public void SetValue(int _newValue)
    {
        value = _newValue;
        value = Mathf.Clamp(value, minValue, maxValue);
    }

    public void AlterValueBy(int _value)
    {
        value += _value;
        value = Mathf.Clamp(value, minValue, maxValue);
    }

    public int GetValue()
    {
        return value;
    }

    public float GetNormalizedValue()
    {
        float average = (float)(minValue + maxValue) / 2.0f;
        float range = (float)(maxValue -minValue) / 2.0f;
        float normalizedValue = (float)(value-minValue)/(float)(maxValue-minValue);

       return normalizedValue;
    }

    public static implicit operator int(IntNode n) => n.value;
}
