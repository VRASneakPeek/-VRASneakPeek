namespace VRA.SneakPeek2019.CustomVision
{
    public sealed class PredictionModel
    {
        public float Probability { get; }
        public string TagName { get; }
        public BoundingBox BoundingBox { get; }

        public PredictionModel(float probability, string tagName, BoundingBox boundingBox)
        {
            Probability = probability;
            TagName = tagName;
            BoundingBox = boundingBox;
        }
    }
}
