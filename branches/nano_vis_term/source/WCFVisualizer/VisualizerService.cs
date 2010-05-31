using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace WCFVisualizer
{
    // NOTE: If you change the class name "VisualizerService" here, you must also update the reference to "VisualizerService" in App.config.
    public class VisualizerService : IVisualizerService
    {
        private static void log(string s)
        {
            if (s.ToUpper().StartsWith("OK"))
            {
                Console.ForegroundColor = ConsoleColor.Green;
            }
            else if (s.ToUpper().StartsWith("FAIL") || s.ToUpper().StartsWith("ERROR"))
            {
                Console.ForegroundColor = ConsoleColor.Red;
            }
            Console.Write(s);
            Console.ForegroundColor = ConsoleColor.Gray;
        }

        public Guid uploadCubeFile(Stream inpFile)
        {
            Guid ret = Guid.Empty;
            try
            {
                ret = Visualizer.Instance.uploadCubeFile(inpFile);
            }catch(Exception ex)
            {
                log("Fail: "+ex.ToString());
            }
            return ret;
        }

        public Stream visualizeCubeFile(Guid fid, Dictionary<string,string> param)
        {
            Stream ret = null;

            try
            {
                ret = Visualizer.Instance.visualizeFile(fid, param);
            }catch(Exception ex)
            {
                log("Fail: "+ex.ToString());                
            }

            return ret;
        }
    }
}
