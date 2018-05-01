using System;
using Windows.UI.Xaml.Data;

namespace TheWeather.Converters
{
    internal class IntFormatter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            if (value != null)
            {
                return System.Convert.ToInt32(value).ToString();
            }
            else
            {
                return null;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            int n;
            bool isNumeric = int.TryParse(value.ToString(), out n);
            if (isNumeric)
            {
                return n;
            }
            else
            {
                return 0;
            }
        }
    }
}
