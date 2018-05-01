using Newtonsoft.Json;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Net.Http;
using Windows.UI.Xaml.Controls;
using Effects;

namespace TheWeather
{
    public sealed partial class MainPage : Page
    {
        WeatherData weatherData = new WeatherData();
        ForecastData forecastData = new ForecastData();
        ObservableCollection<WeatherState> weatherConditions = new ObservableCollection<WeatherState>();
        Effects.EffectsRenderer r;

        string cowerUrl = "https://images.unsplash.com/photo-1453813063438-1ed846c7daf0";
        string currentDate = "";

        public MainPage()
        {
            this.InitializeComponent();

            SetDate();
            GetWeather();
            GetForecast();
            InitSwapChain();
        }

        void InitSwapChain()
        {
            r = new Effects.EffectsRenderer(swapChainPanel);
            swapChainPanel.SizeChanged += (object sender, Windows.UI.Xaml.SizeChangedEventArgs e) =>
            {
                r.SetLogicalSize(e.NewSize);
                r.Draw();
            };
        }

        void SetDate()
        {
            currentDate = DateTime.Now.Date.ToString("dd/MM/yyyy");
            Bindings.Update();
        }

        async void GetWeather()
        {
            string cityId = "703448";
            string url = String.Format("https://api.openweathermap.org/data/2.5/weather?id={0}&APPID={1}&units=metric", cityId, AppSettings.OpenWeatherMapAPIKey);

            HttpClient client = new HttpClient();

            string response = await client.GetStringAsync(url);

            try
            {
                weatherData = JsonConvert.DeserializeObject<WeatherData>(response);
            }
            catch { }

            Bindings.Update();

            GetCover();
        }

        async void GetForecast()
        {
            string cityId = "703448";
            string url = String.Format("https://api.openweathermap.org/data/2.5/forecast?id={0}&APPID={1}&units=metric", cityId, AppSettings.OpenWeatherMapAPIKey);

            HttpClient client = new HttpClient();

            string response = await client.GetStringAsync(url);

            try
            {
                forecastData = JsonConvert.DeserializeObject<ForecastData>(response);

                DateTime day = DateTime.Now;

                for (int i = 0; i < 6; i++)
                {
                    WeatherState weatherCond = new WeatherState
                    {
                        Date = day.Date
                    };

                    forecastData.Forecasts.FindAll(f => f.Dt.Date == day.Date).ForEach( f => {
                        if (weatherCond.AvDayTemp < (int)f.Main.TempMax)
                            weatherCond.AvDayTemp = (int)f.Main.TempMax;
                    });

                    day = day.AddDays(1);

                    weatherConditions.Add(weatherCond);
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e);
            }
        }

        async void GetCover()
        {
            string apiKey = "8770358-aa31097c249df426d3aa19daf";
            string searchPattern = weatherData.Weather[0].Main;
            string requestUrl = String.Format("https://pixabay.com/api/?key={0}&q={1}&image_type=photo", apiKey, searchPattern);

            HttpClient client = new HttpClient();

            string response = await client.GetStringAsync(requestUrl);

            PixabayData pixabayData = JsonConvert.DeserializeObject<PixabayData>(response);

            cowerUrl = pixabayData.hits[0].largeImageURL;

            Bindings.Update();
            r.Draw();
        }
    }
}
