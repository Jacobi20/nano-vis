using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.ServiceModel;
using System.Threading;
using System.Windows.Forms;
using WCFNanoGui.NanoVisualizerService;

namespace WCFNanoGui
{
    public partial class MainForm : Form
    {
        private const string inputFilesPath = @"C:\Temp\inputFiles";
        private string inputFilePath = null;

        #region Calculation data
        private Guid taskId = Guid.Empty;
        #endregion

        #region Данные необходимые между несколькими вызовами визуализатора

        // Путь к текущему cube-файлу
        private string currentCubeFile = null;
        
        // Загружен ли в сервис визуализации cube-файл
        private bool isCubeLoaded = false;

        // Идентификатор загруженного в сервис визуализации cube-файла
        private Guid loadedFileGuid = Guid.Empty;
        
        // Результат bacgroundWorkerVisualize сохраняется в следующую переменную
        private Image newImage = null;

        #endregion

        public MainForm()
        {
            InitializeComponent();
        }

        private void btnVisualize_Click(object sender, EventArgs e)
        {
            // Производим изменения компонентов интерфейса на форме
            progressBar1.Style = ProgressBarStyle.Blocks;
            progressBar1.MarqueeAnimationSpeed = 0;
            progressBar1.Maximum = 3;
            progressBar1.Step = 1;

            this.Cursor = Cursors.WaitCursor;

            btnCalculate.Enabled = false;
            btnVisualize.Enabled = false;

            // Формируем список параметров для вызова визуализатора
            Dictionary<string, string> pars = new Dictionary<string, string>();
            // Повороты вокруг осей
            pars.Add("pitch", trackBar1.Value.ToString());
            pars.Add("yaw", trackBar2.Value.ToString());
            pars.Add("roll", trackBar3.Value.ToString());

            pars.Add("atom_scale", "0.3");
            pars.Add("bond_radius", "0.1");

            // Запуск фонового потока визуализации
            backgroundWorkerVisualize.RunWorkerAsync(pars);
        }

        private void btnCalculate_Click(object sender, EventArgs e)
        {
            if(!File.Exists(comboBox1.Text))
            {
                MessageBox.Show(String.Format("Файл {0} не найден", comboBox1.Text), "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            inputFilePath = comboBox1.Text;

            // Производим изменения компонентов интерфейса на форме
            progressBar1.Style = ProgressBarStyle.Marquee;
            progressBar1.MarqueeAnimationSpeed = 75;

            this.Cursor = Cursors.WaitCursor;

            btnCalculate.Enabled = false;
            btnVisualize.Enabled = false;
            comboBox1.Enabled = false;
            imageOutput.Image = null;

            lbStatus.Text = "Вычисление";

            // Запуск фонового потока расчета
            backgroundWorkerCalculate.RunWorkerAsync();

        }

        // 
        private void backgroundWorkerVisualize_DoWork(object sender, DoWorkEventArgs e)
        {
            // Обнуляем результат работы
            newImage = null;

            VisualizerServiceClient proxy = null;
            try
            {
                backgroundWorkerVisualize.ReportProgress(0, "Создание прокси");
                // Создаем клиентский прокси
                proxy = new VisualizerServiceClient();

                proxy.ClientCredentials.UserName.UserName = "user0";
                proxy.ClientCredentials.UserName.Password = "passw0rd";

                // Если Cube-файл не был загружен в сервис визуализатора, то загружаем его
                if (!isCubeLoaded)
                {
                    backgroundWorkerVisualize.ReportProgress(1, String.Format("Загрузка файла {0} в сервис визуализатора", currentCubeFile));
                    FileStream fs = new FileStream(currentCubeFile, FileMode.Open,FileAccess.Read);
                    try
                    {
                        loadedFileGuid = proxy.uploadCubeFile(fs);
                    }
                    catch (Exception ex)
                    {
                        fs.Close();
                        throw ex;
                    }

                    if (loadedFileGuid != Guid.Empty)
                    {
                        isCubeLoaded = true;
                        backgroundWorkerVisualize.ReportProgress(1, String.Format("Идентификатор загруженного фала: {0}", loadedFileGuid.ToString()));
                    }
                    else
                    {
                        MessageBox.Show("Файл не может быть загружен.", "Ошибка в сервисе визуализации", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }

                // Формируем список параметров
                Dictionary<string, string> pars = (Dictionary<string, string>)e.Argument;

                // Визуализируем
                backgroundWorkerVisualize.ReportProgress(2, "Визуализация..");
                Stream st = proxy.visualizeCubeFile(loadedFileGuid, pars);
                Bitmap bmp = new Bitmap(st);

                newImage = bmp;

                backgroundWorkerVisualize.ReportProgress(3, "Визуализация завершена");
            }
            catch (EndpointNotFoundException ex0)
            {
                MessageBox.Show("Сервис визуализации недоступен", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                backgroundWorkerVisualize.ReportProgress(3, "Ошибка: Сервис визуализации недоступен");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Произошла ошибка во время обращения к сервису визуализации", MessageBoxButtons.OK, MessageBoxIcon.Error);
                backgroundWorkerVisualize.ReportProgress(3, "Ошибка: " + ex.Message);
            }
            finally
            {
                // Закрытие прокси
                if (proxy != null)
                {
                    try
                    {
                        if (proxy.State == CommunicationState.Opened)
                            proxy.Close();
                        else if (proxy.State == CommunicationState.Faulted)
                            proxy.Abort();
                    }
                    catch (CommunicationException)
                    {
                        proxy.Abort();
                    }
                    catch (TimeoutException)
                    {
                        proxy.Abort();
                    }
                }
            }
        }

        private void backgroundWorkerVisualize_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            progressBar1.Value = 0;
            imageOutput.Image = newImage;
            btnCalculate.Enabled = true;
            btnVisualize.Enabled = true;
            this.Cursor = Cursors.Default;

        }

        private void backgroundWorkerVisualize_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
            lbStatus.Text = (string)e.UserState;
        }

        private void backgroundWorkerCalculate_DoWork(object sender, DoWorkEventArgs e)
        {
            taskId = OrcaRunManager.Run(inputFilePath);

            while (OrcaRunManager.IsExecuting(taskId)) 
                Thread.Sleep(1000);
        }

        private void backgroundWorkerCalculate_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            currentCubeFile = OrcaRunManager.GetResultPath(taskId);
            isCubeLoaded = false;
            loadedFileGuid = Guid.Empty;

            // Иниациализация компонентов формы
            imageOutput.Image = null;
            trackBar1.Value = 180;
            trackBar1.Value = 180;
            trackBar1.Value = 180;
            panel2.Enabled = true;

            lbStatus.Text = "Вычисление завершено. Результат: " + currentCubeFile;

            progressBar1.MarqueeAnimationSpeed = 0;
            progressBar1.Value = 0;
            progressBar1.Style = ProgressBarStyle.Blocks;

            imageOutput.Image = null;
            btnCalculate.Enabled = true;
            btnVisualize.Enabled = true;
            comboBox1.Enabled = true;
            this.Cursor = Cursors.Default;
        }


        private void MainForm_Load(object sender, EventArgs e)
        {
            string[] files = Directory.GetFiles(inputFilesPath);
            comboBox1.Items.AddRange(files);
            if(files.Length>0)
                comboBox1.SelectedIndex = 0;
        }

    }
}
