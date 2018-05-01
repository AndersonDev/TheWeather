using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace TheWeather
{
    public class WeatherData
    {
        public Coord Coord { get; set; }
        public Sys Sys { get; set; }
        public List<Weather> Weather { get; set; }
        public Main Main { get; set; }
        public Wind Wind { get; set; }
        public Rain Rain { get; set; }
        public Clouds Clouds { get; set; }
        [JsonConverter(typeof(UnixTimestampConverter))]
        public DateTime Dt { get; set; }
        public int Id { get; set; }
        public string Name { get; set; }
        public int Cod { get; set; }
    }

    public class ForecastData
    {
        public string Cod { get; set; }
        public float Message { get; set; }
        public int Cnt { get; set; }
        [JsonProperty(PropertyName = "list")]
        public List<Forecast> Forecasts { get; set; }
        public City City { get; set; }
    }

    public class City
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public Coord Coord { get; set; }
        public string Country { get; set; }
    }

    public class Forecast
    {
        [JsonConverter(typeof(UnixTimestampConverter))]
        public DateTime Dt { get; set; }
        public Main Main { get; set; }
        public List<Weather> Weather { get; set; }
        public Clouds Clouds { get; set; }
        public Wind Wind { get; set; }
        public Sys Sys { get; set; }
        public Rain Rain { get; set; }
        public Snow Snow { get; set; }
    }

    public class Coord
    {
        public float Lon { get; set; }
        public float Lat { get; set; }
    }

    public class Sys
    {
        public string Country { get; set; }
        public int Sunrise { get; set; }
        public int Sunset { get; set; }
    }

    public class Main
    {
        public float Temp { get; set; }
        public int Humidity { get; set; }
        public float Pressure { get; set; }
        [JsonProperty(PropertyName = "temp_min")]
        public float TempMin { get; set; }
        [JsonProperty(PropertyName = "temp_max")]
        public float TempMax { get; set; }
    }

    public class Wind
    {
        public float Speed { get; set; }
        public float Deg { get; set; }
    }

    public class Rain
    {
        public int _3h { get; set; }
    }

    public class Snow
    {
        public float _3h { get; set; }
    }

    public class Clouds
    {
        public int All { get; set; }
    }

    public class Weather
    {
        public int Id { get; set; }
        public string Main { get; set; }
        public string Description { get; set; }
        public string Icon { get; set; }
    }
}