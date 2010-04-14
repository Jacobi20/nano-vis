using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Text;
using System.Threading;

namespace WCFVisualizer
{
    class Visualizer
    {
        private const string CubesStoragePath = "cubes\\";
        private const string HashesStoragePath = "download\\";
        private const string ImagesStoragePath = "images\\";

        private const uint bufferSize = 4096;


        private object syncVis = new object();
        
        #region Singleton
        private static volatile Visualizer instance;
        private static readonly object _syncRoot = new Object();

        private Visualizer()
        {
        }

        public static Visualizer Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (instance == null)
                        {
                            instance = new Visualizer();
                        }
                    }
                }

                return instance;
            }
        }
        #endregion

        #region Private methods

        private const string nvisDllPath = @"run.dll";

        [DllImport(nvisDllPath)]
        private unsafe static extern void NVisInit();

        [DllImport(nvisDllPath)]
        private unsafe static extern void NVisShutdown();

        [DllImport(nvisDllPath)]
        private unsafe static extern void NVisCommand(byte[] cmd);

        private static void log(string s)
        {
            if(s.ToUpper().StartsWith("OK"))
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

        private void MakeShot(string param_list)
        {
            string command = "NVisSnapshot( { " + param_list + " } );";
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            NVisCommand(encoding.GetBytes(command));
        }
        
        private string GetMD5HashFromFile(string fileName)
        {
            FileStream file = new FileStream(fileName, FileMode.Open);
            MD5 md5 = new MD5CryptoServiceProvider();
            byte[] retVal = md5.ComputeHash(file);
            file.Close();

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < retVal.Length; i++)
            {
                sb.Append(retVal[i].ToString("x2"));
            }
            return sb.ToString();
        }
        
        private Dictionary<Guid,string> guidToMD5Dict = new Dictionary<Guid, string>();


        #endregion
        
        public void Init()
        {
            try
            {
                log("-> Nano visualizer initialization ");
                NVisInit();
                log("OK\n");
            }
            catch (Exception ex)
            {
                log("Failed\nException: "+ex.ToString()+"\n");
                throw ex;
            }
        }

        public void Close()
        {
            try
            {
                log("-> Nano visualizer shutdown ");
                NVisShutdown();
                log("OK\n");
            }
            catch (Exception ex)
            {
                log("Failed\nException: " + ex.ToString() + "\n");
                throw ex;
            }
        }

        /// <summary>
        /// File uploading.
        /// Thread safe.
        /// </summary>
        /// <param name="inputFile"></param>
        /// <returns></returns>
        public Guid uploadCubeFile(Stream inputFile)
        {
            Guid ret = Guid.NewGuid();

            try 
            {
                OperationContext context = OperationContext.Current;
                MessageProperties prop = context.IncomingMessageProperties;
                RemoteEndpointMessageProperty endpoint = prop[RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty;
                string ip = endpoint.Address;

                log("# Uploading file for client from [" + ip + "]\n");

                FileStream fs = new FileStream(CubesStoragePath + ret.ToString(), FileMode.Create);
                BinaryWriter bw = new BinaryWriter(fs);

                // сохраняем на диск
                byte [] buffer = new byte[bufferSize];
                int bytesRead = 0;
                while ((bytesRead = inputFile.Read(buffer, 0, buffer.Length)) > 0) 
                    bw.Write(buffer, 0, bytesRead);
                
                inputFile.Close();
                bw.Close();

                log("OK\n");
            }
            catch(Exception ex)
            {
                log("Error: "+ex.ToString()+"\n");
                ret = Guid.Empty;
            }

            return ret;
        }


        /// <summary>
        /// Interface function.
        /// Not thread safe.
        /// </summary>
        /// <param name="fid">File id. It has to be loaded by uploadCubeFile</param>
        /// <param name="parameters"></param>
        /// <returns>PNG image binary contents</returns>
        public Stream visualizeFile(Guid fid, Dictionary<string,string> parameters)
        {
            string fromFilePath = CubesStoragePath + fid.ToString();

            string hashFileName = null;

            if (guidToMD5Dict.Keys.Contains(fid))
                hashFileName = guidToMD5Dict[fid];
            else
            {
                // calculate hash
                hashFileName = GetMD5HashFromFile(fromFilePath);

                guidToMD5Dict.Add(fid,hashFileName);

                if (!File.Exists(hashFileName))
                {
                    // copy to cache
                    File.Copy(fromFilePath, hashFileName);
                    File.Delete(fromFilePath);
                }
            }

            string param_list = "";
            foreach (var parameter in parameters)
            {
                param_list += parameter.Key;
                param_list += " = ";
                param_list += "\"";
                param_list += parameter.Value;
                param_list += "\";";
            }

            //	set ["path"] :
            param_list += "path = " + "\"" + hashFileName + "\";";

            //	run visualizer :
            OperationContext context = OperationContext.Current;
            MessageProperties prop = context.IncomingMessageProperties;
            RemoteEndpointMessageProperty endpoint = prop[RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty;
            string ip = endpoint.Address;


            string ImageFileName = ImagesStoragePath + fid.ToString() + ".png";

            try
            {
                log("# Making shot for client from ["+ip+"]\n");
                log("#  Params: "+param_list);
                lock (syncVis)
                {
                    MakeShot(param_list);
                    if (File.Exists(ImageFileName))
                        File.Delete(ImageFileName);
                    File.Copy("shot.png", ImageFileName);
                }
                log("OK\n");
            }
            catch (Exception ex)
            {
                log("Failed\nException: " + ex.ToString() + "\n");
            }

            //	construct a response.
            FileStream pngImageStream = new FileStream(ImageFileName, FileMode.Open);

            return pngImageStream;
        }
    }
}
