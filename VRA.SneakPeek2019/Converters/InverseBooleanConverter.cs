﻿using System;
using Windows.UI.Xaml.Data;

namespace VRA.SneakPeek2019.Converters
{
    public class InverseBooleanConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            if (value is bool b)
            {
                return !b;
            }

            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            if (value is bool b)
            {
                return !b;
            }

            return value;
        }
    }
}
