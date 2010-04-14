using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;

namespace WCFVisualizer
{
    class Program
    {
        static void Main(string[] args)
        {
            ServiceHost host;
            
            host = new ServiceHost(typeof(VisualizerService));

            try
            {
                Visualizer.Instance.Init();
            }catch(Exception ex)
            {
                return;
            }

            try
            {
                Console.Write("Starting VisualizerService.. ");
                host.Open();
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("OK");
                Console.ForegroundColor = ConsoleColor.Gray;
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Failed");
                Console.WriteLine(ex);
                Console.ForegroundColor = ConsoleColor.Gray;
                return;
            }

            Console.WriteLine("[Press any key to close]");
            Console.ReadKey();
            Console.WriteLine("Closing VisualizerService..");

            host.Close();

            try
            {
                Visualizer.Instance.Close();
            }catch(Exception ex)
            {
                return;
            }
        }
    }
}
