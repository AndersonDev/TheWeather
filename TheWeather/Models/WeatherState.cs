using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TheWeather
{
    public class WeatherState
    {
        public DateTime Date { get; set; }
        public int AvDayTemp { get; set; } = 0;

        public string DayName 
        {
            get 
            {
                return Date.DayOfWeek.ToString();
            }
        }
    }
}
