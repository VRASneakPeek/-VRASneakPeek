namespace VRA.SneakPeek2019.CustomVision
{
    public sealed class BoundingBox
    {
        public float Left { get; }
        public float Top { get; }
        public float Width { get; }
        public float Height { get; }

        public BoundingBox(float left, float top, float width, float height)
        {
            Left = left;
            Top = top;
            Width = width;
            Height = height;
        }
    }
}
