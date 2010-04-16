using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace WCFNanoGui
{

    public class OrcaRunManager
    {
        //private static string[] OrcaFiles = Directory.GetFiles(@"c:\ORCA\");
        public static string WorkDir = @"d:\temp\";

        /// <summary>
        /// Запуск с указанным входным файлом
        /// </summary>
        /// <param name="inputFilePath">Входной файл для запуска</param>
        /// <returns>ID запуска</returns>
        static public Guid Run(string inputFilePath)
        {
            if (!File.Exists(inputFilePath))
                throw new FileNotFoundException(string.Format("File '{0}' doesn't exist.", inputFilePath));
            Guid runId = Guid.NewGuid();
            string runDir = WorkDir + runId.ToString() + @"\";
            string runInputFile = runDir + "input.inp";
            Directory.CreateDirectory(runDir);
            File.Copy(inputFilePath, runInputFile);
            //foreach (string file in OrcaFiles)
            //    File.Copy(file, runDir + Path.GetFileName(file));
            ProcessStartInfo procStartInfo = new ProcessStartInfo();
            procStartInfo.FileName = /*runDir + */"orca.exe";
            procStartInfo.Arguments = runInputFile;
            procStartInfo.WorkingDirectory = runDir;
            procStartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            Process proc = Process.Start(procStartInfo);
            File.WriteAllText(runDir + "proc.txt", proc.Id.ToString());
            return runId;
        }

        /// <summary>
        /// Проверка состояния процесса
        /// </summary>
        /// <param name="runId">ID запуска</param>
        /// <returns>Процесс запущен</returns>
        static public bool IsExecuting(Guid runId)
        {
            string runDir = WorkDir + runId.ToString() + @"\";
            if (!Directory.Exists(runDir))
                return false;
            try
            {
                Process proc = Process.GetProcessById(int.Parse(File.ReadAllText(runDir + "proc.txt")));
                return (proc != null) && !(proc.HasExited);
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Получение имени файла с результатом (CUBE)
        /// </summary>
        /// <param name="runId">ID запуска</param>
        /// <returns>Путь к файлу</returns>
        static public string GetResultPath(Guid runId)
        {
            string resName = WorkDir + runId.ToString() + @"\eldens.cube";
            if (!File.Exists(resName))
                return null;
            return resName;
        }
    }
}